//
// Created by alexus on 27.01.24.
//

#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H
#include "ProjectExplorerEntry.h"

namespace n::core::ui {

class DirectoryEntry : public ProjectExplorerEntry {
protected:
	DirectoryEntry(const std::string &pName) : ProjectExplorerEntry(pName) {}

public:
	static std::shared_ptr<DirectoryEntry> create(const std::string &pName) {
		return Glib::make_refptr_for_instance(new DirectoryEntry(pName));
	}
};
} // namespace n::core::ui


#endif //DIRECTORYENTRY_H
