//
// Created by alexus on 31.12.23.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <typeinfo>

class Utils {
private:
public:
	template<typename T>
	static std::string getTypeName(const T &pE) {
		return demangle(typeid(pE).name());
	}

	template<typename T>
	static std::string getTypeName(T* pE) {
		return demangle(typeid(*pE).name());
	}

	template<typename T>
	static std::string getTypeName() {
		std::string result;
		if constexpr (std::is_const_v<std::remove_reference_t<T>>) result += "const ";
		if constexpr (std::is_pointer<T>()) {
			result += demangle(typeid(std::remove_pointer_t<T>).name());

		} else
			result += demangle(typeid(T).name());
		if constexpr (std::is_pointer_v<T>) result += "*";
		if constexpr (std::is_reference_v<T>) result += " &";
		return result;
	}

	static std::string demangle(const std::string &pMangledName);

	static std::string parseDlError(const std::string &pMsg);
};


#endif //UTILS_H
