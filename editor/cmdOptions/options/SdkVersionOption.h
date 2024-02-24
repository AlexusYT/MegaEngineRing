//
// Created by alexus on 22.02.24.
//

#ifndef SDKVERSIONOPTION_H
#define SDKVERSIONOPTION_H
#include "Option.h"

namespace mer::editor::utils {

class SdkVersionOption final : public Option {
public:
	explicit SdkVersionOption(Glib::OptionGroup* pOptionGroup);

	bool onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring &pValue,
						bool /*pHasValue*/) override;
};

} // namespace mer::editor::utils

#endif //SDKVERSIONOPTION_H
