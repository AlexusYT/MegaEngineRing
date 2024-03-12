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

#include "Window.h"

#include <EngineUtils/utils/ReportMessage.h>
#include <epoxy/gl.h>

#include "EngineUtils/utils/Logger.h"
#include "GLFW/glfw3.h"

namespace mer::sdk::main {
Window::~Window() {
	if (native) glfwDestroyWindow(native);
}

void Window::show() {
	if (!native) {
		native = glfwCreateWindow(width, height, title.c_str(), nullptr, sharedWindow ? sharedWindow->native : nullptr);
		glfwDefaultWindowHints();
		glfwSetWindowUserPointer(native, this);
		glfwSetWindowSizeCallback(native, [](GLFWwindow* pWindow, const int pWidth, const int pHeight) {
			const auto win = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
			win->width = pWidth;
			win->height = pHeight;
			win->onSizeChanged(pWidth, pHeight);
		});
		glfwSetCursorPosCallback(native, [](GLFWwindow* pWindow, const double pX, const double pY) {
			const auto win = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
			win->onCursorPosChanged(pX, pY);
		});
		glfwSetKeyCallback(
			native, [](GLFWwindow* pWindow, const int pKey, const int pScancode, const int pAction, const int pMods) {
				const auto win = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
				win->onKeyChanged(pKey, pScancode, pAction, pMods);
			});

		makeCurrent();
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(
			[](GLenum pSource, GLenum pType, GLuint pId, GLenum pSeverity, GLsizei pLength, const GLchar* pMessage,
			   const void* /*userParam*/) {
				auto msg = utils::ReportMessage::create();
				msg->setTitle("OpenGL error");
				msg->setMessage("OpenGL error");
				std::string sourceStr;
				switch (pSource) {
					case GL_DEBUG_SOURCE_API: sourceStr = "Calls to the OpenGL API (GL_DEBUG_SOURCE_API)"; break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
						sourceStr = "Calls to a window-system API (GL_DEBUG_SOURCE_WINDOW_SYSTEM)";
						break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER:
						sourceStr = "A compiler for a shading language (GL_DEBUG_SOURCE_SHADER_COMPILER)";
						break;
					case GL_DEBUG_SOURCE_THIRD_PARTY:
						sourceStr = "An application associated with OpenGL (GL_DEBUG_SOURCE_THIRD_PARTY)";
						break;
					case GL_DEBUG_SOURCE_APPLICATION:
						sourceStr = "Generated by the user of this application (GL_DEBUG_SOURCE_APPLICATION)";
						break;
					case GL_DEBUG_SOURCE_OTHER: sourceStr = "Some other source (GL_DEBUG_SOURCE_OTHER)"; break;
					default: sourceStr = "Unknown";
				}

				msg->addInfoLine("Error source: {}: {}", sourceStr, pSource);
				std::string typeString;
				switch (pType) {
					case GL_DEBUG_TYPE_ERROR: typeString = "An error, typically from the API ()"; break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
						typeString =
							"Some behavior marked deprecated has been used (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)";
						break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
						typeString = "Something has invoked undefined behavior (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)";
						break;
					case GL_DEBUG_TYPE_PORTABILITY:
						typeString =
							"Some functionality the user relies upon is not portable (GL_DEBUG_TYPE_PORTABILITY)";
						break;
					case GL_DEBUG_TYPE_PERFORMANCE:
						typeString = "Code has triggered possible performance issues (GL_DEBUG_TYPE_PERFORMANCE)";
						break;
					case GL_DEBUG_TYPE_MARKER: typeString = "Command stream annotation (GL_DEBUG_TYPE_MARKER)"; break;
					case GL_DEBUG_TYPE_PUSH_GROUP: typeString = "Group pushing (GL_DEBUG_TYPE_PUSH_GROUP)"; break;
					case GL_DEBUG_TYPE_POP_GROUP: typeString = "Group popping (GL_DEBUG_TYPE_POP_GROUP)"; break;
					case GL_DEBUG_TYPE_OTHER: typeString = "Some other type (GL_DEBUG_TYPE_OTHER)"; break;
					default: typeString = "Unknown";
				}
				msg->addInfoLine("Error type: {}: {}", typeString, pType);
				msg->addInfoLine("Error id: {}", pId);
				std::string severityStr;
				switch (pSeverity) {
					case GL_DEBUG_SEVERITY_HIGH:
						severityStr = "All OpenGL Errors, shader compilation/linking errors, or highly-dangerous "
									  "undefined behavior (GL_DEBUG_SEVERITY_HIGH)";
						break;
					case GL_DEBUG_SEVERITY_MEDIUM:
						severityStr = "Major performance warnings, shader compilation/linking warnings, or the use of "
									  "deprecated functionality (GL_DEBUG_SEVERITY_MEDIUM)";
						break;
					case GL_DEBUG_SEVERITY_LOW:
						severityStr = "Redundant state change performance warning, or unimportant undefined behavior "
									  "(GL_DEBUG_SEVERITY_LOW)";
						break;
					case GL_DEBUG_SEVERITY_NOTIFICATION:
						severityStr =
							"Anything that isn't an error or performance issue (GL_DEBUG_SEVERITY_NOTIFICATION)";
						break;
					default: severityStr = "Unknown";
				}
				msg->addInfoLine("Error severity: {}: {}", severityStr, pSeverity);
				msg->addInfoLine("Error message: {}",
								 std::string(pMessage, static_cast<std::string::size_type>(pLength)));
				utils::Logger::error(msg);
			},
			nullptr);
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

void Window::onSizeChanged(int /*pWidth*/, int /*pHeight*/) {}

void Window::onCursorPosChanged(double /*pX*/, double /*pY*/) {}

void Window::onKeyChanged(int /*pKey*/, int /*pScancode*/, int /*pAction*/, int /*pMods*/) {}

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