//
// Created by alexus on 20.01.24.
//

#ifndef CPPMETHOD_H
#define CPPMETHOD_H
#include <project/generators/GeneratorElement.h>
#include <project/generators/cpp/CppBodyStatement.h>

namespace n::core::project {
class CppClass;
}

namespace PROJECT_CORE {
class CppMethod : public GeneratorElement {
	CppClass* klass{};
	std::string name;
	std::string returnType;
	bool isVirtual{};
	bool isPureVirtual{};
	bool isOverride{};
	bool isExtern{};
	bool isNoexcept{};
	bool isConstexpr{};
	bool isConst{};
	bool isStatic{};
	std::vector<std::string> paramsList;
	std::list<std::shared_ptr<CppBodyStatement>> statements;

public:
	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getReturnType() const { return returnType; }

	void setReturnType(const std::string &pReturnType) { returnType = pReturnType; }

	[[nodiscard]] const std::vector<std::string> &getParamsList() const { return paramsList; }

	void setParamsList(const std::vector<std::string> &pParamsList) { paramsList = pParamsList; }

	void addParam(const std::string &pParamName) { paramsList.emplace_back(pParamName); }

	[[nodiscard]] CppClass* getClass() const { return klass; }

	void setClass(CppClass* const pClass) { klass = pClass; }

	[[nodiscard]] bool isIsVirtual() const { return isVirtual; }

	void setIsVirtual(const bool pIsVirtual) { isVirtual = pIsVirtual; }

	[[nodiscard]] bool isIsOverride() const { return isOverride; }

	void setIsOverride(const bool pIsOverride) { isOverride = pIsOverride; }

	[[nodiscard]] bool isIsExtern() const { return isExtern; }

	void setIsExtern(const bool pIsExtern) { isExtern = pIsExtern; }

	[[nodiscard]] bool isIsNoexcept() const { return isNoexcept; }

	void setIsNoexcept(const bool pIsNoexcept) { isNoexcept = pIsNoexcept; }

	[[nodiscard]] bool isIsConstexpr() const { return isConstexpr; }

	void setIsConstexpr(const bool pIsConstexpr) { isConstexpr = pIsConstexpr; }

	[[nodiscard]] bool isIsConst() const { return isConst; }

	void setIsConst(const bool pIsConst) { isConst = pIsConst; }

	[[nodiscard]] bool isIsStatic() const { return isStatic; }

	void setIsStatic(const bool pIsStatic) { isStatic = pIsStatic; }

	[[nodiscard]] bool isIsPureVirtual() const { return isPureVirtual; }

	void setIsPureVirtual(const bool pIsPureVirtual) { isPureVirtual = pIsPureVirtual; }

	void addStatement(std::shared_ptr<CppBodyStatement> pStatement) { statements.emplace_back(pStatement); }

	std::string getDefinition(const uint32_t pIndent) override {
		const std::string tabs(pIndent, '\t');
		using namespace std::string_view_literals;
		std::stringstream ss;
		ss << tabs << declaration();
		if (!isPureVirtual) {
			ss << " {\n";
			for (const auto &statement: statements) { ss << statement->getCode(pIndent + 1) << "\n"; }
			ss << tabs << "}";
		} else
			ss << " = 0;";
		return ss.str();
	}

	std::string getDeclaration() override { return declaration() + ";"; }

private:
	std::string declaration() {
		using namespace std::string_view_literals;
		std::stringstream ss;
		if (isStatic) ss << "static ";
		if (isConstexpr) ss << "constexpr ";
		if (isExtern && !klass) ss << "extern ";
		if ((isVirtual || isPureVirtual) && !isStatic) ss << "virtual ";

		ss << returnType << " " << name << "(";
		if (!paramsList.empty()) {
			const auto str =
				std::ranges::fold_left(paramsList | std::views::join_with(", "sv), std::string{}, std::plus{});

			ss << str;
		}

		ss << ")";
		if (isConst && !isStatic) ss << " const";
		if (isOverride && !isStatic) ss << " override";
		if (isNoexcept) ss << " noexcept";
		return ss.str();
	}
};

class CppMethodCall : public CppBodyStatement {
	std::string methodName;
	std::list<std::string> argsList;

	CppMethodCall() = default;

	CppMethodCall(const std::string &pMethodName, const std::list<std::string> &pArgsList)
		: methodName(pMethodName), argsList(pArgsList) {}

public:
	static std::shared_ptr<CppMethodCall> create(const std::string &pMethodName,
												 const std::list<std::string> &pArgsList = {}) {
		return std::shared_ptr<CppMethodCall>(new CppMethodCall(pMethodName, pArgsList));
	}

	static std::shared_ptr<CppMethodCall> create() { return std::shared_ptr<CppMethodCall>(new CppMethodCall()); }

	[[nodiscard]] const std::string &getMethodName() const { return methodName; }

	void setMethodName(const std::string &pMethodName) { methodName = pMethodName; }

	[[nodiscard]] const std::list<std::string> &getArgsList() const { return argsList; }

	void setArgsList(const std::list<std::string> &pArgsList) { argsList = pArgsList; }

	void addArg(const std::string &pArg) { argsList.emplace_back(pArg); }

	std::string getCode(const uint32_t pIndent) override {
		const std::string tabs(pIndent, '\t');
		using namespace std::string_view_literals;
		std::stringstream ss;
		const auto str = std::ranges::fold_left(argsList | std::views::join_with(", "sv), std::string{}, std::plus{});
		ss << tabs << methodName << "(" << str << ");";
		return ss.str();
	}
};
} // namespace PROJECT_CORE


#endif //CPPMETHOD_H
