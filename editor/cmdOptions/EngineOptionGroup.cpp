//
// Created by alexus on 31.12.23.
//

#include "EngineOptionGroup.h"

#include "options/LogPathOption.h"
#include "options/ResPathOption.h"
#include "options/SdkPathOption.h"
#include "options/SdkVersionOption.h"

namespace mer::editor::utils {
EngineOptionGroup::EngineOptionGroup() : OptionGroup("engine-options", "Engine options", "Engine options") {
	optionEntries.emplace_back(std::make_unique<LogPathOption>(this));
	optionEntries.emplace_back(std::make_unique<SdkPathOption>(this));
	optionEntries.emplace_back(std::make_unique<SdkVersionOption>(this));
	optionEntries.emplace_back(std::make_unique<ResPathOption>(this));
}

EngineOptionGroup::~EngineOptionGroup() = default;

bool EngineOptionGroup::on_pre_parse(Glib::OptionContext &pContext) { return OptionGroup::on_pre_parse(pContext); }

bool EngineOptionGroup::on_post_parse(Glib::OptionContext &pContext) { return OptionGroup::on_post_parse(pContext); }

void EngineOptionGroup::on_error(Glib::OptionContext &pContext, const Glib::Error &pError) {
	OptionGroup::on_error(pContext, pError);
}
} // namespace mer::editor::utils