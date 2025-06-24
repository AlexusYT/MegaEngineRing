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
// Created by alexus on 27.02.25.
//

#include "ViewScenePreview.h"

#include <epoxy/gl.h>

#include "KwasarEngine/bounding/VolumeAabb.h"
#include "KwasarEngine/buffers/Framebuffer.h"
#include "KwasarEngine/buffers/ProgramWideShaderBuffer.h"
#include "KwasarEngine/extensions/cameras/OrbitCameraExtension.h"
#include "KwasarEngine/resources/shaders/builtin/DefaultProgram.h"
#include "IPresenterScenePreview.h"
#include "examples/libs/glfw/include/GLFW/glfw3.h"
#include "imgui_internal.h"
#include "IModelScenePreview.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/sceneEditor/SceneOverlayElements.h"

namespace ked {
ViewScenePreview::ViewScenePreview(const std::string &pName, const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool(pName), context(pContext) {}

std::shared_ptr<ViewScenePreview> ViewScenePreview::create(const std::string &pName,
														   const std::shared_ptr<IWidgetContext> &pContext) {
	return std::shared_ptr<ViewScenePreview>(new ViewScenePreview(pName, pContext));
}

void ViewScenePreview::customRender() {
	if (!framebuffer->isComplete()) return;
	if (!frameDisplayed) return;
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	framebuffer->bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, framebuffer->getWidth(), framebuffer->getHeight());

	zoom = (zoom - targetZoom) * 0.5f;
	presenter->getModel()->getEditorCamera()->propertyDistance = std::pow(-zoom, 1.5f);
	presenter->renderScene();

	if (presenter) presenter->renderGeometryBoundingVolumes();
	//selectedPrefab->getShaderProgram()->use();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//if (gltfModel) gltfModel->getDefaultScene()->render();
	framebuffer->unbind();
	glDisable(GL_DEPTH_TEST);
	frameDisplayed = false;
}

void ViewScenePreview::onUpdate(bool pVisible) {
	if (focusWindow) {
		ImGui::SetWindowFocus();
		focusWindow = false;
	}
	widgetHovered = ImGui::IsWindowHovered();
	if (!pVisible) return;
	ImDrawList* dl = ImGui::GetWindowDrawList();
	auto screenCursor = ImGui::GetCursorScreenPos();
	auto mouse = glm::vec2(ImGui::GetMousePos()) /*- glm::vec2(screenCursor)*/;
	glm::vec2 contentAvail = ImGui::GetContentRegionAvail();
	int width = static_cast<int>(contentAvail.x);
	int height = static_cast<int>(contentAvail.y);
	mousePos.x = (mouse.x - screenCursor.x - contentAvail.x / 2.0f) / (contentAvail.x * 0.25f);
	mousePos.y = (mouse.y - screenCursor.y - contentAvail.y / 2.0f) / (contentAvail.y * 0.25f);
	//mousePos = mousePos;
	if (framebuffer->getWidth() != width || framebuffer->getHeight() != height) {
		presenter->getModel()->getEditorCamera()->propertyAspect = contentAvail.x / contentAvail.y;
		framebuffer->setSize(width, height);
	}

	glm::vec2 imageStart = screenCursor;
	glm::vec2 imageEnd = imageStart + contentAvail;
	frameDisplayed = true;
	dl->AddImageRounded(framebuffer->getFrameTexture(), imageStart, imageEnd, ImVec2(0, 1), ImVec2(1, 0),
						IM_COL32_WHITE, 8);
}

void ViewScenePreview::openView() {
	context->add(this);
	framebuffer = std::make_shared<ke::Framebuffer>();
	framebuffer->initialize();
	if (presenter) presenter->init();
}

void ViewScenePreview::closeView() {
	context->remove();
	framebuffer->uninitialize();
}

void ViewScenePreview::onCursorPosChanged(double pX, double pY) {
	if (!widgetHovered) return;
	const glm::dvec2 pos{pX, pY};
	if (presenter->onCursorPosChanged(pX, pY)) return;
	auto camera = presenter->getModel()->getEditorCamera();
	if (moveKeyHeld && mouseHeld) {
		if (!lastCursorPosMove) { lastCursorPosMove = pos; }
		const glm::vec2 delta = glm::vec2(lastCursorPosMove.value() - pos) * 0.002f * zoom;
		glm::vec3 cameraDirection =
			glm::normalize(camera->propertyPosition.getValue() - camera->propertyTargetPosition.getValue());
		glm::vec3 cameraRight = glm::normalize(glm::cross(camera->propertyUp.getValue(), cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		//glm::vec3 cameraFwd = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection);
		//glm::vec3 cameraFwd1 = glm::cross(cameraDirection, cameraFwd);
		camera->propertyTargetPosition = lastTargetPos - cameraRight * delta.x + cameraUp * delta.y;
		/*glm::vec2 angle = camera->propertyAngle;
		ke::Logger::out("{} {}", angle.x, angle.y);
		const float angleYRad = glm::radians(angle.y);
		const float angleYRight = angleYRad - 3.14f / 2.0f;
		const glm::vec3 right =
			glm::normalize(glm::vec3(std::sin(angleYRight), 0.0f, std::cos(angleYRight))) * -delta.x * 0.1f;
		const glm::vec3 fwdBwd =
			glm::normalize(glm::vec3(std::sin(angleYRad), 0.0f, std::cos(angleYRad))) * -delta.y * 0.1f;
		camera->propertyTargetPosition = lastTargetPos + right + fwdBwd;*/
		//camera->propertyAngle + glm::vec2(0.6f) * glm::vec2(-delta.y, delta.x);
		//}
	} else { lastCursorPosMove.reset(); }
	if (!mouseHeld || moveKeyHeld) {
		lastCursorPos.reset();
		return;
	}

	if (lastCursorPos) {
		const glm::vec2 delta = lastCursorPos.value() - pos;

		camera->propertyAngle = camera->propertyAngle + glm::vec2(0.6f) * glm::vec2(delta.y, delta.x);
	}
	lastCursorPos = pos;
}

void ViewScenePreview::onKeyChanged(int pKey, int pScancode, int pAction, int pMods) {
	if (!widgetHovered) return;
	if (pKey == GLFW_KEY_LEFT_SHIFT || pKey == GLFW_KEY_RIGHT_SHIFT) {
		if (!mouseHeld) { if (pAction == GLFW_PRESS) { moveKeyHeld = true; } else { moveKeyHeld = false; } } else {
			moveKeyHeld = false;
		}
	}
	if (pKey == GLFW_KEY_G && pAction == GLFW_PRESS) { if (presenter) presenter->startMovingGesture(); }
	if (pKey == GLFW_KEY_ESCAPE && pAction == GLFW_PRESS) { if (presenter) presenter->cancelCurrentAction(); }
	EditorTool::onKeyChanged(pKey, pScancode, pAction, pMods);
}

void ViewScenePreview::onMouseButton(int pButton, int pAction, int /*pMods*/) {
	//if (!widgetHovered) return;
	mouseHeld = widgetHovered && pButton == GLFW_MOUSE_BUTTON_MIDDLE && pAction == GLFW_PRESS;

	auto camera = presenter->getModel()->getEditorCamera();
	if (moveKeyHeld) { lastTargetPos = camera->propertyTargetPosition; }
	if (!widgetHovered) return;
	if (pButton == GLFW_MOUSE_BUTTON_LEFT && pAction == GLFW_RELEASE)
		if (presenter) presenter->onPrimaryMouseKeyPressed();
	if (pButton == GLFW_MOUSE_BUTTON_RIGHT && pAction == GLFW_RELEASE)
		if (presenter) presenter->onSecondaryMouseKeyPressed();
}

void ViewScenePreview::onMouseScroll(double /*pXOffset*/, double pYOffset) {
	if (!widgetHovered) return;
	targetZoom -= static_cast<float>(pYOffset) * 0.2f;
	//if (targetZoom < 0.4f) targetZoom = 0.4f;
}

bool ViewScenePreview::isRotate() { return rotate; }

void ViewScenePreview::focusOnThis() { focusWindow = true; }
} // namespace ked
