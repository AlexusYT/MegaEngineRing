//
// Created by alexus on 12.02.24.
//

#ifndef BASICRENDEROBJECT_H
#define BASICRENDEROBJECT_H
#include "SceneObject.h"

namespace mer::sdk::main {
class BasicRenderObject : public SceneObject {
public:
	uint32_t vbo;

private:
	void fillResourceRequests(const std::shared_ptr<ResourceRequests> &pRequests) override;

	sdk::utils::ReportMessagePtr init() override;

	void render() override;
};
} // namespace mer::sdk::main


#endif //BASICRENDEROBJECT_H
