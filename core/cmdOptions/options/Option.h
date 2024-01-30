//
// Created by alexus on 31.12.23.
//

#ifndef OPTION_H
#define OPTION_H

namespace UTILS_CORE {

class Option : public Glib::OptionEntry {
public:
	Option(Glib::OptionGroup* pOptionGroup, const Glib::ustring &pLongName, const Glib::ustring &pDescription) {

		set_long_name(pLongName);
		set_description(pDescription);
		pOptionGroup->add_entry(*this, sigc::mem_fun(*this, &Option::onOptionParsed));
	}

	~Option() override = default;

	virtual bool onOptionParsed(const Glib::ustring &pOptionName, const Glib::ustring &pValue, bool pHasValue);
};

} // namespace UTILS_CORE

#endif //OPTION_H
