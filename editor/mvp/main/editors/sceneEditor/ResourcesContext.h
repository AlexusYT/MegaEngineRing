//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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

#ifndef RESOURCESCONTEXT_H
#define RESOURCESCONTEXT_H
#include <condition_variable>
#include <thread>

#include "EngineSDK/main/context/Application.h"
#include "EngineSDK/main/resources/IResourceLoadExecutor.h"

namespace mer::sdk::main {
class IResourceLoader;
class ILoadedResources;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class ResourcesContext : public sdk::main::IResourceLoadExecutor {
	enum class RequestType { PRELOAD, LOAD };

	struct Request {
		std::filesystem::path uri;
		RequestType type;
		sigc::signal<void(const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult)> callbackSignal;
	};

	std::mutex queueMutex;
	std::unordered_map<std::filesystem::path, std::shared_ptr<Request>> queue;
	std::mutex waitMutex;
	std::shared_ptr<sdk::main::ILoadedResources> resources;
	std::condition_variable cv;

	std::shared_ptr<Gdk::GLContext> sharedContext;
	std::list<std::jthread> threads;
	sdk::main::IApplication* application{};
	std::mutex headersLengthMutex;
	std::unordered_map<std::shared_ptr<sdk::main::IResource>, int64_t> headersLength;
	std::mutex processingQueueMutex;
	std::unordered_map<std::filesystem::path, std::shared_ptr<Request>> processingQueue;

public:
	explicit ResourcesContext(const std::shared_ptr<Gdk::GLContext> &pSharedContext) : sharedContext(pSharedContext) {
		for (uint32_t i = 0, maxI = std::max(std::thread::hardware_concurrency() / 2, 1u); i < maxI; i++) {
			threads.emplace_back([this](const std::stop_token &pToken) { this->loop(pToken); });
		}
	}

	~ResourcesContext() override {
		for (auto &thread: threads) { thread.request_stop(); }
		cv.notify_all();
	}

	std::shared_ptr<sdk::main::ResourceLoadResult> loadResourceSync(const std::string &pResourceUri) override;


	void preloadResources();

	void loadResourceAsync(
		const std::string &pResourceUri,
		const sigc::slot<void(const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult)> &pSlot) override;

	void loop(const std::stop_token &pToken);

	void processRequest(const std::shared_ptr<Request> &pRequest);

	[[nodiscard]] sdk::main::IApplication* getApplication() const override { return application; }

	void setApplication(sdk::main::IApplication* pApplication) override { application = pApplication; }

	void setResources(const std::shared_ptr<sdk::main::ILoadedResources> &pResources) { resources = pResources; }

	const std::shared_ptr<sdk::main::ILoadedResources> &getResources() override { return resources; }

private:
	static void callSlot(const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult,
						 const sigc::slot<void(const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult)> &pSlot);

	static std::shared_ptr<sdk::main::IResourceLoader> getLoader(const std::shared_ptr<Request> &pRequest);

	std::shared_ptr<std::istream> getResourceStream(const std::shared_ptr<Request> &pRequest) const;
};
} // namespace mer::editor::mvp


#endif //RESOURCESCONTEXT_H
