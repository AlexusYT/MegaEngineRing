//
// Created by alexus on 31.12.23.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <typeinfo>

class Utils {
public:
	template<typename T>
	static std::string getTypeName(T pE) {
		if constexpr (std::is_pointer<T>()) return demangle(typeid(*pE).name());
		else
			return demangle(typeid(pE).name());
	}

	static std::string demangle(const std::string &pMangledName);
};


#endif //UTILS_H
