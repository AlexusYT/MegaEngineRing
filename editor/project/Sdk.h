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

#ifndef SDK_H
#define SDK_H

#include <typeinfo>

namespace mer::sdk::main {
class ISceneDataInjector;
class Application;
class ILoadedResources;
class IResources;
class Extension;
class ModelRequest;
class ShaderProgramRequest;
class IScene;
class ISceneObject;
} // namespace mer::sdk::main

namespace mer::editor::project {
class Sdk {
	void* handler{};
	std::shared_ptr<sdk::main::ISceneObject> (*createSceneObjectFunc)();
	std::shared_ptr<sdk::main::IScene> (*createSceneFunc)();
	const std::shared_ptr<sdk::main::ShaderProgramRequest> &(*defaultShaderProgramFunc)();
	std::shared_ptr<sdk::main::ModelRequest> (*createFileModelRequestFunc)(const std::string &pName,
																		   const std::filesystem::path &pFilePath);
	void (*initExtensionRegistryFunc)();
	std::shared_ptr<sdk::main::Extension> (*newExtensionInstanceFunc)(const std::string &pName);
	std::shared_ptr<sdk::main::ILoadedResources> (*createLoadedResourcesFunc)(sdk::main::IResources* pResources);
	std::shared_ptr<sdk::main::Application> (*createApplicationFunc)();
	std::shared_ptr<sdk::main::ISceneDataInjector> (*createSceneInjectorFunc)(sdk::main::IScene* pScene);
	std::string mainNamespace{};
	explicit Sdk(void* pHandler);


public:
	static std::shared_ptr<Sdk> create(const std::string &pSdkPath, sdk::utils::ReportMessagePtr &pReportMessageOut);

	~Sdk();

	std::shared_ptr<sdk::main::ISceneObject> createSceneObject() const;

	std::shared_ptr<sdk::main::IScene> createScene() const;

	const std::shared_ptr<sdk::main::ShaderProgramRequest> &getDefaultShaderProgram() const;

	std::shared_ptr<sdk::main::ModelRequest> createFileModelRequest(const std::string &pName,
																	const std::filesystem::path &pFilePath) const;

	std::shared_ptr<sdk::main::ILoadedResources> createLoadedResources(sdk::main::IResources* pResources) const;

	void initExtensionRegistry();

	std::shared_ptr<sdk::main::Extension> newExtensionInstance(const std::string &pName) const;

	std::shared_ptr<sdk::main::Application> createApplication() const;

	std::shared_ptr<sdk::main::ISceneDataInjector> createSceneInjector(sdk::main::IScene* pScene) const;

	template<typename ClassT>
	std::shared_ptr<ClassT> newExtensionInstance() {
		std::shared_ptr<ClassT> (*sym)(const std::string &pName);
		getSymbol("ExtensionRegistry", "newInstance", sym);
		return sym(ClassT::typeName());
	}

private:
	sdk::utils::ReportMessagePtr initSymbols();

	std::string buildSymName(const std::string &pClassName, const std::string &pMethodName) const;

	template<typename RetT, typename... ArgsT>
	sdk::utils::ReportMessagePtr getSymbol(const std::string &pClassName, const std::string &pMethodName,
										   RetT (*&pSym)(ArgsT...), std::vector<std::string> stringArgs = {}) const {
		sdk::utils::ReportMessagePtr msg;
		std::stringstream ss;
		ss << "_Z" << mainNamespace << pClassName.size() << pClassName << pMethodName.size() << pMethodName << 'E';
		std::initializer_list<const std::type_info*> args = {&typeid(ArgsT)...};
		if (args.size() == 0) {
			ss << "v";
		} else if (stringArgs.empty()) {
			/*auto x = &typeid(std::string);
			x.*/
			for (auto arg: args) {
				if (!arg->__is_pointer_p()) ss << "RK";
				ss << arg->name();
			}
		} else {
			for (auto arg: stringArgs) { ss << arg; }
		}
		//_ZN3mer3sdk4main15LoadedResources6createEPNS1_10IResourcesE
		//_ZN3mer3sdk4main15LoadedResources6createEPN3mer3sdk4main10IResourcesE
		auto sym = getSymbol(ss.str(), msg);
		if (!sym) return msg;
		pSym = reinterpret_cast<RetT (*)(ArgsT...)>(sym);
		return nullptr;
	}

	void* getSymbol(const std::string &pSymName, sdk::utils::ReportMessagePtr &pReportMessageOut) const;
};
} // namespace mer::editor::project


#endif //SDK_H
