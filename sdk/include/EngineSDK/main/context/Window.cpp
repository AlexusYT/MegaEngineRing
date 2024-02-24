//
// Created by alexus on 31.01.24.
//

#ifndef EDITOR_SDK
	#include "Window.h"

	#include <EngineUtils/utils/ReportMessage.h>
	#include <GLFW/glfw3.h>

namespace mer::sdk::main {
Window::~Window() {
	if (native) glfwDestroyWindow(native);
}

void Window::show() {
	if (!native) {
		native = glfwCreateWindow(width, height, title.c_str(), nullptr, sharedWindow ? sharedWindow->native : nullptr);
		glfwDefaultWindowHints();
		glfwSetWindowUserPointer(native, this);
		glfwSetWindowSizeCallback(native, [](GLFWwindow* window, const int pWidth, const int pHeight) {
			const auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->onResize(pWidth, pHeight);
		});
	}
}

void Window::setVisible(const bool pState) const {
	if (native) {
		glfwSetWindowAttrib(native, GLFW_VISIBLE, pState);
	} else {
		glfwWindowHint(GLFW_VISIBLE, pState);
	}
}

bool Window::isVisible() const { return glfwGetWindowAttrib(native, GLFW_VISIBLE); }

void Window::setCloseRequest(const bool pState) const {
	if (native) glfwSetWindowShouldClose(native, pState);
}

sdk::utils::ReportMessagePtr Window::setContextVersion(int pMajor, int pMinor) const {
	if (native) return nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, pMajor);
	const char* errorMsg;
	if (auto errorId = glfwGetError(&errorMsg); errorId != GLFW_NO_ERROR) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to set context version");
		msg->setMessage("Function returns an error when setting major version");
		msg->addInfoLine("Error num: {}", errorId);
		msg->addInfoLine("Error message: {}", errorMsg);
		msg->addInfoLine("Requested version: {}.{}", pMajor, pMinor);
		return msg;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, pMajor);

	if (auto errorId = glfwGetError(&errorMsg); errorId != GLFW_NO_ERROR) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to set context version");
		msg->setMessage("Function returns an error when setting minor version");
		msg->addInfoLine("Error num: {}", errorId);
		msg->addInfoLine("Error message: {}", errorMsg);
		msg->addInfoLine("Requested version: {}.{}", pMajor, pMinor);
		return msg;
	}
	return nullptr;
}

bool Window::isCloseRequest() const {

	if (native) return glfwWindowShouldClose(native);
	return false;
}

inline void Window::onResize(int /*pWidth*/, int /*pHeight*/) {}

sdk::utils::ReportMessagePtr Window::init() {
	makeCurrent();
	/*glewExperimental = true;
	if (auto error = glewInit(); error != GLEW_OK) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to init glew");
		msg->setMessage("Error occurred");
		msg->addInfoLine("Error num: {}", error);
		msg->addInfoLine("Error msg: {}", reinterpret_cast<const char*>(glewGetErrorString(error)));
		return msg;
	}*/
	return nullptr;
}

void Window::makeCurrent() { glfwMakeContextCurrent(native); }

void Window::swapBuffers() const {
	glfwSwapBuffers(native);
	glfwPollEvents();
}
} // namespace mer::sdk::main
#endif