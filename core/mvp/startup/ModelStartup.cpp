//
// Created by alexus on 25.01.24.
//

#include "ModelStartup.h"

#include <Globals.h>

n::core::mvp::ModelStartup::ModelStartup() : name("Untitled"), path(Globals::getProjectsPath() / name) {}