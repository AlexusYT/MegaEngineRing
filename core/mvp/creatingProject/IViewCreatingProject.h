//
// Created by alexus on 24.01.24.
//

#ifndef IVIEWCREATINGPROJECT_H
#define IVIEWCREATINGPROJECT_H

#include <mvp/IView.h>

namespace MVP_CORE {
class IViewCreatingProject : public IView {
public:
	~IViewCreatingProject() override = default;

	virtual void addMessageToLog(const std::string &pMessage) = 0;

	virtual void reportError(engine::utils::ReportMessagePtr pMessage) = 0;
};
} // namespace MVP_CORE
#endif //IVIEWCREATINGPROJECT_H
