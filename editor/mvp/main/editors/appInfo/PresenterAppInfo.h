//
// Created by alexus on 29.01.24.
//

#ifndef PRESENTERAPPINFO_H
#define PRESENTERAPPINFO_H
#include <mvp/main/editors/IEditorPresenter.h>

namespace mer::editor::mvp {
class IViewAppInfo;
class IModelAppInfo;

class PresenterAppInfo : public IEditorPresenter {
	std::shared_ptr<IViewAppInfo> viewAppInfo;
	std::shared_ptr<IModelAppInfo> modelAppInfo;
	sigc::signal<void(const std::string &pName)> tabHeaderChanged;

public:
	PresenterAppInfo(const std::shared_ptr<IViewAppInfo> &pViewAppInfo,
					 const std::shared_ptr<IModelAppInfo> &pModelAppInfo);

	operator Gtk::Widget&() override;

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) override {
		return tabHeaderChanged.connect(pSlot);
	}
};

} // namespace mer::editor::mvp

#endif //PRESENTERAPPINFO_H
