//
// Created by alexus on 31.12.23.
//

#include "Option.h"

namespace mer::editor::utils {
bool Option::onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring & /*pValue*/,
							bool /*pHasValue*/) {
	return true;
}
} // namespace mer::editor::utils