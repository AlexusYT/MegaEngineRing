//
// Created by alexus on 28.09.24.
//

#ifndef BASICOBJECTEXTENSION_H
#define BASICOBJECTEXTENSION_H
#include <glm/vec3.hpp>

#include "Extension.h"

namespace mer::sdk::main {

class MainObjectExtension final : public Extension {
	MainObjectExtension();

public:
	METHOD_CREATE(MainObjectExtension)

	EXT_TYPE_NAME("MainObjectExtension")

	ExtensionProperty<std::string> propertyName;
	ExtensionProperty<glm::vec3> propertyPosition;
};

} // namespace mer::sdk::main

#endif //BASICOBJECTEXTENSION_H
