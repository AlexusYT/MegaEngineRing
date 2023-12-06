//
// Created by alexus on 06.12.23.
//

#include "OpenGLRender.h"

#include <GLFW/glfw3.h>

void OpenGLRender::onRealize() {
	area->make_current();
	if (!glfwInit()) exit(EXIT_FAILURE);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void OpenGLRender::onResize(int width, int heigth) {}

bool OpenGLRender::onRender(const Glib::RefPtr<Gdk::GLContext> &context) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return false;
}

void OpenGLRender::updateViewProjMatrix() {}