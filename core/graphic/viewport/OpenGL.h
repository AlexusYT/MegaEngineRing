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

class OpenGL final : public n::sdk::main::IWindow, public n::core::graphics::EngineContext {
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

	n::engine::utils::ReportMessagePtr init() override;

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
