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
// Created by alexus on 06.12.23.
//

#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

#include <EngineSDK/main/IWindow.h>
#include <EngineSDK/main/context/IContext.h>
#include <graphic/EngineContext.h>

#include "CameraFwd.h"
#include "OpenGLFwd.h"

//#include <graphic/ProgramWideSettingsFwd.h>
//#include "Render/opengl/shaders/ShaderProgram.h"
//#include "graphic/renderers/SceneMeshFwd.h"

class OpenGL final : public mer::sdk::main::IWindow, public mer::editor::graphics::EngineContext {
	friend Camera;
	glm::ivec2 size{};
	CameraUPtr camera;
	//ProgramWideSettingsUPtr programWideSettings{};
	glm::mat4 projectionMatrix;
	//rel::opengl::ShaderProgram* shader;
	//SceneMeshUPtr mesh;

	explicit OpenGL(Gtk::GLArea* pArea);


public:
	static OpenGLUPtr create(Gtk::GLArea* pArea) { return OpenGLUPtr(new (std::nothrow) OpenGL(pArea)); }

public:
	void onRender(double pDeltaTime) override;

	glm::ivec2 getWindowSize() override { return {}; }

	~OpenGL();

	mer::sdk::utils::ReportMessagePtr init() override;

	void swapBuffers() const override {}

	void onRealize();

	void onResize(int width, int heigth);

	void onUnrealize();

	//[[nodiscard]] const ProgramWideSettings* getProgramWideSettings() const { return programWideSettings.get(); }

private:
	void updateViewProjMatrix();

	void updateProjectMatrix();
};


#endif //OPENGLRENDER_H
