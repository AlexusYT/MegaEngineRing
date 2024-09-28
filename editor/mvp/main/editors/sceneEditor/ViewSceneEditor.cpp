// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 06.02.24.
//

#include "ViewSceneEditor.h"

#include <mvp/main/MainWindow.h>

#include "ResourcesContext.h"
#include "mvp/main/centerWindow/TabPlaceholder.h"

namespace mer::editor::mvp {
ViewSceneEditor::ViewSceneEditor(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {
	modeSwitch.set_margin(5);
	modeSwitch.set_sensitive(false);
	Gtk::Label simulatingLabel("Simulation");
	simulatingLabel.set_margin(5);
	Gtk::Box topBox;
	topBox.append(simulatingLabel);
	topBox.append(modeSwitch);

	mainWidget.append(topBox);
	mainWidget.append(area);

	area.set_required_version(4, 0);

	area.signal_create_context().connect(
		[this]() -> Glib::RefPtr<Gdk::GLContext> {
			const auto surface = area.get_native()->get_surface();
			auto sharedContext = surface->create_gl_context();
			auto glContext = surface->create_gl_context();
			sharedContext->realize();
			glContext->realize();
			resourcesContext = std::make_shared<ResourcesContext>(sharedContext);
			return glContext;
		},
		false);
	area.set_expand(true);
	area.set_auto_render(true);
	motionController = Gtk::EventControllerMotion::create();
	area.add_controller(motionController);

	gestureClick = Gtk::GestureClick::create();
	gestureClick->set_button();
	area.add_controller(gestureClick);

	keyController = Gtk::EventControllerKey::create();
	mainWidget.add_controller(keyController);
}

sigc::connection ViewSceneEditor::connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) {

	return area.signal_render().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectRealize(const sigc::slot<void()> &pSlot) {
	return area.signal_realize().connect(pSlot);
}

sigc::connection ViewSceneEditor::connectUnrealize(const sigc::slot<void()> &pSlot) {
	return area.signal_unrealize().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) {
	return area.signal_resize().connect(pSlot);
}

sigc::connection ViewSceneEditor::connectKeyPressedSignal(
	const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const {
	return keyController->signal_key_pressed().connect(pSlot, false);
}

sigc::connection ViewSceneEditor::connectKeyReleasedSignal(
	const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const {
	return keyController->signal_key_released().connect(pSlot, false);
}

void ViewSceneEditor::makeCurrent() { area.make_current(); }

void ViewSceneEditor::redraw() { area.queue_render(); }

void ViewSceneEditor::throwIfError() { area.throw_if_error(); }

void ViewSceneEditor::emitResize() { area.queue_resize(); }

void ViewSceneEditor::setTitle(const std::string &pTitle) { context->setTitle(pTitle); }

void ViewSceneEditor::openView() { context->addWidget(&mainWidget); }

void ViewSceneEditor::closeView() { context->removeWidget(); }
} // namespace mer::editor::mvp