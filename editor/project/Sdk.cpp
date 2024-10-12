//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 31.08.24.
//

#include "Sdk.h"

#include <dlfcn.h>

namespace mer::editor::project {
Sdk::Sdk(void* pHandler) : handler(pHandler) { mainNamespace = "N3mer3sdk4main"; }

Sdk::~Sdk() {
	if (handler) dlclose(handler);
}

std::shared_ptr<Sdk> Sdk::create(const std::string &pSdkPath, sdk::utils::ReportMessagePtr &pReportMessageOut) {

	auto handler = dlopen(pSdkPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	if (!handler) {
		pReportMessageOut = sdk::utils::ReportMessage::create();
		pReportMessageOut->setTitle("Failed to open sdk shared library");
		pReportMessageOut->setMessage("The function dlopen() returned error");
		pReportMessageOut->addInfoLine("Path: {}", pSdkPath);
		pReportMessageOut->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
		return nullptr;
	}
	auto ptr = std::shared_ptr<Sdk>(new Sdk(handler));
	if (const auto msg = ptr->initSymbols()) {
		pReportMessageOut = msg;
		return nullptr;
	}
	return ptr;
}

sdk::utils::ReportMessagePtr Sdk::initSymbols() {
	if (auto msg = getSymbol("SceneObject", "create", createSceneObjectFunc)) return msg;
	if (auto msg = getSymbol("Scene", "create", createSceneFunc)) return msg;
	if (auto msg = getSymbol("BuiltInProgramRequest", "getDefaultProgram", defaultShaderProgramFunc)) return msg;
	if (auto msg = getSymbol("ExtensionRegistry", "init", initExtensionRegistryFunc)) return msg;
	if (auto msg = getSymbol("ExtensionRegistry", "newInstance", newExtensionInstanceFunc)) return msg;
	if (auto msg = getSymbol("LoadedResources", "create", createLoadedResourcesFunc)) return msg;
	if (auto msg = getSymbol("Application", "create", createApplicationFunc)) return msg;
	if (auto msg = getSymbol("SceneDataInjector", "create", createSceneInjectorFunc, {"PNS1_6ISceneE"})) return msg;
	if (auto msg = getSymbol("Model3DObject", "create", createModel3DObjectFunc)) return msg;
	if (auto msg = getSymbol("Model3DResource", "create", createModel3DResourceFunc)) return msg;
	if (auto msg = getSymbol("ResourceLoaders", "getInstance", getResourceLoadersInstanceFunc)) return msg;
	if (auto msg = getSymbol("FileSystemResourceBundle", "create", createFileSystemResourceBundleFunc)) return msg;
	if (auto msg = getSymbol("TextureResource", "create", createTextureResourceFunc)) return msg;

	return nullptr;
}

std::string Sdk::buildSymName(const std::string &pClassName, const std::string &pMethodName) const {
	std::stringstream ss;
	ss << "_Z" << mainNamespace << pClassName.size() << pClassName << pMethodName.size() << pMethodName << "Ev";
	return ss.str();
}

void* Sdk::getSymbol(const std::string &pSymName, sdk::utils::ReportMessagePtr &pReportMessageOut) const {
	auto sym = dlsym(handler, pSymName.c_str());
	if (!sym) {
		pReportMessageOut = sdk::utils::ReportMessage::create();
		pReportMessageOut->setTitle("Unable to load the sdk");
		pReportMessageOut->setMessage("The function dlsym() returned error");
		pReportMessageOut->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
		pReportMessageOut->addInfoLine("Generated symbol name: {}", pSymName);
		return nullptr;
	}
	return sym;
}
} // namespace mer::editor::project