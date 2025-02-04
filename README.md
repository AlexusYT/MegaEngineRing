# MegaEngineRing

MegaEngineRing is a game engine, written on C++.

As of October 30, 2024, the engine is at the concept stage.

The project consists of some components:

* Projects Editor
* Engine SDK
* Utils

# INSTALLING

## Windows

Installing on the Windows is currently unsupported, but with some minor you can do that.

## Linux

### 1. Preparing

To install it on Linux (tested on Ubuntu 23.10, but should work on earlier versions), you need:

- [gcc](https://gcc.gnu.org/) 14+ (or 12+ with --enable-libstdcxx-backtrace=yes)
- [CMake](https://cmake.org/)* v3.27+
- [ninja](https://github.com/ninja-build/ninja) v1.11.1
- [vcpkg](https://github.com/microsoft/vcpkg)

*It is very likely that earlier versions of CMake can be used, but this has not been tested yet.

Also, you may need to install some additional libraries for project dependencies:

- libx11-dev
- libxft-dev
- libxext-dev
- libxinerama-dev
- libxcursor-dev
- xorg-dev
- libglu1-mesa-dev

Install it with

```shell
sudo apt install libx11-dev libxft-dev libxext-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev
```

### 2. Configuring

First of all, you need to specify the path to the vcpkg using VCPKG_INSTALLATION_ROOT env variable. For example,

```shell
export VCPKG_INSTALLATION_ROOT="/home/builder/vcpkg"
```

And for gcc:

```shell
export CXX="/home/builder/gcc/bin/g++"
```

Run the following command in the project root:

```shell
cmake -DCMAKE_INSTALL_PREFIX={INSTALLATION_PATH} --preset ci-ubuntu -S ./ -B build/ci-ubuntu
```

Replace the {INSTALLATION_PATH} with your path. It's optional. Default path is `/usr/local`. For more info,
see [CMake Documentation](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html)
and [GNU Coding Standards](https://www.gnu.org/prep/standards/html_node/Directory-Variables.html).

Also, you can change a binary, sdk and resources installation path:

* `CMAKE_INSTALL_LOCALSTATEDIR` for sdk installation path. Default is `$(prefix)/var`
* `CMAKE_INSTALL_DATADIR` for resource installation path. Default is `$(datarootdir)`
* `CMAKE_INSTALL_BINDIR` for binary installation path. Default is `$(exec_prefix)/bin`

CMake will run vcpkg to install the required dependencies.

### 3. Building and Installing

To build and install the whole project, run

```shell
cmake --build build/ci-ubuntu
cmake --install build/ci-ubuntu
```

If you want to build and install only a specific program component

```shell
cmake --build build/ci-ubuntu --target={TARGET}
cmake --install build/ci-ubuntu --component={COMPONENT}
```

| Program component | {TARGET}         | {COMPONENT} |
|-------------------|------------------|-------------|
| SDK               | MegaEngineSDK    | sdk         |
| Editor            | MegaEngineEditor | editor      |

If you encounter any problems, please feel free to create a new issue.

### 4. Running

> MegaEngineSDK target now links to MegaEngineEditor directly, so no need to manually specify `--sdk-path` or
`--sdk-version`


~~Editor requires SDK to run. Normally it installs to `/usr/local/var/MegaEngineRing/sdk`. If SDK not found at this
path,
you can specify a path to SDK directory and SDK version manually.~~

* ~~`--sdk-path` to set a path to SDK directory.~~
* ~~`--sdk-version` to set an SDK version. By default, it is randomly selected. Currently only `0.1.0` version is
  available, so it's not a big problem.~~

### Another way

To simplify the installation process, I created the [Dockerfile](Dockerfile), that can build image for your Docker with
all necessary libraries and tools.

Note that the image creation process can take about an hour, but you can find the already created image
[here](https://hub.docker.com/r/alexusxx/mer-builder). How to use it is described in the Overview section.

## MacOS

Installing on the macOS is currently unsupported.

# CONTRIBUTING

See the [CONTRIBUTING.md](CONTRIBUTING.md).
