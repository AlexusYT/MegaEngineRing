//
// Created by alexus on 14.07.23.
//

#ifndef MAIN_MESHMATERIAL_H
#define MAIN_MESHMATERIAL_H

struct MeshMaterial {
	glm::vec4 ambient{0, 0, 0, 1};
	glm::vec4 diffuse{0, 0, 0, 1};
	glm::vec4 specular{0, 0, 0, 1};
	float shininess{};

	[[maybe_unused]] uint32_t align1{};
	[[maybe_unused]] uint32_t align2{};
	[[maybe_unused]] uint32_t align3{};
};


#endif //MAIN_MESHMATERIAL_H
