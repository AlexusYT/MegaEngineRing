//
// Created by alexus on 24.01.24.
//

#ifndef IVIEWCREATINGPROJECT_H
#define IVIEWCREATINGPROJECT_H

#include <mvp/IView.h>

namespace mer::editor::mvp {
class IViewCreatingProject : public IView {
public:
	~IViewCreatingProject() override = default;

	virtual void addMessageToLog(const std::string &pMessage) = 0;

	virtual void reportError(sdk::utils::ReportMessagePtr pMessage) = 0;
};
} // namespace mer::editor::mvp
#endif //IVIEWCREATINGPROJECT_H
