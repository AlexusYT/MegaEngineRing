//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 27.02.24.
//

#ifndef EXTENSION_H
#define EXTENSION_H
#include <nlohmann/json_fwd.hpp>
#include <sigc++/signal.h>

#include "KwasarEngine/resources/Resources.h"
#include "KwasarEngine/scene/IScene.h"
#include "KwasarEngine/utils/IPropertyProvider.h"
#include "KwasarEngine/utils/ReportMessageFwd.h"
#include "ExtensionProperty.h"

namespace ke {
class Node;
}

namespace ke {
class ModifierKeys;
enum class KeyboardKey;
} // namespace ke

namespace ke {
class SceneObject;


#define METHOD_CREATE(__CLASS)                                                                                         \
	static std::shared_ptr<__CLASS> create() { return Extension::create(new __CLASS, trimFilename(__FILE__)); }

#define EXT_TYPE_NAME(__TYPE_NAME)                                                                                     \
	static const char* typeName() { return __TYPE_NAME; }

class Extension : public virtual IPropertyProvider {
	std::vector<PropertyBase*> properties;
	friend SceneObject;
	SceneObject* object{};
	Node* node{};
	std::vector<sigc::connection> connectionStorage;
	std::string typeNameStr;
	std::string name;
	std::string header;
	bool enabled = true;
	sigc::signal<void()> onEnabledChangedSignal;

protected:
	Extension();

public:
	using ValueChanged = sigc::signal<void()>;
	template <typename... Args>
	using Method = sigc::slot<void(Args...)>;

	static constexpr auto trimFilename(const std::string_view &pFilename) {
		return pFilename.substr(pFilename.find("include") + 8);
	}

	template <typename ClassT>
	static std::shared_ptr<ClassT> create(ClassT* pPtr = new ClassT(), std::string_view pFilePath = "") {
		static_assert(std::derived_from<ClassT, Extension>, "ClassT must be derived from Extension");
		auto ptr = std::shared_ptr<ClassT>(pPtr);
		getTypeNameFor(pPtr, ptr->typeNameStr);
		ptr->setHeader(std::string(pFilePath));
		return ptr;
	}

	~Extension() override = default;

	[[nodiscard]] const std::vector<PropertyBase*> &getProperties() const { return properties; }

	[[nodiscard]] SceneObject* getObject() const { return object; }

	static const char* typeName() { return nullptr; }

	[[nodiscard]] const std::string &getTypeName() const { return typeNameStr; }

	[[nodiscard]] bool isEnabled() const { return enabled; }

	void setEnabled(const bool pEnabled = true) {
		enabled = pEnabled;
		onEnabledChangedSignal();
		onEnabledChanged();
	}

	[[nodiscard]] sigc::signal<void()> &getOnEnabledChangedSignal() { return onEnabledChangedSignal; }

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getHeader() const { return header; }

	void setHeader(const std::string &pHeader) { header = pHeader; }

	void serialize(nlohmann::json &pJson);

	void deserialize(const nlohmann::json &pJson);

	virtual ReportMessagePtr onInit() { return nullptr; }

	virtual ReportMessagePtr onDeinit() {
		freeConnectionStorage();
		return nullptr;
	}

	//TODO Documentation
	/**
	 * @brief NON BLOCKING!
	 * @tparam ClassT
	 * @param pResourceUri
	 * @param pSlot NOT in a main context
	 */
	void loadResourceAsync(const std::string &pResourceUri,
						   const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> &pResult)> &pSlot) const {
		getScene()->loadResourceAsync(pResourceUri,
									  [pSlot](const std::shared_ptr<ResourceLoadResult> &pResult) { pSlot(pResult); });
	}

	std::shared_ptr<IResource> loadResourceSync(const std::string &pResourceUri) const {
		return getScene()->loadResourceSync(pResourceUri);
	}

	IScene* getScene() const;

	bool notifyOnMouseScroll(double pDx, double pDy);

	void setNode(Node* pNode) {
		if (node == pNode) return;
		auto oldNode = node;
		node = pNode;
		onNodeChanged(oldNode);
	}

	virtual void onNodeChanged(Node* pOldNode);

	[[nodiscard]] Node* getNode() const { return node; }

protected:
	void putConnectionToStorage(const sigc::connection &pConnection) { connectionStorage.emplace_back(pConnection); }

	void freeConnectionStorage() {
		for (auto &connection: connectionStorage) { if (connection.connected()) connection.disconnect(); }
	}

	virtual void onRender() {}

	virtual void onWindowSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyStateChanged(KeyboardKey pKey, bool pPressed, const ModifierKeys &pMods);

	virtual void onMouseButtonStateChanged(MouseButton pButton, bool pPressed, double pX, double pY) const;

	virtual void onEnabledChanged();

	virtual bool onMouseScroll(double pDx, double pDy);

public:
	void setObject(SceneObject* const pObject) { object = pObject; }

private:
	static void getTypeNameFor(Extension* pExt, std::string &pNameOut);

	void addProperty(PropertyBase* pProperty) override;

	void removeProperty(PropertyBase* pProperty) override;

	void propertyChanged(PropertyBase* pProperty) override;
};
} // namespace ke


#endif //EXTENSION_H
