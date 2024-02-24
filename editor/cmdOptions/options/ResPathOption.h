//
// Created by alexus on 23.02.24.
//

#ifndef RESPATHOPTION_H
#define RESPATHOPTION_H
#include "Option.h"

namespace mer::editor::utils {
class ResPathOption final : public Option {
public:
	explicit ResPathOption(Glib::OptionGroup* pOptionGroup);

	bool onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring &pValue,
						bool /*pHasValue*/) override;
};
} // namespace mer::editor::utils

#endif //RESPATHOPTION_H
