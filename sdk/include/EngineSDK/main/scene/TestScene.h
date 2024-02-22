//
// Created by alexus on 02.02.24.
//

#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Scene.h"

class TestScene : public n::sdk::main::Scene {
public:
	using ResourceT = TestScene;

	n::engine::utils::ReportMessagePtr preloadScene(
		const std::shared_ptr<n::sdk::main::ResourceRequests> &pRequests) override;
	n::engine::utils::ReportMessagePtr initScene() override;
};


#endif //TESTSCENE_H
