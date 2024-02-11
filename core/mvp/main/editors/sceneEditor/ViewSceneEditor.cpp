//
// Created by alexus on 06.02.24.
//

#include "ViewSceneEditor.h"

namespace n::core::mvp {
ViewSceneEditor::ViewSceneEditor() {
	Gtk::Notebook leftNotebook;
	leftNotebook.append_page(objectWindow, "Дерево объектов");
	leftNotebook.set_show_border(false);

	Gtk::Paned leftPaned;
	leftPaned.set_resize_start_child(false);
	leftPaned.set_start_child(leftNotebook);
	leftPaned.set_shrink_start_child(false);


	Gtk::Overlay areaOverlay;
	area.set_size_request(200, 200);
	areaOverlay.set_child(area);

	loadingBox.set_halign(Gtk::Align::CENTER);
	loadingBox.set_valign(Gtk::Align::CENTER);
	Gtk::Spinner loadingSpinner;
	loadingSpinner.set_spinning(true);
	loadingSpinner.set_size_request(25);
	loadingBox.append(loadingSpinner);
	Gtk::Label loadingLabel("Загрузка сцены...");
	loadingBox.append(loadingLabel);

	loadingErrorLabel.set_visible(false);
	loadingBox.append(loadingErrorLabel);

	areaOverlay.add_overlay(loadingBox);
	leftPaned.set_end_child(areaOverlay);
	leftPaned.set_shrink_end_child(false);
	leftPaned.set_resize_end_child(true);

	mainWidget.set_start_child(leftPaned);
	mainWidget.set_shrink_start_child(false);

	Gtk::Notebook rightNotebook;
	rightNotebook.set_show_border(false);
	rightNotebook.append_page(propertiesWindow, "Свойства объекта");
	mainWidget.set_end_child(rightNotebook);
	mainWidget.set_shrink_end_child(false);
	mainWidget.set_resize_end_child(false);

	/*objectWindow.setSlotGetObjectChildren([this](const SceneObject* /*pObject#1#) {
		std::vector<std::shared_ptr<SceneObject>> children;

		for (const auto &toplevelObject: project->getToplevelObjects()) { children.emplace_back(toplevelObject); }
		return children;
	});*/
}

sigc::connection ViewSceneEditor::connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) {

	return area.signal_render().connect(pSlot, false);
}

void ViewSceneEditor::onLoadingStarted() { loadingBox.set_visible(true); }

void ViewSceneEditor::onLoadingStopped(const engine::utils::ReportMessagePtr &pError) {
	if (pError) {
		loadingErrorLabel.set_label(pError->getReport());
		loadingErrorLabel.set_visible();
		engine::utils::Logger::error(pError);
		return;
	}
	loadingBox.set_visible(false);
}
} // namespace n::core::mvp