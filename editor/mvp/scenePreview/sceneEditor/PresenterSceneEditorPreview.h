//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 12.03.25.
//

#ifndef PRESENTERSCENEEDITORPREVIEW_H
#define PRESENTERSCENEEDITORPREVIEW_H
#include <thread>

#include "mvp/scenePreview/PresenterScenePreview.h"

namespace ked {
class PresenterSceneEditorPreview : public PresenterScenePreview {
	std::optional<std::jthread> simulationThread;
	pid_t sandboxPid{};
	bool sandboxRunning{};
	int sharedMemHandle{-1};
	void* sharedFrameHandle{};
	std::string sharedMemName;
	size_t sharedFrameSize{0};
	int32_t sharedFrameWidth{0};
	int32_t sharedFrameHeight{0};

	int childStdIn[2]{};
	int childStdOut[2]{};
	int childStdErr[2]{};

	PresenterSceneEditorPreview(const std::shared_ptr<IViewScenePreview> &pView,
								const std::shared_ptr<IModelScenePreview> &pModel)
		: PresenterScenePreview(pView, pModel) {}

public:
	static std::shared_ptr<PresenterSceneEditorPreview> create(const std::shared_ptr<IViewScenePreview> &pView,
															   const std::shared_ptr<IModelScenePreview> &pModel);

	std::string getTypeName() override { return "PresenterSceneEditorPreview"; }

	void onSimulationButtonClicked() override;

	inline void getFrameData(int32_t &pWidthOut, int32_t &pHeightOut, void*&pDataOut, size_t &pDataSizeOut) override;

private:
	void parseJson(const std::string &pJsonString);

	void initSharedMemory(const std::string &pName);

	void sharedFrameResized(size_t pNewFrameSize, int32_t pNewFrameWidth, int32_t pNewFrameHeight);

	void uninitSharedMemory();

	int unmapMemory();

	void reportSandboxError(const ke::ReportMessagePtr &pErrorMsg);
};
} // namespace ked

#endif //PRESENTERSCENEEDITORPREVIEW_H
