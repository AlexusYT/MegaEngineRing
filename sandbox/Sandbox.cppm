module;

#include <iostream>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <KwasarEngine/buffers/Framebuffer.h>
#include <KwasarEngine/extensions/MeshExtension.h>
#include <KwasarEngine/gltf/Node.h>
#include <KwasarEngine/meshes/BlockCubeMesh.h>
#include <KwasarEngine/scene/Scene3D.h>
#include <KwasarEngine/utils/Logger.h>
#include "KwasarEngine/context/Application.h"
#include "KwasarEngine/context/Window.h"

export module Sandbox;

void send_message(const nlohmann::json &pJson) {
	std::cout << std::format("@JSON_BEGIN@{}@JSON_END@", pJson.dump(2)) << std::endl;
}

void send_error(const ke::ReportMessagePtr &pMsg) {
	nlohmann::json json;
	json["status"] = "errored";
	json["details"] = pMsg;
	std::cout << std::format("@JSON_BEGIN@{}@JSON_END@", json.dump(2)) << std::endl;
	ke::Logger::error(pMsg);
}

class SandboxWindow : public ke::Window {
	std::shared_ptr<ke::Framebuffer> framebuffer;
	int shm_fd{};
	const char* shm_name = "/my_shm";
	size_t previousFrameSize{};
	void* framePtr{};
	std::vector<uint8_t> pixels;

public:
	SandboxWindow() : framebuffer(std::make_shared<ke::Framebuffer>()) {}

protected:
	ke::ReportMessagePtr onInitialize() override {
		if (auto msg = framebuffer->initialize()) {
			send_error(msg);
			return msg;
		}
		int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
		if (shm_fd == -1) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Failed to start sandbox process");
			msg->setMessage("Failed to create a shared memory region to store frames");
			msg->addInfoLine("Shared memory region name: {}", shm_name);
			send_error(msg);
			return msg;
		}

		previousFrameSize = getWidth() * getHeight() * 4;
		if (ftruncate(shm_fd, previousFrameSize) == -1) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Failed to start sandbox process");
			msg->setMessage("Failed to set size of a shared memory region");
			msg->addInfoLine("Shared memory region name: {}", shm_name);
			msg->addInfoLine("Frame size: {}x{}", getWidth(), getHeight());
			msg->addInfoLine("Memory size: {}", previousFrameSize);
			send_error(msg);
			close(shm_fd);
			return msg;
		}

		framePtr = mmap(0, previousFrameSize, PROT_WRITE, MAP_SHARED, shm_fd, 0);
		if (framePtr == MAP_FAILED) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Failed to start sandbox process");
			msg->setMessage("Failed to map shared memory to sandbox's process memory");
			msg->addInfoLine("Shared memory region name: {}", shm_name);
			msg->addInfoLine("Frame size: {}x{}", getWidth(), getHeight());
			msg->addInfoLine("Memory size: {}", previousFrameSize);
			send_error(msg);
			close(shm_fd);
			return msg;
		}
		nlohmann::json json = {
			{"name", shm_name},
			{"status", "started"}
		};
		send_message(json);
		return nullptr;
	}

	void onUninitialize() override {
		framebuffer->uninitialize();

		munmap(framePtr, previousFrameSize);
		shm_unlink(shm_name);
		nlohmann::json json = {
			{"status", "stopped"}
		};
		send_message(json);
	}

public:
	void beforeDraw() override {
		if (!framebuffer->isComplete()) return;
		framebuffer->bind();
	}

	void afterDraw() override {
		if (!framebuffer->isComplete()) return;
		framebuffer->unbind();

		framebuffer->readPixels(pixels);
		memcpy(framePtr, pixels.data(), pixels.size());
	}

private:
	void onSizeChanged(int pWidth, int pHeight) override {
		framebuffer->setSize(pWidth, pHeight);
		nlohmann::json json = {
			{"status", "resized"},
			{"width", getWidth()},
			{"height", getHeight()},
			{"size", previousFrameSize}
		};
		send_message(json);
	}
};


std::atomic_bool exit_request;

export int run(int pArgc, char* pArgv[]) {
	auto application = ke::Application::create();
	if (auto msg = application->initEngine()) {
		ke::Logger::error(msg);
		return 1;
	}
	auto window = std::make_shared<SandboxWindow>();
	std::jthread thread([window](const std::stop_token &pToken) {
		while (!pToken.stop_requested()) {
			std::string word;
			std::cin >> word;
			if (word == "exit") {
				exit_request = true;
				window->setCloseRequest(true);
				break;
			}
		}
	});
	thread.detach();
	auto scene = ke::Scene3D::create();
	auto mesh = ke::BlockCubeMesh::create();
	scene->addMesh(mesh);
	auto node = ke::Node::create("Test");
	auto meshExt = ke::MeshExtension::create();
	meshExt->mesh = mesh;
	node->addExtension(meshExt);

	scene->addNode(nullptr, node);
	window->addScene(scene);
	application->registerWindow(window);
	window->setVisible(false);
	return application->runMainLoop(pArgc, pArgv);
}
