//
// Created by alexus on 06.12.23.
//

#include "OpenGL.h"

//#include <Render/opengl/shaders/Shader.h>

#include <epoxy/gl.h>
#include <graphic/ProgramWideSettings.h>

#include "Camera.h"
//#include "Render/main/RenderEngine.h"
//#include "Render/main/windows/WindowManager.h"
//#include "Render/opengl/shaders/ShaderProgram.h"
#include <graphic/EngineContext.h>

#include "graphic/renderers/SceneMesh.h"
#include "graphic/shaders/ShadersList.h"

OpenGL::OpenGL(Gtk::GLArea* pArea) : EngineContext(pArea) {

	getArea()->set_required_version(4, 0);
	getArea()->signal_render().connect(
		[this](const Glib::RefPtr<Gdk::GLContext> & /*context*/) {
			onRender(0);
			return true;
		},
		true);
	getArea()->signal_realize().connect(sigc::mem_fun(*this, &OpenGL::onRealize), true);
	getArea()->signal_unrealize().connect(sigc::mem_fun(*this, &OpenGL::onUnrealize), false);
	getArea()->signal_resize().connect(sigc::mem_fun(*this, &OpenGL::onResize), true);
}

OpenGL::~OpenGL() {}

n::engine::utils::ReportMessagePtr OpenGL::init() {

	camera = Camera::create(this);
	if (!camera) {
		std::cerr << "Failed to create camera\n";
		return nullptr;
	}
	return nullptr;
}

void OpenGL::onRealize() {
	makeCurrent();

	/*programWideSettings = ProgramWideSettings::create();
	if (!programWideSettings) {
		std::cerr << "Failed to create ProgramWideSettings\n";
		return;
	}
	if (!programWideSettings->init()) {
		std::cerr << "Failed to init ProgramWideSettings\n";
		return;
	}
	ShadersList::addVertexShader("Basic/Basic.vert", "Basic");
	ShadersList::addFragmentShader("Basic/Basic.frag", "Basic");
	ShadersList::addShaderProgram("Basic", "Basic", "Basic");*/
	//shader = ShadersList::getProgram("Basic");
	//shader->bindSSBO(programWideSettings->getSSBO(), "ProgramWideSettings", 2);


	//mesh = SceneMesh::create();
	//mesh->init();

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void OpenGL::onResize(const int width, const int heigth) {
	size.x = width;
	size.y = heigth;
	updateProjectMatrix();
}

void OpenGL::onUnrealize() { /*ShadersList::clear(); */
}

void OpenGL::onRender(double /*pDeltaTime*/) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//shader->use();
	//mesh->update();
}

void OpenGL::updateViewProjMatrix() {

	//if (!camera || !programWideSettings) return;
	//programWideSettings->setViewProjMatrix(projectionMatrix * camera->getViewMatrix());
}

void OpenGL::updateProjectMatrix() {
	/*if (!camera || !programWideSettings) return;
	const float windowAspect = size.y != 0 ? static_cast<float>(size.x) / static_cast<float>(size.y) : 0;
	projectionMatrix = glm::perspective(glm::radians(camera->getFov()), windowAspect, 0.1f, 400.0f);
	updateViewProjMatrix();*/
}