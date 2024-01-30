//
// Created by alexus on 31.12.23.
//

#ifndef ENGINEOPTIONGROUP_H
#define ENGINEOPTIONGROUP_H
#include "options/Option.h"

namespace UTILS_CORE {


class EngineOptionGroup final : public Glib::OptionGroup {
	std::vector<std::unique_ptr<Option>> optionEntries{};

public:
	EngineOptionGroup();

	explicit EngineOptionGroup(GOptionGroup* const pCastitem) : OptionGroup(pCastitem) {}

	explicit EngineOptionGroup(const OptionGroup &pOther) = delete;

	explicit EngineOptionGroup(OptionGroup &&pOther) = delete;

	~EngineOptionGroup() override;

protected:
	bool on_pre_parse(Glib::OptionContext &pContext) override;
	bool on_post_parse(Glib::OptionContext &pContext) override;
	void on_error(Glib::OptionContext &pContext, const Glib::Error &pError) override;
};
} // namespace CORE_UTILS_NAMESPACE


#endif //ENGINEOPTIONGROUP_H
