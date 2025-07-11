//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 02.02.24.
//

#ifndef RESOURCESWINDOW_H
#define RESOURCESWINDOW_H

#ifndef EDITOR_SDK
#include <thread>

#include "KwasarEngine/resources/IResourceLoadExecutor.h"
#include "KwasarEngine/resources/Resources.h"
#include "KwasarEngine/scene/IScene.h"
#include "Window.h"

namespace ke {
class ILoadedResources;
} // namespace ke

namespace ke {
class ResourcesWindow : public Window, public IResourceLoadExecutor {
	std::list<std::pair<std::string, LoadingFinishedSlot>> queue;
	std::mutex queueMutex;
	std::mutex waitMutex;

	std::shared_ptr<ILoadedResources> resources;
	std::jthread thread;
	IApplication* application;

protected:
	ResourcesWindow();

public:
	static auto create() { return std::shared_ptr<ResourcesWindow>(new(std::nothrow) ResourcesWindow()); }

	std::shared_ptr<ResourceLoadResult> loadResourceSync(const std::string &pResourceUri) override;

	void loadResourceAsync(const std::string &pResourceUri,
						   const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> & pResult)> &pSlot) override;

	void requestStopThread();

	[[nodiscard]] IApplication* getApplication() const override { return application; }

	void setApplication(IApplication* pApplication) override { application = pApplication; }

	const std::shared_ptr<ILoadedResources> &getResources() override { return resources; }

	static void callSlot(const std::shared_ptr<ke::ResourceLoadResult> &pResult,
						 const sigc::slot<void(const std::shared_ptr<ke::ResourceLoadResult> & pResult)> &pSlot);

private:
	void resourceLoop(const std::stop_token &pToken);
};
} // namespace ke


#endif
#endif //RESOURCESWINDOW_H