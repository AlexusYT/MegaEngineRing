//
// Created by alexus on 20.01.24.
//

#ifndef ACCESSMODIFIER_H
#define ACCESSMODIFIER_H

namespace PROJECT_CORE {
enum class AccessModifier { PUBLIC, PROTECTED, PRIVATE };

inline const char* to_string(const AccessModifier pE) {
	switch (pE) {
		case AccessModifier::PUBLIC: return "public";
		case AccessModifier::PROTECTED: return "protected";
		case AccessModifier::PRIVATE: return "private";
		default: return "";
	}
}
} // namespace PROJECT_CORE


#endif //ACCESSMODIFIER_H
