# MegaEngineRing

MegaEngineRing is a game engine, written on C++.

As of February 20, 2024, the engine is at the concept stage.

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

First of all, you need to specify the path to the vcpkg using VCPKG_ROOT env variable. For example,

```shell
export VCPKG_ROOT=/home/builder/vcpkg
```

Run the following command in the project root:

```shell
cmake -DCMAKE_INSTALL_PREFIX={INSTALLATION_PATH} --preset ci-ubuntu -S ./ -B build/ci-ubuntu
```

Replace the {INSTALLATION_PATH} with your path. It's optional. Default path is `/usr/local`.

CMake will run vcpkg to install the required dependencies.

### 3. Building and Installing

To install the project, run

```shell
cmake --build build/ci-ubuntu -j 20 --target install
```

If you encounter any problems, please feel free to create a new issue.

### 4. Running

To run MegaEngineEditor executable, you need to specify path to SDK directory and sdk version to use.

* `--sdk-path` to set a path to SDK directory. By default, it is `${INSTALLATION_DIR}/sdk`
* `--sdk-version` to set an SDK version. By default, it is randomly selected. Currently only `1.0.0` version is
  available, so it's not a big problem.

### Another way

To simplify the installation process, I created the [Dockerfile](Dockerfile), that can build image for your Docker with
all necessary libraries and tools.

Note that the image creation process can take about an hour, but you can find the already created image
[here](https://hub.docker.com/r/alexusxx/mer-builder). How to use it is described in the Overview section.

## MacOS

Installing on the macOS is currently unsupported.

# CONTRIBUTING

See the [CONTRIBUTING.md](CONTRIBUTING.md).

# LICENSE En

The LICENSE file has not been written yet, but I can briefly tell you about your rights here.

You can view, download, modify, compile, run and use all the components of the product. You MAY NOT distribute, sell,
lease, sublicense or use any of the components of the product for similar purposes.

The files created by any of the product components are entirely yours. The developers do not claim the right to own
and/or dispose of your works. If you decide to distribute your works created with the product on a commercial basis, the
developers may require payment of royalties.

The voluntary contribution made to the development of the project by the participant is its intellectual property. By
making a contribution, the participant allows the developer to use it as part of the product.

To put it simply:

1. The engine can be used for any purpose except distribution and profit-making.
2. The created games or other works completely and unconditionally belong to you. If you want to sell, you may need to
   pay royalties.
3. If you want to help the project, your code remains yours, but you will not be able to withdraw it from the product in
   the future.

# LICENSE Ru

Пока что файл LICENSE еще не написан, но я могу кратко рассказать о ваших правах здесь.

Вы можете просматривать, скачивать, изменять, компилировать, запускать и использовать все компоненты продукта. Вы НЕ
МОЖЕТЕ распространять, продавать, сдавать в аренду, сублицензировать или использовать в подобных целях ни один из
компонентов продукта.

Файлы, созданные каким-либо из компонентов продукта полностью принадлежат вам. Разработчики не претендуют на право
владения и/или распоряжения вашими произведениями. Если вы решите распространять ваши произведения, созданные с помощью
продукта, на коммерческой основе, разработчики могут требовать уплаты роялти.

Добровольный вклад, внесенный в развитие проекта участником, является его интеллектуальной собственностью. Внося вклад,
участник позволяет разработчику использовать его в составе продукта.

Если проще:

1. Движок можно использовать в любых своих целях, кроме распространения и извлечения прибыли.
2. Созданные игры или иные произведения полностью и безоговорочно принадлежат вам. Если хотите продавать - возможно
   потребуется уплата роялти.
3. Если хотите помочь проекту, ваш код остается вашим, но вы не сможете его отозвать из продукта в дальнейшем.
