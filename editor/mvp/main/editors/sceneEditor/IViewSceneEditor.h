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

#ifndef IVIEWSCENEEDITOR_H
#define IVIEWSCENEEDITOR_H
#include <mvp/ThreadDispatcher.h>

namespace mer::editor::mvp {
class IViewSceneEditor : public ThreadDispatcher {
public:
	virtual ~IViewSceneEditor() = default;

	virtual Gtk::Widget &getMainWidget() = 0;

	virtual sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) = 0;

	virtual sigc::connection connectRealize(const sigc::slot<void()> &pSlot) = 0;

	virtual sigc::connection connectUnrealize(const sigc::slot<void()> &pSlot) = 0;

	virtual sigc::connection connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) = 0;

	virtual sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const = 0;

	virtual sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const = 0;

	virtual sigc::connection connectCursorMovedSignal(const sigc::slot<void(double pX, double pY)> &pSlot) const = 0;

	virtual void makeCurrent() = 0;

	virtual const Glib::RefPtr<Gdk::GLContext> &getSharedContext() const = 0;

	virtual void redraw() = 0;

	virtual void throwIfError() = 0;

	virtual void emitResize() = 0;

	virtual void onLoadingStarted() = 0;

	virtual void onLoadingStopped(const sdk::utils::ReportMessagePtr &pError) = 0;
};
} // namespace mer::editor::mvp


#endif //IVIEWSCENEEDITOR_H
