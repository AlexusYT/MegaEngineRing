//
// Created by alexus on 02.02.24.
//

#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Scene.h"

class TestScene : public mer::sdk::main::Scene {
public:
	using ResourceT = TestScene;

	mer::sdk::utils::ReportMessagePtr preloadScene(
		const std::shared_ptr<mer::sdk::main::ResourceRequests> &pRequests) override;
	mer::sdk::utils::ReportMessagePtr initScene() override;
};


#endif //TESTSCENE_H
