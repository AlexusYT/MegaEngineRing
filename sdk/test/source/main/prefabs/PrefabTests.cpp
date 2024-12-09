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
// Created by alexus on 27.11.24.
//

#include <GLFW/glfw3.h>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EngineSDK/main/context/Window.h"
#include "EngineSDK/main/prefabs/IPrefabInstance.h"
#include "EngineSDK/main/prefabs/Prefab.h"
#include "EngineSDK/main/prefabs/PrefabInstance.h"
#include "EngineSDK/main/prefabs/elements/MeshPrefabElement.h"
#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/models/Model3DLoader.h"
#include "EngineSDK/main/resources/models/Model3DResource.h"

using namespace mer::sdk::main;
using ::testing::_;

class LoadExecutorStub : public IResourceLoadExecutor {
	std::shared_ptr<ILoadedResources> resources{};

public:
	std::shared_ptr<ResourceLoadResult> loadResourceSync(const std::string &pResourceUri) override {
		auto result = ResourceLoadResult::create();
		result->setError(nullptr);
		result->setResource(nullptr);
		result->setRequestedUri(pResourceUri);
		result->setState(ResourceLoadResult::State::READY);
		return result;
	}

	void loadResourceAsync(const std::string &pResourceUri,
						   const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> &pResult)> &pSlot) override {
		auto result = ResourceLoadResult::create();
		result->setError(nullptr);
		result->setResource(nullptr);
		result->setRequestedUri(pResourceUri);
		result->setState(ResourceLoadResult::State::READY);
		pSlot(result);
	}

	[[nodiscard]] IApplication* getApplication() const override { return nullptr; }

	void setApplication(IApplication* /*pApplication*/) override {}

	const std::shared_ptr<ILoadedResources> &getResources() override { return resources; }
};

class HiddenWindow : public mer::sdk::main::Window {

public:
	HiddenWindow() = default;
};

class PrefabsFixture : public testing::Test {
public:
	inline static std::shared_ptr<Model3DResource> model;
	inline static std::shared_ptr<HiddenWindow> window;

	static void SetUpTestSuite() {

		glfwInit();
		glfwSetErrorCallback([](int /*pErrorCode*/, const char* pDescription) { std::cout << pDescription << "\n"; });
		window = std::make_shared<HiddenWindow>();
		window->setVisible(false);
		window->show();
		window->getContext()->makeCurrent();
		LoadExecutorStub executor;
		std::shared_ptr<std::istream> stream = std::make_shared<std::ifstream>("main/prefabs/Pistol.enmodel");
		Model3DLoader loader;
		auto resource = loader.createResource();
		loader.preload(&executor, stream, resource);
		loader.load(&executor, stream, resource);
		model = std::dynamic_pointer_cast<Model3DResource>(resource);
	}

	static void TearDownTestSuite() {
		model.reset();
		window.reset();
	}
};

TEST_F(PrefabsFixture, RemoveElement) {

	auto elementToRemove = MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.002"));
	auto prefab1 = Prefab::create();
	prefab1->addElement(MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.001")));
	prefab1->addElement(elementToRemove);
	prefab1->addElement(MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.003")));

	prefab1->removeElement(elementToRemove);
	auto prefab2 = Prefab::create();
	prefab2->addElement(MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.001")));
	prefab2->addElement(MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.003")));

	auto pref1Elements = prefab1->getElements();
	auto pref2Elements = prefab2->getElements();
	auto element1 = pref1Elements.at("CustomPistol9mm.003");
	auto element2 = pref2Elements.at("CustomPistol9mm.003");
	EXPECT_EQ(pref1Elements.size(), pref2Elements.size());
	EXPECT_EQ(element1->getDataStart(), element2->getDataStart());
	EXPECT_EQ(element1->getIndicesStart(), element2->getIndicesStart());
	EXPECT_EQ(prefab1->getData(), prefab2->getData());
	EXPECT_EQ(prefab1->getIndices(), prefab2->getIndices());
}

class PrefabInstanceMock : public PrefabInstance {
public:
	~PrefabInstanceMock() override = default;
	MOCK_METHOD(std::shared_ptr<PrefabElementInstance>, getElement, (const UUID &pUuid), (const, override));

	MOCK_METHOD(void, addElement, (const UUID &pUuid, const std::shared_ptr<PrefabElementInstance> &pInstance),
				(override));

	MOCK_METHOD(void, removeElement, (const UUID &pUuid), (override));
};

TEST_F(PrefabsFixture, AddPrefabInstance) {
	auto prefab = Prefab::create();

	auto element1 = MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.001"));
	auto element2 = MeshPrefabElement::create(model->getModelObject("CustomPistol9mm.003"));
	prefab->addElement(element1);
	prefab->addElement(element2);
	auto mockPrefabInstance = std::make_shared<PrefabInstanceMock>();
	EXPECT_CALL(*mockPrefabInstance, addElement(element2->getUuid(), _)).Times(1);
	EXPECT_CALL(*mockPrefabInstance, addElement(element1->getUuid(), _)).Times(1);
	prefab->addInstance(mockPrefabInstance);
}