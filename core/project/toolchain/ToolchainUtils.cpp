//
// Created by alexus on 11.12.23.
//

#include "ToolchainUtils.h"

#include <ranges>
#include <sys/wait.h>
#include <thread>

#define NUM_PIPES 2

#define PARENT_WRITE_PIPE 0
#define PARENT_READ_PIPE 1

int pipes[NUM_PIPES][2];

/* always in a pipe[], pipe[0] is for read and
   pipe[1] is for write */
#define READ_FD 0
#define WRITE_FD 1

#define PARENT_READ_FD (pipes[PARENT_READ_PIPE][READ_FD])
#define PARENT_WRITE_FD (pipes[PARENT_WRITE_PIPE][WRITE_FD])

#define CHILD_READ_FD (pipes[PARENT_WRITE_PIPE][READ_FD])
#define CHILD_WRITE_FD (pipes[PARENT_READ_PIPE][WRITE_FD])

void ToolchainUtils::execute(const std::filesystem::path &pRootPath, const std::filesystem::path &pPath,
							 const std::string &pArgs, const sigc::slot<void(const std::string &pLog)> &pCoutCallback,
							 const sigc::slot<void(const std::string &pLog)> &pCerrCallback,
							 const sigc::slot<void(int pReturnValue)> &pOnFinish) {

	std::jthread thrd([pArgs, pPath, pCoutCallback, pCerrCallback, pRootPath, pOnFinish] {
		n::engine::utils::Logger::info("Command invoked: {} {} at {}", pPath.string(), pArgs, pRootPath.string());
		pCoutCallback(pPath.string() + " " + pArgs + "\n");
		// pipes for parent to write and read
		pipe(pipes[PARENT_READ_PIPE]);
		pipe(pipes[PARENT_WRITE_PIPE]);

		auto pid = fork();
		if (pid == 0) {

			dup2(pipes[PARENT_WRITE_PIPE][WRITE_FD], STDERR_FILENO);
			dup2(pipes[PARENT_READ_PIPE][WRITE_FD], STDOUT_FILENO);


			/* Close fds not required by child. Also, we don't
		   want the exec'ed program to know these existed */
			close(pipes[PARENT_WRITE_PIPE][READ_FD]);
			close(pipes[PARENT_READ_PIPE][WRITE_FD]);
			close(pipes[PARENT_READ_PIPE][READ_FD]);
			close(pipes[PARENT_WRITE_PIPE][WRITE_FD]);

			std::vector<std::string> argsArray;
			argsArray.emplace_back(pPath.filename());
			for (const auto word: std::views::split(pArgs, ' ')) {
				std::string s(word.begin(), word.end());
				if (s.empty()) continue;
				argsArray.emplace_back(s);
			}
			std::vector<char*> argsTmp;
			argsTmp.reserve(argsArray.size());
			for (auto &&arg: argsArray) { argsTmp.emplace_back(arg.data()); }
			argsTmp.emplace_back(nullptr);

			current_path(pRootPath);

			execv(pPath.c_str(), argsTmp.data());
			std::cout << "Testr111\n";
		} else {
			/* close fds not required by parent */
			close(pipes[PARENT_WRITE_PIPE][WRITE_FD]);
			close(pipes[PARENT_READ_PIPE][WRITE_FD]);


			std::mutex mutex;
			std::jthread jthread([&mutex, pCerrCallback] {
				char buffer[4096] = {};
				ssize_t size;
				while ((size = read(pipes[PARENT_WRITE_PIPE][READ_FD], buffer, sizeof(buffer) - 1)) > 0) {
					std::lock_guard lock(mutex);
					pCerrCallback({buffer, static_cast<std::string::size_type>(size)});
				}
			});
			jthread.detach();
			{
				char buffer[8192] = {};
				ssize_t size;
				while ((size = read(pipes[PARENT_READ_PIPE][READ_FD], buffer, sizeof(buffer) - 1)) > 0) {
					std::lock_guard lock(mutex);
					pCoutCallback({buffer, static_cast<std::string::size_type>(size)});
				}
			}
		}

		if (pid > 0) {
			int wstatus;
			waitpid(pid, &wstatus, 0); // Store proc info into wstatus
			int returnValue = WEXITSTATUS(wstatus);
			pOnFinish(returnValue);
		}
	});
	thrd.detach();
}

int ToolchainUtils::executeSync(const std::filesystem::path &pRootPath, const std::filesystem::path &pPath,
								const std::string &pArgs,
								const sigc::slot<void(const std::string &pLog)> &pCoutCallback,
								const sigc::slot<void(const std::string &pLog)> &pCerrCallback) {

	n::engine::utils::Logger::info("Command invoked: {} {} at {}", pPath.string(), pArgs, pRootPath.string());
	pCoutCallback(pPath.string() + " " + pArgs + "\n");
	// pipes for parent to write and read
	pipe(pipes[PARENT_READ_PIPE]);
	pipe(pipes[PARENT_WRITE_PIPE]);

	auto pid = fork();
	if (pid == 0) {

		dup2(pipes[PARENT_WRITE_PIPE][WRITE_FD], STDERR_FILENO);
		dup2(pipes[PARENT_READ_PIPE][WRITE_FD], STDOUT_FILENO);


		/* Close fds not required by child. Also, we don't
		   want the exec'ed program to know these existed */
		close(pipes[PARENT_WRITE_PIPE][READ_FD]);
		close(pipes[PARENT_READ_PIPE][WRITE_FD]);
		close(pipes[PARENT_READ_PIPE][READ_FD]);
		close(pipes[PARENT_WRITE_PIPE][WRITE_FD]);

		std::vector<std::string> argsArray;
		argsArray.emplace_back(pPath.filename());
		for (const auto word: std::views::split(pArgs, ' ')) {
			std::string s(word.begin(), word.end());
			if (s.empty()) continue;
			argsArray.emplace_back(s);
		}
		std::vector<char*> argsTmp;
		argsTmp.reserve(argsArray.size());
		for (auto &&arg: argsArray) { argsTmp.emplace_back(arg.data()); }
		argsTmp.emplace_back(nullptr);

		current_path(pRootPath);

		execv(pPath.c_str(), argsTmp.data());
		std::cout << "Testr111\n";
	} else {
		/* close fds not required by parent */
		close(pipes[PARENT_WRITE_PIPE][WRITE_FD]);
		close(pipes[PARENT_READ_PIPE][WRITE_FD]);


		std::mutex mutex;
		std::jthread jthread([&mutex, pCerrCallback] {
			char buffer[4096] = {};
			ssize_t size;
			while ((size = read(pipes[PARENT_WRITE_PIPE][READ_FD], buffer, sizeof(buffer) - 1)) > 0) {
				std::lock_guard lock(mutex);
				pCerrCallback({buffer, static_cast<std::string::size_type>(size)});
			}
		});
		jthread.detach();
		{
			char buffer[8192] = {};
			ssize_t size;
			while ((size = read(pipes[PARENT_READ_PIPE][READ_FD], buffer, sizeof(buffer) - 1)) > 0) {
				std::lock_guard lock(mutex);
				pCoutCallback({buffer, static_cast<std::string::size_type>(size)});
			}
		}
	}

	if (pid > 0) {
		int wstatus;
		waitpid(pid, &wstatus, 0); // Store proc info into wstatus
		n::engine::utils::Logger::info("Command {} finished with {}", pPath.string(), wstatus);
		return WEXITSTATUS(wstatus);
	}
	return 1;
}

n::engine::utils::ReportMessagePtr ToolchainUtils::generateCMakeListsFile(const std::filesystem::path &pPath) {
	//language=cmake
	return writeFile(pPath, R"(

cmake_minimum_required(VERSION 3.14)

#In-source guard. Generated by the editor. You can remove it if necessary.
include(cmake/internal/in-source-guard.cmake)

project(
        GameEngine
        VERSION 0.1.0
        DESCRIPTION "Game engine"
        HOMEPAGE_URL "https://example.com/"
        LANGUAGES CXX
)

# You must specify your own sources here. Like this:
# set(USER_SOURCES source/foo.h source/foobar.h)
set(USER_SOURCES )

#Generated by the editor. DO NOT REMOVE THIS LINE.
include(cmake/internal/general.cmake)

# ---- Declare library ----
add_library(
        GameEngine_lib OBJECT ${GENERATED_SOURCES} ${USER_SOURCES}

)
add_required_include_target(GameEngine_lib)


# ---- Declare executable ----

add_executable(GameEngine_exe source/main.cpp)
add_executable(GameEngine::exe ALIAS GameEngine_exe)

# Generated by the editor. DO NOT REMOVE THIS LINE.
# To add new libraries, use vcpkg.json file.
add_required_libraries_target(GameEngine_exe)

# You can add your own libraries below this comment or add .cmake file to cmake/libraries/user and call
# add_library_target(<TARGET> <CMAKE_FILE>)

set_property(TARGET GameEngine_exe PROPERTY OUTPUT_NAME GameEngine)

target_compile_features(GameEngine_exe PRIVATE cxx_std_23)
target_link_libraries(GameEngine_exe PRIVATE stdc++exp)

add_dependencies(GameEngine_exe _EDITOR_TMP_)

target_link_libraries(GameEngine_exe PRIVATE GameEngine_lib epoxy)

# ---- Install rules ----

if (NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/internal/install-rules.cmake)
endif ()

# ---- Developer mode ----

if (NOT GameEngine_DEVELOPER_MODE)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    message(
            AUTHOR_WARNING
            "Developer mode is intended for developers of GameEngine"
    )
endif ()

include(cmake/internal/dev-mode.cmake)
)");
}

n::engine::utils::ReportMessagePtr ToolchainUtils::generateCMakePresetsFile(const std::filesystem::path &pPath) {
	return writeFile(pPath, R"({
	"version": 2,
	"cmakeMinimumRequired": {
		"major": 3,
		"minor": 14,
		"patch": 0
	},
	"configurePresets": [
		{
			"name": "dev-mode",
			"hidden": true,
			"cacheVariables": {
				"GameEngine_DEVELOPER_MODE": "ON",
				"VCPKG_MANIFEST_FEATURES": "test"
			}
		},
		{
			"name": "vcpkg",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
			}
		},
		{
			"name": "vcpkg-win64-static",
			"hidden": true,
			"cacheVariables": {
				"VCPKG_TARGET_TRIPLET": "x64-windows-static-md"
			}
		},
		{
			"name": "cppcheck",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_CPPCHECK": "cppcheck;--inline-suppr"
			}
		},
		{
			"name": "clang-tidy",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_CLANG_TIDY": "clang-tidy;--header-filter=^${sourceDir}/"
			}
		},
		{
			"name": "ci-std",
			"description": "This preset makes sure the project actually builds with at least the specified standard",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_EXTENSIONS": "OFF",
				"CMAKE_CXX_STANDARD": "23",
				"CMAKE_CXX_STANDARD_REQUIRED": "ON"
			}
		},
		{
			"name": "flags-gcc-clang",
			"description": "These flags are supported by both GCC and Clang",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_FLAGS": "-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS=1 -fstack-protector-strong -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor",
				"CMAKE_EXE_LINKER_FLAGS": "-Wl,--allow-shlib-undefined,--as-needed,-z,noexecstack,-z,relro,-z,now,-z,nodlopen",
				"CMAKE_SHARED_LINKER_FLAGS": "-Wl,--allow-shlib-undefined,--as-needed,-z,noexecstack,-z,relro,-z,now"
			}
		},
		{
			"name": "flags-appleclang",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_FLAGS": "-fstack-protector-strong -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Werror=float-equal -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor"
			}
		},
		{
			"name": "flags-msvc",
			"description": "Note that all the flags after /W4 are required for MSVC to conform to the language standard",
			"hidden": true,
			"cacheVariables": {
				"CMAKE_CXX_FLAGS": "/sdl /guard:cf /utf-8 /diagnostics:caret /w14165 /w44242 /w44254 /w44263 /w34265 /w34287 /w44296 /w44365 /w44388 /w44464 /w14545 /w14546 /w14547 /w14549 /w14555 /w34619 /w34640 /w24826 /w14905 /w14906 /w14928 /w45038 /W4 /permissive- /volatile:iso /Zc:inline /Zc:preprocessor /Zc:enumTypes /Zc:lambda /Zc:__cplusplus /Zc:externConstexpr /Zc:throwingNew /EHsc",
				"CMAKE_EXE_LINKER_FLAGS": "/machine:x64 /guard:cf"
			}
		},
		{
			"name": "ci-linux",
			"generator": "Ninja",
			"hidden": true,
			"inherits": [
				"flags-gcc-clang",
				"ci-std"
			],
			"cacheVariables": {
				"CMAKE_BUILD_TYPE": "Release"
			}
		},
		{
			"name": "ci-darwin",
			"generator": "Unix Makefiles",
			"hidden": true,
			"inherits": [
				"flags-appleclang",
				"ci-std"
			],
			"cacheVariables": {
				"CMAKE_BUILD_TYPE": "Release"
			}
		},
		{
			"name": "ci-win64",
			"inherits": [
				"flags-msvc",
				"ci-std"
			],
			"generator": "Visual Studio 17 2022",
			"architecture": "x64",
			"hidden": true
		},
		{
			"name": "coverage-linux",
			"binaryDir": "${sourceDir}/build/coverage",
			"inherits": "ci-linux",
			"hidden": true,
			"cacheVariables": {
				"ENABLE_COVERAGE": "ON",
				"CMAKE_BUILD_TYPE": "Coverage",
				"CMAKE_CXX_FLAGS_COVERAGE": "-Og -g --coverage -fkeep-inline-functions -fkeep-static-functions",
				"CMAKE_EXE_LINKER_FLAGS_COVERAGE": "--coverage",
				"CMAKE_SHARED_LINKER_FLAGS_COVERAGE": "--coverage",
				"CMAKE_MAP_IMPORTED_CONFIG_COVERAGE": "Coverage;RelWithDebInfo;Release;Debug;"
			}
		},
		{
			"name": "ci-coverage",
			"inherits": [
				"coverage-linux",
				"dev-mode",
				"vcpkg"
			],
			"cacheVariables": {
				"COVERAGE_HTML_COMMAND": ""
			}
		},
		{
			"name": "ci-sanitize",
			"binaryDir": "${sourceDir}/build/sanitize",
			"inherits": [
				"ci-linux",
				"dev-mode",
				"vcpkg"
			],
			"cacheVariables": {
				"CMAKE_BUILD_TYPE": "Sanitize",
				"CMAKE_CXX_FLAGS_SANITIZE": "-U_FORTIFY_SOURCE -O2 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-common",
				"CMAKE_MAP_IMPORTED_CONFIG_SANITIZE": "Sanitize;RelWithDebInfo;Release;Debug;"
			}
		},
		{
			"name": "ci-build",
			"binaryDir": "${sourceDir}/build",
			"hidden": true
		},
		{
			"name": "ci-macos",
			"inherits": [
				"ci-build",
				"ci-darwin",
				"dev-mode",
				"vcpkg"
			]
		},
		{
			"name": "ci-ubuntu",
			"inherits": [
				"ci-build",
				"ci-linux",
				"clang-tidy",
				"vcpkg",
				"cppcheck",
				"dev-mode"
			]
		},
		{
			"name": "ci-windows",
			"inherits": [
				"ci-build",
				"ci-win64",
				"dev-mode",
				"vcpkg",
				"vcpkg-win64-static"
			]
		}
	]
})");
}

n::engine::utils::ReportMessagePtr ToolchainUtils::generateCMakeUserPresetsFile(const std::filesystem::path &pPath) {

	return writeFile(pPath, R"({
	"version": 2,
	"cmakeMinimumRequired": {
		"major": 3,
		"minor": 14,
		"patch": 0
	},
	"configurePresets": [
		{
			"name": "dev-common",
			"hidden": true,
			"inherits": [
				"dev-mode",
				"vcpkg"
			],
			"cacheVariables": {
				"BUILD_MCSS_DOCS": "ON",
				"ENGINE_SDK_PATH": "/mnt/LinuxFS/CLionProjects/GameEngine/installed/sdk",
				"ENGINE_SDK_VERSION": "1.0.0"
			}
		},
		{
			"name": "dev-linux",
			"binaryDir": "${sourceDir}/build/dev-linux",
			"inherits": [
				"dev-common",
				"ci-linux"
			],
			"cacheVariables": {
				"CMAKE_BUILD_TYPE": "Debug",
				"CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
			}
		},
		{
			"name": "dev-darwin",
			"binaryDir": "${sourceDir}/build/dev-darwin",
			"inherits": [
				"dev-common",
				"ci-darwin"
			],
			"cacheVariables": {
				"CMAKE_BUILD_TYPE": "Debug",
				"CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
			}
		},
		{
			"name": "dev-win64",
			"binaryDir": "${sourceDir}/build/dev-win64",
			"inherits": [
				"dev-common",
				"ci-win64",
				"vcpkg-win64-static"
			],
			"environment": {
				"UseMultiToolTask": "true",
				"EnforceProcessCountAcrossBuilds": "true"
			}
		},
		{
			"name": "dev",
			"binaryDir": "${sourceDir}/build/dev",
			"inherits": "dev-linux",
			"environment": {
				"VCPKG_ROOT": "/home/alexus/.clion-vcpkg/vcpkg",
				"CC": "/home/alexus/gcc/bin/gcc",
				"CXX": "/home/alexus/gcc/bin/g++"
			}
		},
		{
			"name": "dev-coverage",
			"binaryDir": "${sourceDir}/build/coverage",
			"inherits": [
				"dev-mode",
				"coverage-linux",
				"vcpkg"
			]
		}
	],
	"buildPresets": [
		{
			"name": "dev",
			"configurePreset": "dev",
			"configuration": "Debug",
			"jobs": 20
		}
	],
	"testPresets": [
		{
			"name": "dev",
			"configurePreset": "dev",
			"configuration": "Debug",
			"output": {
				"outputOnFailure": true
			},
			"execution": {
				"jobs": 20,
				"noTestsAction": "error"
			}
		}
	]
})");
}

n::engine::utils::ReportMessagePtr ToolchainUtils::generateVcpkgManifestFile(const std::filesystem::path &pPath) {
	//language=json
	return writeFile(pPath, R"(
{
  "name": "gameengine",
  "version-string": "0.1.0",
  "dependencies": [
    {
      "name": "libsigcpp",
      "version>=": "3.6.0"
    },
    {
      "name": "glfw3",
      "version>=": "3.3.8#3"
    },

	{
		"name": "libepoxy",
		"version>=": "1.5.10#2"
	},
    {
      "name": "glm",
      "version>=": "0.9.9.8#2"
    },
    {
      "name": "ms-gsl",
      "version>=": "4.0.0"
    }
  ],
  "default-features": [],
  "features": {
    "test": {
      "description": "Dependencies for testing",
      "dependencies": [
        {
          "name": "catch2",
          "version>=": "3.4.0"
        }
      ]
    }
  },
  "builtin-baseline": "16ee2ecb31788c336ace8bb14c21801efb6836e4"
}
)");
}

n::engine::utils::ReportMessagePtr ToolchainUtils::generateCMakeDirectory(const std::filesystem::path &pPath) {
	const auto internalPath = pPath / "internal";
	create_directory(internalPath);
	if (auto msg = writeFile(internalPath / "general.cmake", R"(if( NOT ENGINE_SDK_PATH)
    message(FATAL_ERROR "You must specify ENGINE_SDK_PATH")
endif ()
if( NOT ENGINE_SDK_VERSION)
    message(FATAL_ERROR "You must specify ENGINE_SDK_VERSION")
endif ()

include(cmake/generated-sources.cmake)
include(cmake/internal/add-libraries.cmake)


add_library(
        _EDITOR_TMP_ SHARED ${GENERATED_SOURCES} ${USER_SOURCES}

)
target_compile_options(_EDITOR_TMP_ PUBLIC -rdynamic -fPIC)
add_required_include_target(_EDITOR_TMP_)


# This variable is set by project() in CMake 3.21+
string(
        COMPARE EQUAL
        "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}"
        PROJECT_IS_TOP_LEVEL
)

# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of GameEngine
# Targets necessary to build the project must be provided unconditionally, so
# consumers can trivially build and package the project
if (PROJECT_IS_TOP_LEVEL)
    option(GameEngine_DEVELOPER_MODE "Enable developer mode" OFF)
endif ()

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(warning_guard "")
if (NOT PROJECT_IS_TOP_LEVEL)
    option(
            GameEngine_INCLUDES_WITH_SYSTEM
            "Use SYSTEM modifier for GameEngine's includes, disabling warnings"
            ON
    )
    mark_as_advanced(GameEngine_INCLUDES_WITH_SYSTEM)
    if (GameEngine_INCLUDES_WITH_SYSTEM)
        set(warning_guard SYSTEM)
    endif ()
endif ()
)"))
		return msg;

	if (auto msg = writeFile(internalPath / "dev-mode.cmake", R"(include(cmake/internal/folders.cmake)

include(CTest)
if (BUILD_TESTING)
    #add_subdirectory(test)
endif ()

add_custom_target(
        run-exe
        COMMAND GameEngine_exe
        VERBATIM
)
add_dependencies(run-exe GameEngine_exe)

add_folders(Project)
)"))
		return msg;

	if (auto msg = writeFile(internalPath / "install-rules.cmake", R"(
install(
        TARGETS GameEngine_exe
        RUNTIME COMPONENT GameEngine_Runtime
)

if (PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif ()
)"))
		return msg;

	if (auto msg = writeFile(internalPath / "folders.cmake", R"(
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Call this function at the end of a directory scope to assign a folder to
# targets created in that directory. Utility targets will be assigned to the
# UtilityTargets folder, otherwise to the ${name}Targets folder. If a target
# already has a folder assigned, then that target will be skipped.
function(add_folders name)
    get_property(targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach (target IN LISTS targets)
        get_property(folder TARGET "${target}" PROPERTY FOLDER)
        if (DEFINED folder)
            continue()
        endif ()
        set(folder Utility)
        get_property(type TARGET "${target}" PROPERTY TYPE)
        if (NOT type STREQUAL "UTILITY")
            set(folder "${name}")
        endif ()
        set_property(TARGET "${target}" PROPERTY FOLDER "${folder}Targets")
    endforeach ()
endfunction()
)"))
		return msg;
	if (auto msg = writeFile(internalPath / "add-libraries.cmake", R"(
function(add_required_include_target target)

	target_include_directories(
			${target} ${warning_guard}
			PUBLIC
			"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
	)
	target_include_directories(
			${target} ${warning_guard}
			PUBLIC
			"${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/include"
	)
endfunction()

function(add_required_libraries_target target)
	add_required_include_target(${target})

	target_link_libraries(${target} PRIVATE "${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/lib/libGameEngine.a")
	target_link_libraries(${target} PRIVATE "${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/lib/libGameEngineUtils.a")
	target_link_libraries(${target} PRIVATE stdc++exp)
	file(GLOB cmakeLibFiles RELATIVE ${CMAKE_SOURCE_DIR} "cmake/libraries/required/*.cmake")

	foreach(X IN LISTS cmakeLibFiles)
		include(${X})
	endforeach()
endfunction()

function(add_library_target target file)
	include(${file})
endfunction()
)"))
		return msg;
	if (auto msg = writeFile(internalPath / "in-source-guard.cmake", R"(
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds are not supported. "
            "Please read the BUILDING document before trying to build this project. "
            "You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' first."
    )
endif ()
)"))
		return msg;
	return nullptr;
}

n::engine::utils::ReportMessagePtr ToolchainUtils::writeFile(const std::filesystem::path &pPath,
															 const std::string &pContents) {
	create_directories(pPath.parent_path());
	if (!exists(pPath.parent_path())) {
		auto msg = n::engine::utils::ReportMessage::create();
		msg->setTitle("Failed to write file");
		msg->setMessage("Parent directory doesn't exists");
		msg->addInfoLine("File path: {}", pPath.string());
		return msg;
	}
	std::optional<std::filesystem::file_time_type> time{std::nullopt};
	if (exists(pPath)) time = last_write_time(pPath);
	std::ofstream stream(pPath);
	stream << pContents << '\n';
	stream.close();
	if (time) last_write_time(pPath, time.value());
	return nullptr;
}
