//
// Created by alexus on 06.12.23.
//

#ifndef COMMON_H
#define COMMON_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wextra-semi"
#include <glibmm.h>
#include <gtkmm.h>
#pragma GCC diagnostic pop

#include <EngineUtils/utils/Logger.h>
#include <EngineUtils/utils/ReportMessage.h>
#include <filesystem>
#include <fstream>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#define GENERAL_NAMESPACE n
#define CORE_NAMESPACE GENERAL_NAMESPACE::core
#define UI_CORE CORE_NAMESPACE::ui
#define MVP_CORE CORE_NAMESPACE::mvp
#define PROJECT_CORE CORE_NAMESPACE::project
#define UTILS_CORE CORE_NAMESPACE::utils

#endif //COMMON_H
