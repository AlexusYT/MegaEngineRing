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
class ResourceLoadResult;
}

namespace mer::sdk::main {
class IMaterialResource;
class ITextureResource;
class FileSystemResourceBundle;
class IResourceLoaders;
class IModel3DResource;
class IModel3DObject;
class ISceneDataInjector;
class Application;
class ILoadedResources;
class IResourceLoadExecutor;
class Extension;
class IScene;
class ISceneObject;
} // namespace mer::sdk::main

namespace mer::editor::project {
class Sdk {
	void* handler{};
	std::shared_ptr<sdk::main::ISceneObject> (*createSceneObjectFunc)();
	std::shared_ptr<sdk::main::IScene> (*createSceneFunc)();
	void (*initExtensionRegistryFunc)();
	std::shared_ptr<sdk::main::Extension> (*newExtensionInstanceFunc)(const std::string &pName);
	std::shared_ptr<sdk::main::ILoadedResources> (*createLoadedResourcesFunc)();
	std::shared_ptr<sdk::main::Application> (*createApplicationFunc)();
	std::shared_ptr<sdk::main::ISceneDataInjector> (*createSceneInjectorFunc)(sdk::main::IScene* pScene);
	std::shared_ptr<sdk::main::IModel3DObject> (*createModel3DObjectFunc)();
	std::shared_ptr<sdk::main::IModel3DResource> (*createModel3DResourceFunc)();
	std::shared_ptr<sdk::main::ITextureResource> (*createTextureResourceFunc)();
	std::shared_ptr<sdk::main::IMaterialResource> (*createMaterialResourceFunc)();
	std::shared_ptr<sdk::main::IResourceLoaders> (*getResourceLoadersInstanceFunc)();
	std::shared_ptr<sdk::main::FileSystemResourceBundle> (*createFileSystemResourceBundleFunc)(
		const std::filesystem::path &pSearchPath);
	std::shared_ptr<sdk::main::ResourceLoadResult> (*createResourceLoadResultFunc)();
	std::string mainNamespace{};

	explicit Sdk(void* pHandler);

public:
	static std::shared_ptr<Sdk> create(const std::string &pSdkPath, sdk::utils::ReportMessagePtr &pReportMessageOut);

	~Sdk();

	std::shared_ptr<sdk::main::ISceneObject> createSceneObject() const { return createSceneObjectFunc(); }

	std::shared_ptr<sdk::main::IScene> createScene() const { return createSceneFunc(); }

	std::shared_ptr<sdk::main::ILoadedResources> createLoadedResources() const { return createLoadedResourcesFunc(); }

	void initExtensionRegistry() const { initExtensionRegistryFunc(); }

	std::shared_ptr<sdk::main::Extension> newExtensionInstance(const std::string &pName) const {
		return newExtensionInstanceFunc(pName);
	}

	std::shared_ptr<sdk::main::Application> createApplication() const { return createApplicationFunc(); }

	std::shared_ptr<sdk::main::ISceneDataInjector> createSceneInjector(sdk::main::IScene* pScene) const {
		return createSceneInjectorFunc(pScene);
	}

	std::shared_ptr<sdk::main::IModel3DObject> createModel3DObject() const { return createModel3DObjectFunc(); }

	std::shared_ptr<sdk::main::IModel3DResource> createModel3DResource() const { return createModel3DResourceFunc(); }

	std::shared_ptr<sdk::main::ITextureResource> createTextureResource() const { return createTextureResourceFunc(); }

	std::shared_ptr<sdk::main::IMaterialResource> createMaterialResource() const {
		return createMaterialResourceFunc();
	}

	std::shared_ptr<sdk::main::IResourceLoaders> getResourceLoadersInstance() const {
		return getResourceLoadersInstanceFunc();
	}

	std::shared_ptr<sdk::main::FileSystemResourceBundle> createFileSystemResourceBundle(
		const std::filesystem::path &pSearchPath) const {
		return createFileSystemResourceBundleFunc(pSearchPath);
	}

	std::shared_ptr<sdk::main::ResourceLoadResult> createResourceLoadResult() const {
		return createResourceLoadResultFunc();
	}

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
