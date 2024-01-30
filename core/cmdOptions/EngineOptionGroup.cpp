//
// Created by alexus on 31.12.23.
//

#include "EngineOptionGroup.h"

#include "options/LogPathOption.h"

namespace UTILS_CORE {
EngineOptionGroup::EngineOptionGroup() : OptionGroup("engine-options", "Engine options", "Engine options") {
	optionEntries.emplace_back(std::make_unique<LogPathOption>(this));
}

EngineOptionGroup::~EngineOptionGroup() = default;

bool EngineOptionGroup::on_pre_parse(Glib::OptionContext &pContext) { return OptionGroup::on_pre_parse(pContext); }

bool EngineOptionGroup::on_post_parse(Glib::OptionContext &pContext) { return OptionGroup::on_post_parse(pContext); }

void EngineOptionGroup::on_error(Glib::OptionContext &pContext, const Glib::Error &pError) {
	OptionGroup::on_error(pContext, pError);
}
} // namespace CORE_UTILS_NAMESPACE