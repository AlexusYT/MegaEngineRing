// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 27.02.24.
//

#ifndef EXTENSION_H
#define EXTENSION_H
#include <sigc++/signal.h>

#include "EngineSDK/main/resources/Resources.h"
#include "EngineSDK/main/scene/IScene.h"
#include "EngineUtils/utils/ReportMessageFwd.h"
#include "ExtensionProperties.h"

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::sdk::main {
class SceneObject;


#define METHOD_CREATE(__CLASS)                                                                                         \
	static std::shared_ptr<__CLASS> create() { return Extension::create(new __CLASS); }

#define EXT_TYPE_NAME(__TYPE_NAME)                                                                                     \
	static const char* typeName() { return __TYPE_NAME; }

class Extension {
	friend SceneObject;
	SceneObject* object{};
	std::vector<sigc::connection> connectionStorage;
	std::string typeNameStr;
	bool enabled = true;
	sigc::signal<void()> onEnabledChangedSignal;

protected:
	Extension();

public:
	using ValueChanged = sigc::signal<void()>;
	template<typename... Args>
	using ValueChangedArgs = sigc::signal<void(Args...)>;
	template<typename... Args>
	using Method = sigc::slot<void(Args...)>;

	template<typename ClassT>
	static std::shared_ptr<ClassT> create(ClassT* pPtr = new ClassT()) {
		static_assert(std::derived_from<ClassT, Extension>, "ClassT must be derived from Extension");
		auto ptr = std::shared_ptr<ClassT>(pPtr);
		getTypeNameFor(pPtr, ptr->typeNameStr);
		return ptr;
	}

	virtual ~Extension() = default;

	[[nodiscard]] SceneObject* getObject() const { return object; }

	virtual void getProperties(ExtensionProperties &pProperties);

	static const char* typeName() { return nullptr; }

	[[nodiscard]] const std::string &getTypeName() const { return typeNameStr; }

	[[nodiscard]] bool isEnabled() const { return enabled; }

	void setEnabled(const bool pEnabled = true) {
		enabled = pEnabled;
		onEnabledChangedSignal();
		onEnabledChanged();
	}

	[[nodiscard]] sigc::signal<void()> &getOnEnabledChangedSignal() { return onEnabledChangedSignal; }

protected:
	template<RequestRegularConcept ClassT>
	void enqueueResourceLoading(const std::shared_ptr<ClassT> &pRequest,
								const sigc::slot<void(const std::shared_ptr<typename ClassT::ResourceT> &pResource,
													  const utils::ReportMessagePtr &pError)> &pSlot) {
		getScene()->enqueueResourceLoading(
			pRequest, [pSlot](const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError) {
				pSlot(std::dynamic_pointer_cast<typename ClassT::ResourceT>(pResource), pError);
			});
	}

	IScene* getScene() const;

	void putConnectionToStorage(const sigc::connection &pConnection) { connectionStorage.emplace_back(pConnection); }

	void freeConnectionStorage() {
		for (auto &connection: connectionStorage) {
			if (connection.connected()) connection.disconnect();
		}
	}

	virtual utils::ReportMessagePtr onInit() { return nullptr; }

	virtual utils::ReportMessagePtr onDeinit() {
		freeConnectionStorage();
		return nullptr;
	}

	virtual void onRender() {}

	virtual void onWindowSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods);

	virtual void onMouseButtonStateChanged(utils::MouseButton pButton, bool pPressed, double pX, double pY) const;

	template<typename T, typename T1 = std::remove_cvref_t<T>, typename ClassT>
	std::shared_ptr<ExtensionProperty<T1>> createProperty(const std::string &pName, const std::string &pDescription,
														  T (ClassT::*pGetterFunc)() const,
														  void (ClassT::*pSetterFunc)(T)) {
		return std::make_shared<ExtensionProperty<T1>>(
			pName, pDescription, sigc::mem_fun<T, ClassT>(*dynamic_cast<ClassT*>(this), pGetterFunc),
			sigc::mem_fun<void, ClassT>(*dynamic_cast<ClassT*>(this), pSetterFunc));
	}

	virtual void onEnabledChanged();

private:
	void setObject(SceneObject* const pObject) { object = pObject; }

	static void getTypeNameFor(Extension* pExt, std::string &pNameOut);
};
} // namespace mer::sdk::main


#endif //EXTENSION_H
