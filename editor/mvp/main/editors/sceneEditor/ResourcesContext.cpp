//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 03.09.24.
//

#include "ResourcesContext.h"

#include <epoxy/gl.h>
#include <future>

#include "EngineSDK/main/IApplication.h"
#include "EngineSDK/main/resources/IResourceBundle.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/ResourceLoader.h"
#include "EngineSDK/main/resources/ResourceLoaders.h"
#include "Globals.h"

namespace mer::editor::mvp {
std::shared_ptr<sdk::main::ResourceLoadResult> ResourcesContext::loadResourceSync(const std::string &pResourceUri) {


	std::promise<const std::shared_ptr<sdk::main::ResourceLoadResult>> promise;

	loadResourceAsync(pResourceUri, [&promise](const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult) {
		if (!pResult->isLoaded()) promise.set_value(pResult);
	});
	auto future = promise.get_future();
	future.wait();
	return future.get();
}

void ResourcesContext::resourceLoop(const std::stop_token &pToken) {

	sharedContext->make_current();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);
	glDebugMessageCallback(
		[](GLenum pSource, GLenum pType, GLuint pId, GLenum pSeverity, GLsizei pLength, const GLchar* pMessage,
		   const void* /*pUserParam*/) {
			if (pSeverity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
			if (pId == 131076) return;
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("OpenGL error");
			msg->setMessage(std::string(pMessage, static_cast<std::string::size_type>(pLength)));
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
					typeString = "Some behavior marked deprecated has been used (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)";
					break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
					typeString = "Something has invoked undefined behavior (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)";
					break;
				case GL_DEBUG_TYPE_PORTABILITY:
					typeString = "Some functionality the user relies upon is not portable (GL_DEBUG_TYPE_PORTABILITY)";
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
					severityStr = "Anything that isn't an error or performance issue (GL_DEBUG_SEVERITY_NOTIFICATION)";
					break;
				default: severityStr = "Unknown";
			}
			msg->addInfoLine("Error severity: {}: {}", severityStr, pSeverity);
			sdk::utils::Logger::error(msg);
		},
		nullptr);
	while (!pToken.stop_requested()) {
		std::unique_lock lck(waitMutex);
		cv.wait(lck, [this, pToken]() { return !queue.empty() || pToken.stop_requested(); });
		for (auto &[uri, slot]: queue) {
			sharedContext->make_current();

			auto result = sdk::main::ResourceLoadResult::create();
			if (std::shared_ptr<sdk::main::IResource> resource = resources->getResource(uri)) {
				result->setResource(resource);
				result->setState(sdk::main::ResourceLoadResult::State::READY);
				result->setRequestedUri(uri);
				callSlot(result, slot);
				continue;
			}

			std::filesystem::path uriPath = uri;
			try {

				if (!uriPath.has_extension()) {
					auto msg = sdk::utils::ReportMessage::create();
					msg->setTitle("Unable to load resource");
					msg->setMessage("No resource extension in uri");
					msg->addInfoLine("Resource URI: {}", uri);
					result->setError(msg);
					result->setRequestedUri(uri);
					callSlot(result, slot);
					continue;
				}

				auto loader = sdk::main::ResourceLoaders::getInstance()->getLoader(uriPath.extension());
				if (!loader) {
					auto msg = sdk::utils::ReportMessage::create();
					msg->setTitle("Unable to load resource");
					msg->setMessage("No loader registered that can load such resource");
					msg->addInfoLine("Resource URI: {}", uri);
					result->setError(msg);
					result->setRequestedUri(uri);
					callSlot(result, slot);
					continue;
				}

				std::shared_ptr<std::istream> stream;
				if (uri.starts_with("_builtin_")) {
					if (!Gio::Resource::get_file_exists_global_nothrow("/" + uri)) {
						auto msg = sdk::utils::ReportMessage::create();
						msg->setTitle("Failed to get resource stream");
						msg->setMessage("Builtin resource not found");
						msg->addInfoLine("Resource URI to search: {}", uri);
						result->setError(msg);
						result->setRequestedUri(uri);
						callSlot(result, slot);
					}
					auto resourceBytes = Gio::Resource::lookup_data_global("/" + uri);
					auto ss = std::make_shared<std::stringstream>();
					unsigned long size = 0;
					auto buf = static_cast<const char*>(resourceBytes->get_data(size));
					ss->write(buf, static_cast<std::streamsize>(size));
					stream = ss;
				} else {

					if (auto msg = application->getResourceBundle()->getResourceStream(uriPath, stream)) {
						msg->setTitle("Unable to load resource");
						result->setError(msg);
						result->setRequestedUri(uri);
						callSlot(result, slot);
						continue;
					}
				}
				std::shared_ptr<sdk::main::IResource> resource;
				auto startTime = std::chrono::steady_clock::now();
				if (auto msg = loader->load(this, stream, resource)) {
					msg->setTitle("Unable to load resource");
					result->setError(msg);
					result->setRequestedUri(uri);
					callSlot(result, slot);
					continue;
				}
				auto endTime = std::chrono::steady_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
				if (duration.count() > 500) {
					auto msg = sdk::utils::ReportMessage::create();
					msg->setTitle("Too long duration");
					msg->setMessage("Resource loading took to long");
					msg->addInfoLine("Resource URI: {}", uri);
					msg->addInfoLine("Duration: {}ms", duration.count());
					sdk::utils::Logger::warn(msg->getReport(false));
				}
				result->setResource(resource);
				result->setRequestedUri(uri);
				result->setState(sdk::main::ResourceLoadResult::State::LOADED);
				callSlot(result, slot);

				if (auto msg = loader->init(this, resource)) {
					msg->setTitle("Unable to load resource");
					result->setError(msg);
					result->setRequestedUri(uri);
					callSlot(result, slot);
					continue;
				}
				resources->addResource(uri, resource);
				result->setState(sdk::main::ResourceLoadResult::State::READY);
				callSlot(result, slot);
			} catch (...) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Unable to load resource");
				msg->setMessage("Exception thrown while executing request");
				msg->addInfoLine("Resource URI: {}", uri);
				result->setError(msg);
				result->setRequestedUri(uri);
				callSlot(result, slot);
			}
		}
		std::lock_guard lock(queueMutex);
		queue.clear();
	}
}

void ResourcesContext::callSlot(
	const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult,
	const sigc::slot<void(const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult)> &pSlot) {
	try {
		pSlot(pResult);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to send loading result to the callback");
		msg->setMessage("Exception thrown in callback");
		msg->addInfoLine("Result state: {}", pResult->getStateStr());
		msg->addInfoLine("Requested resource URI: {}", pResult->getRequestedUri());
		if (pResult->isErrored()) {
			msg->addInfoLine("Result error reported earlier");
			sdk::utils::Logger::error(pResult->getError());
		}
		sdk::utils::Logger::error(msg);
	}
}
} // namespace mer::editor::mvp