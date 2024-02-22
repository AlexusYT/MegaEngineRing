//
// Created by Alexus on 25.01.2023.
//

#ifndef MAIN_DOOR_H
#define MAIN_DOOR_H


#include "DoorFwd.h"
#include "graphic/renderers/SceneMesh.h"

class Door : public SceneMesh {

private:
protected:
	Door() = default;


private:
	//friend void from_json(const nlohmann::json &j, Door &t);
};


#endif //MAIN_DOOR_H
