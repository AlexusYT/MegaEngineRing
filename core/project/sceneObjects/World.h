//
// Created by alexus on 12.01.24.
//

#ifndef SCENE_H
#define SCENE_H
#include "SceneObject.h"

class World : public SceneObject {
public:
	std::string getType() override { return "World"; }
};


#endif //SCENE_H
