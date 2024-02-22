//
// Created by alexus on 22.02.24.
//

#ifndef SDKPATHOPTION_H
#define SDKPATHOPTION_H
#include "Option.h"

namespace UTILS_CORE {

class SdkPathOption final : public Option {
public:
	explicit SdkPathOption(Glib::OptionGroup* pOptionGroup);

	bool onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring &pValue,
						bool /*pHasValue*/) override;
};

} // namespace UTILS_CORE

#endif //SDKPATHOPTION_H
