//
// Created by alexus on 30.07.23.
//

#include "InstancedSceneMeshObject.h"

#include "models/ModelsList.h"
#include "utils/Color.h"

bool InstancedSceneMeshObject::setModelObject(const std::string &modelName, const std::string &newModelObjectName,
											  ErrorReport &error) {
	rel::engine::Model* model = ModelsList::getModel(modelName);
	if (!model) {
		error.addMessage("No model with such name");
		error.addInfoToMain("Model name: " + modelName);
		error.addInfoToMain("Class name: " + Utils::getTypeName(this));
		return false;
	}
	return setModelObject(model, newModelObjectName, error);
}

bool InstancedSceneMeshObject::setModelObject(rel::engine::Model* model, const std::string &newModelObjectName,
											  ErrorReport &error) {
	if (!model) {
		error.addMessage("Given model ptr is null");
		error.addInfoToMain("Model object name: " + newModelObjectName);
		error.addInfoToMain("Class name: " + Utils::getTypeName(this));
		return false;
	}
	auto object = model->getObject(newModelObjectName);
	if (!object) {
		error.addMessage("No model object with such name");
		error.addInfoToMain("Model name: " + model->getName());
		error.addInfoToMain("Model object name: " + newModelObjectName);
		error.addInfoToMain("Class name: " + Utils::getTypeName(this));
		return false;
	}
	setModelObject(object);
	return true;
}

void InstancedSceneMeshObject::getVertices(std::vector<glm::vec3> &newVertices) {
	DEBUG_ASSERT(modelObject != nullptr, PrintHandler{}, "Model object cannot be a nullptr");
	newVertices = modelObject->getVertices();
}

void InstancedSceneMeshObject::getIndices(std::vector<uint16_t> &newIndices) {
	DEBUG_ASSERT(modelObject != nullptr, PrintHandler{}, "Model object cannot be a nullptr");
	newIndices = modelObject->getIndices();
}

void InstancedSceneMeshObject::getUVs(std::vector<glm::vec2> &newUVs) {
	DEBUG_ASSERT(modelObject != nullptr, PrintHandler{}, "Model object cannot be a nullptr");
	newUVs = modelObject->getUVs();
}

void InstancedSceneMeshObject::getColors(std::vector<glm::vec4> &newColors) {
	newColors.resize(getExpectedVerticesCount(), Colors::WHITE);
}

void InstancedSceneMeshObject::getNormals(std::vector<glm::vec3> &newNormals) {
	DEBUG_ASSERT(modelObject != nullptr, PrintHandler{}, "Model object cannot be a nullptr");
	newNormals = modelObject->getNormals();
}

bool InstancedSceneMeshObject::setBounds(rel::engine::Model* model, const std::string &newBoundsModelObjectName,
										 ErrorReport &error) {
	if (!model) {
		error.addMessage("Given model ptr is null");
		error.addInfoToMain("Bounds model object name: " + newBoundsModelObjectName);
		error.addInfoToMain("Class name: " + Utils::getTypeName(this));
		return false;
	}
	auto object = model->getObject(newBoundsModelObjectName);
	if (!object) {
		error.addMessage("No bounds model object with such name");
		error.addInfoToMain("Model name: " + model->getName());
		error.addInfoToMain("Bounds model object name: " + newBoundsModelObjectName);
		error.addInfoToMain("Class name: " + Utils::getTypeName(this));
		return false;
	}
	setBounds(object);
	return true;
}
