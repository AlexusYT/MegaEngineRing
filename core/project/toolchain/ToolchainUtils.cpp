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
		return WEXITSTATUS(wstatus);
	}
}

n::engine::utils::ReportMessageUPtr ToolchainUtils::generateCMakeListsFile(const std::filesystem::path &pPath) {
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

#Generated by the editor. DO NOT REMOVE THIS LINE.
include(cmake/internal/general.cmake)

# ---- Declare library ----



add_library(
        GameEngine_lib OBJECT ${GENERATED_SOURCES}

)
target_compile_features(GameEngine_lib PUBLIC cxx_std_23)

target_include_directories(
        GameEngine_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)


# Generated by the editor. DO NOT REMOVE THIS LINE.
# To add new libraries, use vcpkg.json file.
add_required_libraries_target(GameEngine_lib)

# You can add your own libraries below this comment or add .cmake file to cmake/libraries/user and call
# add_library_target(<TARGET> <CMAKE_FILE>)





# ---- Declare executable ----

add_executable(GameEngine_exe source/main.cpp)
add_executable(GameEngine::exe ALIAS GameEngine_exe)

set_property(TARGET GameEngine_exe PROPERTY OUTPUT_NAME GameEngine)

target_compile_features(GameEngine_exe PRIVATE cxx_std_23)
target_link_libraries(GameEngine_exe PRIVATE stdc++exp)


target_link_libraries(GameEngine_exe PRIVATE GameEngine_lib )
# Remove existing Resources directory before copying and copy new resources
add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
        VERBATIM)

#add_dependencies(GameEngine_exe CopyResources)

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

include(cmake/internal/dev-mode.cmake))");
}

n::engine::utils::ReportMessageUPtr ToolchainUtils::generateCMakePresetsFile(const std::filesystem::path &pPath) {
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
				"CMAKE_CXX_STANDARD": "20",
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
				"CMAKE_SHARED_LINKER_FLAGS": "-Wl,--allow-shlib-undefined,--as-needed,-z,noexecstack,-z,relro,-z,now,-z,nodlopen"
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

n::engine::utils::ReportMessageUPtr ToolchainUtils::generateCMakeUserPresetsFile(const std::filesystem::path &pPath) {

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

n::engine::utils::ReportMessageUPtr ToolchainUtils::generateVcpkgManifestFile(const std::filesystem::path &pPath) {
	return writeFile(pPath, R"({
  "name" : "gameengine",
  "version-string" : "0.1.0",
  "dependencies" : [ {
    "name" : "fmt",
    "version>=" : "10.1.0"
  }, {
    "name" : "atk",
    "version>=" : "2.38.0#8"
  }, {
    "name" : "brotli",
    "version>=" : "1.1.0#1"
  }, {
    "name" : "bzip2",
    "version>=" : "1.0.8#5"
  }, {
    "name" : "cairo",
    "version>=" : "1.17.8#4"
  }, {
    "name" : "cairomm",
    "version>=" : "1.17.1#1"
  }, {
    "name" : "dirent",
    "version>=" : "1.24"
  }, {
    "name" : "egl-registry",
    "version>=" : "2022-09-20"
  }, {
    "name" : "expat",
    "version>=" : "2.5.0#3"
  }, {
    "name" : "fontconfig",
    "version>=" : "2.14.2#1"
  }, {
    "name" : "freetype",
    "version>=" : "2.12.1#4"
  }, {
    "name" : "fribidi",
    "version>=" : "1.0.13"
  }, {
    "name" : "gdk-pixbuf",
    "version>=" : "2.42.10#2"
  }, {
    "name" : "getopt",
    "version>=" : "0#3"
  }, {
    "name" : "gettext-libintl",
    "version>=" : "0.21.1#1"
  }, {
    "name" : "gettext",
    "version>=" : "0.21.1#3"
  }, {
    "name" : "glib",
    "version>=" : "2.78.0"
  }, {
    "name" : "glibmm",
    "version>=" : "2.76.0#1"
  }, {
    "name" : "gperf",
    "version>=" : "3.1#6"
  }, {
    "name" : "graphene",
    "version>=" : "1.10.8#2"
  }, {
    "name" : "gtk",
    "version>=" : "4.10.5#1"
  }, {
    "name" : "gtkmm",
    "version>=" : "4.10.0#1"
  }, {
    "name" : "harfbuzz",
    "version>=" : "8.2.1#3"
  }, {
    "name" : "libepoxy",
    "version>=" : "1.5.10#2"
  }, {
    "name" : "libffi",
    "version>=" : "3.4.4#6"
  }, {
    "name" : "libiconv",
    "version>=" : "1.17#1"
  }, {
    "name" : "libjpeg-turbo",
    "version>=" : "3.0.1"
  }, {
    "name" : "liblzma",
    "version>=" : "5.4.4"
  }, {
    "name" : "libpng",
    "version>=" : "1.6.39#1"
  }, {
    "name" : "libsass",
    "version>=" : "3.6.5#1"
  }, {
    "name" : "libsigcpp-3",
    "version>=" : "3.0.3#1"
  }, {
    "name" : "libsigcpp",
    "version>=" : "3.6.0"
  }, {
    "name" : "libuuid",
    "version>=" : "1.0.3#14"
  }, {
    "name" : "lzo",
    "version>=" : "2.10#9"
  }, {
    "name" : "pango",
    "version>=" : "1.50.14#4"
  }, {
    "name" : "pangomm",
    "version>=" : "2.50.1#3"
  }, {
    "name" : "pcre2",
    "version>=" : "10.42#1"
  }, {
    "name" : "pcre",
    "version>=" : "8.45#5"
  }, {
    "name" : "pixman",
    "version>=" : "0.42.2#2"
  }, {
    "name" : "pthread",
    "version>=" : "3.0.0#2"
  }, {
    "name" : "pthreads",
    "version>=" : "3.0.0#14"
  }, {
    "name" : "sassc",
    "version>=" : "3.6.2"
  }, {
    "name" : "tiff",
    "version>=" : "4.6.0#1"
  }, {
    "name" : "vcpkg-cmake-config",
    "version>=" : "2022-02-06#1"
  }, {
    "name" : "vcpkg-cmake-get-vars",
    "version>=" : "2023-03-02"
  }, {
    "name" : "vcpkg-cmake",
    "version>=" : "2023-05-04"
  }, {
    "name" : "vcpkg-tool-meson",
    "version>=" : "0.63"
  }, {
    "name" : "zlib",
    "version>=" : "1.3"
  }, {
    "name" : "glfw3",
    "version>=" : "3.3.8#3"
  }, {
    "name" : "glew",
    "version>=" : "2.2.0#3"
  }, {
    "name" : "glm",
    "version>=" : "0.9.9.8#2"
  }, {
    "name" : "ms-gsl",
    "version>=" : "4.0.0"
  }, {
    "name" : "pngpp",
    "version>=" : "0.2.10#2"
  } ],
  "default-features" : [ ],
  "features" : {
    "test" : {
      "description" : "Dependencies for testing",
      "dependencies" : [ {
        "name" : "catch2",
        "version>=" : "3.4.0"
      } ]
    }
  },
  "builtin-baseline" : "16ee2ecb31788c336ace8bb14c21801efb6836e4"
})");
}

n::engine::utils::ReportMessageUPtr ToolchainUtils::generateCMakeDirectory(const std::filesystem::path &pPath) {
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
function(add_required_libraries_target target)
	file(GLOB cmakeLibFiles RELATIVE ${CMAKE_SOURCE_DIR} "cmake/libraries/required/*.cmake")

	foreach(X IN LISTS cmakeLibFiles)
		include(${X})
	endforeach()

	target_include_directories(
			${target} ${warning_guard}
			PUBLIC
			"${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/include"
	)

	target_link_libraries(${target} PRIVATE "${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/lib/libGameEngine.a")
	target_link_libraries(${target} PRIVATE "${ENGINE_SDK_PATH}/${ENGINE_SDK_VERSION}/lib/libGameEngineUtils.a")
	target_link_libraries(${target} PRIVATE stdc++exp)
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

n::engine::utils::ReportMessageUPtr ToolchainUtils::writeFile(const std::filesystem::path &pPath,
															  const std::string &pContents) {
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
