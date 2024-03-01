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
// Created by alexus on 31.01.24.
//

#ifndef ISCENE_H
#define ISCENE_H
#include <sigc++/slot.h>

#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::editor::mvp {
class PresenterSceneEditor;
} // namespace mer::editor::mvp

namespace mer::sdk::main {
class ResourceRequest;
class IResources;
class ProgramWideShaderBuffer;

class IScene {
	friend class MainWindow;
	friend class editor::mvp::PresenterSceneEditor;

public:
	virtual ~IScene() = default;

	virtual sdk::utils::ReportMessagePtr initScene() = 0;

	[[nodiscard]] virtual IResources* getResources() const = 0;


	virtual void onResourceLoadingError(const std::shared_ptr<ResourceRequest> &pRequest,
										const sdk::utils::ReportMessagePtr &pError) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ProgramWideShaderBuffer> &getProgramBuffer() const = 0;
	virtual void enqueueResourceLoading(
		const std::shared_ptr<ResourceRequest> &pRequest,
		const sigc::slot<void(const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError)>
			&pSlot) const = 0;

private:
	virtual void setResources(IResources* pResources) = 0;

	virtual void render() = 0;

	virtual void resize(int pWidth, int pHeight) = 0;

	virtual void onCursorPosChanged(double pX, double pY) = 0;

	virtual void onKeyChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) = 0;
};
} // namespace mer::sdk::main


#endif //ISCENE_H
