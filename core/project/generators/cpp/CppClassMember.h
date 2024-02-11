//
// Created by alexus on 08.02.24.
//

#ifndef CPPCLASSMEMBER_H
#define CPPCLASSMEMBER_H

namespace PROJECT_CORE {
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
} // namespace PROJECT_CORE

#endif //CPPCLASSMEMBER_H
