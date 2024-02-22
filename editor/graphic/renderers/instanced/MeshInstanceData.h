//
// Created by alexus on 29.07.23.
//

#ifndef MAIN_MESHINSTANCEDATA_H
#define MAIN_MESHINSTANCEDATA_H

#include <graphic/renderers/MeshMaterial.h>

struct MeshInstanceData {
	glm::mat4 modelViewMatrix{1};
	MeshMaterial material;
	uint32_t type{};
	bool lighted = true;
	[[maybe_unused]] uint8_t align1{};
	[[maybe_unused]] uint16_t align2{};
	[[maybe_unused]] uint32_t align3{};
	[[maybe_unused]] uint32_t align4{};
};


#endif //MAIN_MESHINSTANCEDATA_H
