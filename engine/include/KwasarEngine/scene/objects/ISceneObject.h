//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 12.02.24.
//

#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H

#include <KwasarEngine/utils/ReportMessageFwd.h>
#include <map>

#include "KwasarEngine/utils/PropertyBase.h"

namespace ke {
class UUID;
enum class MouseButton;
class ModifierKeys;
enum class KeyboardKey;
} // namespace ke

namespace ke {
class IScript;
class MainObjectExtension;
class Extension;
class IScene;

class ISceneObject {
public:
	virtual ~ISceneObject() = default;

	virtual ReportMessagePtr addExtension(const std::string &pName, const std::shared_ptr<Extension> &pExtension) = 0;

	virtual ReportMessagePtr removeExtension(const std::string &pName, std::shared_ptr<Extension> &pExtension) = 0;

	virtual ReportMessagePtr transferExtensionTo(const std::string &pName, ISceneObject* pTransferTo) = 0;

	virtual sigc::connection connectOnExtensionAdded(
		const sigc::slot<void(const std::shared_ptr<Extension> & pNewExt)> &pSlot) = 0;

	virtual sigc::connection connectOnExtensionRemoved(
		const sigc::slot<void(const std::shared_ptr<Extension> & pExtToRemove)> &pSlot) = 0;


	virtual sigc::connection connectOnExtensionPropertyChanged(
		const sigc::slot<void(Extension * pExtension, PropertyBase * pProperty)> &pSlot) = 0;

	virtual void notifyExtensionPropertyChanged(Extension* pExtension, PropertyBase* pProperty) = 0;

	virtual ReportMessagePtr init() = 0;

	virtual void deinit() = 0;

	virtual void render() const = 0;

	virtual void onWindowSizeChanged(int pWidth, int pHeight) const = 0;

	virtual void onCursorPosChanged(double pX, double pY) const = 0;

	virtual void onKeyStateChanged(KeyboardKey pKey, bool pPressed, const ModifierKeys &pMods) const = 0;

	virtual void onMouseButtonStateChanged(MouseButton pButton, bool pPressed, double pX, double pY) const = 0;

	virtual void setScene(IScene* pScene) = 0;

	virtual void setUuid(const std::shared_ptr<UUID> &pUuid) = 0;

	virtual UUID* getUuid() = 0;

	virtual const std::map<std::string, std::shared_ptr<Extension>> &getExtensions() const = 0;

	[[nodiscard]] virtual MainObjectExtension* getMainExtension() const = 0;

	[[nodiscard]] virtual const std::string &getScriptName() const = 0;

	virtual void setScriptName(const std::string &pScriptName) = 0;

	[[nodiscard]] virtual const std::shared_ptr<IScript> &getScript() const = 0;

	virtual void setScript(const std::shared_ptr<IScript> &pScript) = 0;

	virtual bool notifyOnMouseScroll(double pDx, double pDy) = 0;

protected:
	virtual bool onMouseScroll(double pDx, double pDy);
};

inline bool ISceneObject::onMouseScroll(double /*pDx*/, double /*pDy*/) { return false; }
} // namespace ke

#endif //ISCENEOBJECT_H