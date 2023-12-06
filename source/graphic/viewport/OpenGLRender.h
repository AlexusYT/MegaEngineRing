//
// Created by alexus on 06.12.23.
//

#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H

#include <iostream>

#include "Camera.h"
#include "CameraFwd.h"
#include "OpenGLRenderFwd.h"

class OpenGLRender {
	Gtk::GLArea* area;
	CameraUPtr camera;

	explicit OpenGLRender(Gtk::GLArea* gl_area) : area(gl_area) {}

public:
	static OpenGLRenderUPtr create(Gtk::GLArea* area) {
		return OpenGLRenderUPtr(new (std::nothrow) OpenGLRender(area));
	}

	bool init() {

		camera = Camera::create(this);
		if (!camera) {
			std::cerr << "Failed to create camera\n";
			return false;
		}

		area->signal_render().connect(sigc::mem_fun(*this, &OpenGLRender::onRender), true);
		area->signal_realize().connect(sigc::mem_fun(*this, &OpenGLRender::onRealize), true);
		area->signal_unrealize().connect(sigc::mem_fun(*this, &OpenGLRender::onUnrealize), true);
		area->signal_resize().connect(sigc::mem_fun(*this, &OpenGLRender::onResize), true);
		return true;
	}

	void onRealize();

	void onResize(int width, int heigth);

	void onUnrealize() {}

	bool onRender(const Glib::RefPtr<Gdk::GLContext> &context);

private:
	void updateViewProjMatrix();
};


#endif //OPENGLRENDER_H
