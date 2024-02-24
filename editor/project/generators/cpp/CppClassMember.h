//
// Created by alexus on 08.02.24.
//

#ifndef CPPCLASSMEMBER_H
#define CPPCLASSMEMBER_H

namespace mer::editor::project {
class CppClass;

class CppClassMember {
	CppClass* klass{};
	bool outsideClass{};

public:
	[[nodiscard]] CppClass* getKlass() const { return klass; }

	void setKlass(CppClass* const pKlass) { klass = pKlass; }

	[[nodiscard]] bool isOutsideClass() const { return outsideClass; }

	void setOutsideClass(const bool pOutsideClass) { outsideClass = pOutsideClass; }
};
} // namespace mer::editor::project

#endif //CPPCLASSMEMBER_H
