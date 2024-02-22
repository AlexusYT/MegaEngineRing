//
// Created by alexus on 12.01.24.
//

#ifndef SIMPLEMESH_H
#define SIMPLEMESH_H
#include "SceneObject.h"

class SimpleMesh : public SceneObject {
public:
	std::string getType() override { return "SimpleMesh"; }
};


#endif //SIMPLEMESH_H
