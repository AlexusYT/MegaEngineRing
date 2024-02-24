//
// Created by alexus on 06.02.24.
//

#ifndef PRESENTERSCENEEDITOR_H
#define PRESENTERSCENEEDITOR_H
#include "mvp/main/editors/IEditorPresenter.h"

namespace mer::editor::mvp {
class IViewSceneEditor;
class IModelSceneEditor;

class PresenterSceneEditor : public IEditorPresenter {
	std::shared_ptr<IViewSceneEditor> viewSceneEditor;
	std::shared_ptr<IModelSceneEditor> modelSceneEditor;
	sigc::signal<void(const std::string &pName)> tabHeaderChanged;

public:
	PresenterSceneEditor(const std::shared_ptr<IViewSceneEditor> &pViewSceneEditor,
						 const std::shared_ptr<IModelSceneEditor> &pModelSceneEditor);

	operator Gtk::Widget&() override;

	sigc::connection connectTabHeaderChanged(const sigc::slot<void(const std::string &pName)> &pSlot) override {

		return tabHeaderChanged.connect(pSlot);
	}

private:
	void notifyLoadingStarted() const;
	void notifyLoadingStopped(const sdk::utils::ReportMessagePtr &pError) const;

	sdk::utils::ReportMessagePtr loadScene() const;
};
} // namespace mer::editor::mvp


#endif //PRESENTERSCENEEDITOR_H
