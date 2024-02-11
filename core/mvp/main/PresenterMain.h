//
// Created by alexus on 26.01.24.
//

#ifndef PRESENTERMAIN_H
#define PRESENTERMAIN_H
#include <mvp/IPresenter.h>

namespace n::core::mvp {
class IViewMain;
class IModelMain;
} // namespace n::core::mvp

namespace MVP_CORE {

class PresenterMain : public IPresenter {
	std::shared_ptr<IViewMain> viewMain;
	std::shared_ptr<IModelMain> modelMain;

public:
	PresenterMain(const std::shared_ptr<IViewMain> &pViewMain, const std::shared_ptr<IModelMain> &pModelMain);

	~PresenterMain() override;

private:
	void generateCmakeFiles(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void build(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void run(const sigc::slot<void(int pExitCode)> &pOnFinish = {}) const;

	void logMessage(int pId, const std::string &pMessage) const;

	void logError(int pId, const std::string &pMessage) const;
};

} // namespace MVP_CORE

#endif //PRESENTERMAIN_H
