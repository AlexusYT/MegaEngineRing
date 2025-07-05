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

#include "PresenterSceneEditorPreview.h"

#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include <Globals.h>
#include <KwasarEngine/gltf/Texture.h>
#include <mvp/scenePreview/IViewScenePreview.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sys/mman.h>

#include "SandboxStatus.h"

#define CHILD_STDIN_READ childStdIn[0]
#define CHILD_STDIN_WRITE childStdIn[1]
#define CHILD_STDOUT_READ childStdOut[0]
#define CHILD_STDOUT_WRITE childStdOut[1]
#define CHILD_STDERR_READ childStdErr[0]
#define CHILD_STDERR_WRITE childStdErr[1]

namespace ked {
std::shared_ptr<PresenterSceneEditorPreview> PresenterSceneEditorPreview::create(
	const std::shared_ptr<IViewScenePreview> &pView, const std::shared_ptr<IModelScenePreview> &pModel) {
	return std::shared_ptr<PresenterSceneEditorPreview>(new PresenterSceneEditorPreview(pView, pModel));
}


void PresenterSceneEditorPreview::onSimulationButtonClicked() {
	if (sandboxRunning) {
		if (!simulationThread->request_stop()) {
			if (sandboxPid > 0)
				kill(sandboxPid, SIGKILL);
		}
		return;
	}
	simulationThread = std::jthread([this](const std::stop_token &token) {
		sandboxRunning = true;
		if (pipe(childStdIn) == -1 || pipe(childStdOut) == -1 || pipe(childStdErr) == -1) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Error when running the sandbox process");
			msg->setMessage("Failed to open pipes to sandbox process");
			reportSandboxError(msg);
			sandboxRunning = false;
			sandboxPid = 0;
			return;
		}

		const pid_t pid = fork();
		if (pid < 0) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Error when running the sandbox process");
			msg->setMessage("Failed to fork editor process to create sandbox process");
			reportSandboxError(msg);
			sandboxRunning = false;
			sandboxPid = 0;
			return;
		}
		//Child process
		if (pid == 0) {
			if (dup2(CHILD_STDOUT_WRITE, STDOUT_FILENO) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to link sandbox stdout to the pipe");
				ke::Logger::error(msg);
				exit(1);
			}
			if (dup2(CHILD_STDERR_WRITE, STDERR_FILENO) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to link sandbox stderr to the pipe");
				ke::Logger::error(msg);
				exit(1);
			}
			if (dup2(CHILD_STDIN_READ, STDIN_FILENO) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to link sandbox stdin to the pipe");
				ke::Logger::error(msg);
				exit(1);
			}

			if (close(CHILD_STDIN_WRITE) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to close sandbox stdin");
				ke::Logger::error(msg);
				exit(1);
			}
			if (close(CHILD_STDOUT_READ) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to close sandbox stdout");
				ke::Logger::error(msg);
				exit(1);
			}
			if (close(CHILD_STDERR_READ) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to close sandbox stderr");
				ke::Logger::error(msg);
				exit(1);
			}

			const auto &path = Globals::getPathToSandbox().string();
			if (execlp(path.c_str(), path.c_str(), (char*) NULL) == -1) {
				auto msg = ke::ReportMessage::create();
				msg->setTitle("Error when running the sandbox process");
				msg->setMessage("Failed to run sandbox executable");
				msg->addInfoLine("Path to executable: {}", path);
				ke::Logger::error(msg);
				exit(1);
			}

			exit(1);
		} else {
			sandboxPid = pid;
			view->sandboxStatusChanged(SandboxStatus::STARTED);
			close(CHILD_STDIN_READ);
			close(CHILD_STDOUT_WRITE);
			close(CHILD_STDERR_WRITE);
			fcntl(CHILD_STDOUT_READ, F_SETFL, fcntl(CHILD_STDOUT_READ, F_GETFL) | O_NONBLOCK);
			int status;
			while (waitpid(pid, &status, WNOHANG | WUNTRACED) != pid) {
				if (token.stop_requested()) { kill(sandboxPid, SIGTERM); }
				char buffer1[1024]{};
				if (auto bytes1 = read(CHILD_STDOUT_READ, buffer1, sizeof(buffer1)); bytes1 > 0) {
					std::string s(buffer1, static_cast<std::string::size_type>(bytes1));
					std::cout << "STDOUT: " << s << "\n";
					//language=regexp
					static std::regex regex(R"(\@JSON_BEGIN\@([\s\S]*?)\@JSON_END\@)");
					std::smatch match;
					while (std::regex_search(s, match, regex, std::regex_constants::match_any)) {
						parseJson(match.str(1));
						s = match.suffix();
					}
				} else { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }

				/*// write to stdin
				if (FD_ISSET(pipe_stdin[1], &read_fds)) {
					const char* input = "continuous input\n";
					write(pipe_stdin[1], input, strlen(input));
				}*/
			}


			ke::Logger::info("Sandbox process exited with code {}", WEXITSTATUS(status));
			close(CHILD_STDIN_WRITE);
			close(CHILD_STDOUT_READ);
			close(CHILD_STDERR_READ);
			sandboxRunning = false;
			sandboxPid = 0;
			uninitSharedMemory();
			view->sandboxStatusChanged(SandboxStatus::STOPPED);
		}
	});
	simulationThread->detach();
}

void PresenterSceneEditorPreview::getFrameData(int32_t &pWidthOut, int32_t &pHeightOut, void*&pDataOut,
											   size_t &pDataSizeOut) {
	pWidthOut = sharedFrameWidth;
	pHeightOut = sharedFrameHeight;
	pDataOut = sharedFrameHandle;
	pDataSizeOut = sharedFrameSize;
}

void PresenterSceneEditorPreview::parseJson(const std::string &pJsonString) {
	try {
		nlohmann::json json = nlohmann::json::parse(pJsonString);
		auto status = json.at("status").get<std::string>();
		if (status == "started") initSharedMemory(json.at("name").get<std::string>());
		if (status == "resized")
			sharedFrameResized(json.at("size").get<size_t>(), json.at("width").get<int32_t>(),
							   json.at("height").get<int32_t>());
		if (status == "errored") {
			auto msg = json.at("details").get<ke::ReportMessagePtr>();
			reportSandboxError(msg);
		}
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Error when running the sandbox process");
		msg->setMessage("Exception occurred when trying to communicate with the sandbox process");
		reportSandboxError(msg);
	}
}

void PresenterSceneEditorPreview::initSharedMemory(const std::string &pName) {
	if (sharedMemHandle > -1) return;

	sharedMemHandle = shm_open(pName.c_str(), O_RDWR, 0666);
	if (sharedMemHandle == -1) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Error when running the sandbox process");
		msg->setMessage("Failed to open shared memory region to get rendered frames by sandbox");
		msg->addInfoLine("Shared memory name: {}", pName);
		reportSandboxError(msg);
		return;
	}
	sharedMemName = pName.c_str();
}

void PresenterSceneEditorPreview::sharedFrameResized(const size_t pNewFrameSize, int32_t pNewFrameWidth,
													 int32_t pNewFrameHeight) {
	if (sharedMemHandle <= -1) return;
	if (sharedFrameHandle)
		if (unmapMemory() == -1)
			return;


	sharedFrameHandle = mmap(nullptr, pNewFrameSize, PROT_READ, MAP_SHARED, sharedMemHandle, 0);
	if (sharedFrameHandle == MAP_FAILED) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Error when running the sandbox process");
		msg->setMessage("Failed to map shared memory to editor's process memory when resizing frame");
		msg->addInfoLine("Shared memory name: {}", sharedMemName);
		msg->addInfoLine("Shared memory handle: {}", sharedMemHandle);
		msg->addInfoLine("Requested memory size: {}", pNewFrameSize);
		msg->addInfoLine("Requested frame size: {}x{}", pNewFrameWidth, pNewFrameHeight);
		reportSandboxError(msg);
		shm_unlink(sharedMemName.c_str());
		return;
	}
	sharedFrameSize = pNewFrameSize;
	sharedFrameWidth = pNewFrameWidth;
	sharedFrameHeight = pNewFrameHeight;
	view->sandboxStatusChanged(SandboxStatus::RUNNING);
}

void PresenterSceneEditorPreview::uninitSharedMemory() {
	if (sharedFrameHandle)
		unmapMemory();

	if (sharedMemHandle > -1) {
		shm_unlink(sharedMemName.c_str());
		sharedMemHandle = -1;
	}
}

int PresenterSceneEditorPreview::unmapMemory() {
	if (munmap(sharedFrameHandle, sharedFrameSize) == -1) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Error when running the sandbox process");
		msg->setMessage("Failed to unmap old shared memory from editor's process memory when resizing frame");
		msg->addInfoLine("Shared memory name: {}", sharedMemName);
		msg->addInfoLine("Shared memory handle: {}", sharedMemHandle);
		msg->addInfoLine("Shared frame handle: {}", sharedFrameHandle);
		reportSandboxError(msg);
		sharedFrameHandle = nullptr;
		return -1;
	}
	return 0;
}

void PresenterSceneEditorPreview::reportSandboxError(const ke::ReportMessagePtr &pErrorMsg) {
	view->sandboxStatusChanged(SandboxStatus::FAILED);
	view->reportError(pErrorMsg);
}
} // namespace ked
