//
// Created by alexus on 08.02.24.
//

#ifndef CPPHEADERFILE_H
#define CPPHEADERFILE_H
#include "CppFile.h"

namespace mer::editor::project {
class CppHeaderFile : public CppFile {

public:
	std::string getExtension() override { return "h"; }

	bool isHeader() override { return true; }
};

class CppSourceFile : public CppFile {

public:
	std::string getExtension() override { return "cpp"; }

	bool isHeader() override { return false; }
};
} // namespace mer::editor::project


#endif //CPPHEADERFILE_H
