//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 15.02.25.
//

#ifndef VIEWPREFABEDITOR_H
#define VIEWPREFABEDITOR_H
#include "KwasarEngine/ui/UiWindow.h"
#include "IViewPrefabEditor.h"

namespace ked {
class SubWindows;
class SceneOverlayElements;
class IPresenterPrefabEditor;
} // namespace ked

namespace ke {
class Framebuffer;
class GltfModel;
class PrefabInstance;
class ProgramWideShaderBuffer;
class OrbitCameraExtension;
class Prefab;
} // namespace ke

namespace ked {
class ViewPrefabEditor : public IViewPrefabEditor, public ke::UiWindow {
	std::shared_ptr<IWidgetContext> context{};
	std::vector<std::pair<std::shared_ptr<ke::Prefab>, bool /*unsaved*/>> prefabs;
	std::shared_ptr<ke::Framebuffer> frameBuffer;
	int width{}, height{};
	std::shared_ptr<ke::Prefab> selectedPrefab{};
	IPresenterPrefabEditor* presenter{};
	std::shared_ptr<ke::OrbitCameraExtension> camera;
	std::shared_ptr<ke::ProgramWideShaderBuffer> programBuffer;
	bool mouseHeld{};
	float zoom{10};
	std::shared_ptr<SceneOverlayElements> overlay;
	std::shared_ptr<ke::PrefabInstance> instance;
	glm::vec3 scale{1};
	glm::vec3 translation;
	glm::vec3 rotation;
	std::shared_ptr<ke::GltfModel> gltfModel;

	explicit ViewPrefabEditor(const std::shared_ptr<IWidgetContext> &pContext);

public:
	static std::shared_ptr<ViewPrefabEditor> create(const std::shared_ptr<IWidgetContext> &pContext);

	void onUpdate(bool pVisible) override;

	[[nodiscard]] std::string getTitle() const override;

	void openView() override;

	void closeView() override;

	void customRender() override;

	void setPresenter(IPresenterPrefabEditor* pPresenter) override { presenter = pPresenter; }

	void onSizeChanged(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(int pKey, int pScancode, int pAction, int pMods) override;

private:
	void updateTabBar(std::shared_ptr<ke::Prefab> &pSelectedPrefab, int &pSelectedIndex);

	void updateElements(const std::shared_ptr<ke::Prefab> &pSelectedPrefab, size_t pPrefabIndex);
};
} // namespace ked

#endif //VIEWPREFABEDITOR_H