//
// Created by alexus on 25.01.24.
//

#ifndef IMODELSTARTUP_H
#define IMODELSTARTUP_H

namespace MVP_CORE {
class IModelStartup {
public:
	virtual ~IModelStartup() = default;

	[[nodiscard]] virtual const std::string &getName() const = 0;

	virtual void setName(const std::string &pName) = 0;

	[[nodiscard]] virtual const std::filesystem::path &getPath() const = 0;

	virtual void setPath(const std::filesystem::path &pPath) = 0;
};
} // namespace MVP_CORE


#endif //IMODELSTARTUP_H
