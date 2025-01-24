//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 09.07.24.
//

#include "CanvasWidget.h"

#include "CanvasElement.h"

namespace mer::editor::mvp {
CanvasWidget::CanvasWidget() {
	set_focus_on_click();
	set_focusable();
	const auto scroll = Gtk::EventControllerScroll::create();
	add_controller(scroll);
	scroll->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
	scroll->signal_scroll().connect(
		[this](double /*pX*/, const double pY) {
			if (const auto delta = pY * 0.1; zoom - delta >= 0.2 && zoom - delta <= 3.1) {
				zoom -= delta;
				queue_draw();
			}
			return true;
		},
		true);
	gestureDrag = Gtk::GestureDrag::create();
	add_controller(gestureDrag);
	gestureDrag->set_button(0);
	gestureDrag->signal_drag_begin().connect([this](double /*pX*/, double /*pY*/) {
		if (gestureDrag->get_current_button() == 2) prevCanvasOffset = canvasOffset;
	});
	gestureDrag->signal_drag_update().connect([this](const double pX, const double pY) {
		const glm::dvec2 deltaPos{pX, pY};
		if (gestureDrag->get_current_button() == 2) {
			canvasOffset = prevCanvasOffset + deltaPos;
			queue_draw();
		}
	});
}

size_t CanvasWidget::addElement(CanvasElement* pElement) {
	elements.emplace(pElement);
	pElement->setWidget(this);
	this->queue_draw();
	return elements.size() - 1;
}

sdk::ReportMessagePtr CanvasWidget::removeElement(CanvasElement* pElement) {
	auto iter = elements.find(pElement);
	if (iter == elements.end()) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Unable to remove element");
		msg->setMessage("Element not found");
		msg->addInfoLine("Element typename: " + Utils::getTypeName(pElement));
		return msg;
	}
	elements.erase(iter);
	queue_draw();
	return nullptr;
}

uint32_t CanvasWidget::getCurrentDragButton() const { return gestureDrag->get_current_button(); }

sigc::connection CanvasWidget::connectDragBegin(const sigc::slot<void(double pX, double pY)> &pSlot,
												const bool pAfter) {
	return gestureDrag->signal_drag_begin().connect(pSlot, pAfter);
}

sigc::connection CanvasWidget::connectDragUpdate(const sigc::slot<void(double pX, double pY)> &pSlot,
												 const bool pAfter) {
	return gestureDrag->signal_drag_update().connect(pSlot, pAfter);
}

sigc::connection CanvasWidget::connectDragEnd(const sigc::slot<void(double pX, double pY)> &pSlot, const bool pAfter) {
	return gestureDrag->signal_drag_end().connect(pSlot, pAfter);
}

void CanvasWidget::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot> &pSnapshot) {
	Gdk::Graphene::Rect rect;
	if (auto bounds = compute_bounds(*this); bounds.has_value()) rect = bounds.value();

	const auto width = static_cast<double>(rect.get_width());
	const auto height = static_cast<double>(rect.get_height());

	const std::shared_ptr<Cairo::Context> context = pSnapshot->append_cairo(rect);

	Gdk::Cairo::set_source_rgba(context, background);
	const Gdk::Rectangle backgroundRect(0, 0, static_cast<int>(width), static_cast<int>(height));
	Gdk::Cairo::add_rectangle_to_path(context, backgroundRect);
	context->fill();

	const double step = 100 * zoom;

	context->translate(std::fmod(canvasOffset.x, step), std::fmod(canvasOffset.y, step));

	Gdk::Cairo::set_source_rgba(context, foreground);
	for (auto x = 0; x < width / step + 1; ++x) {
		context->move_to(step * x, -step);
		context->line_to(step * x, (height + step));
	}

	for (auto y = 0; y < height / step + 1; ++y) {
		context->move_to(-step, step * y);
		context->line_to(width + step, step * y);
	}
	context->stroke();

	for (const auto &canvasElement: elements) {
		context->set_identity_matrix();

		canvasElement->updateSize(context);

		context->translate(canvasOffset.x, canvasOffset.y);
		context->scale(zoom, zoom);
		canvasElement->draw(context);
	}

	context->set_identity_matrix();
	context->set_font_size(20);

	Cairo::FontExtents extents;
	context->get_font_extents(extents);
	Gdk::Cairo::set_source_rgba(context, bottomPanelBackground);
	context->rectangle(0, height - extents.height, width, extents.height);
	context->fill();
	context->move_to(0, height - extents.descent);
	Cairo::TextExtents textExtents;
	drawBottomText(context, std::format("Offset: {:.2f}, {:.2f}", canvasOffset.x, canvasOffset.y), textExtents);
	context->rel_move_to(-textExtents.width - 10, 0);
	drawBottomText(context, std::format("Zoom: {:.0f}%", zoom * 100), textExtents);
}

void CanvasWidget::drawBottomText(const std::shared_ptr<Cairo::Context> &pContext, const std::string &pText,
								  Cairo::TextExtents &pExtents) const {
	const int width = get_width();
	//const int height = allocation.get_height();

	glm::dvec2 lastPos;
	pContext->get_current_point(lastPos.x, lastPos.y);
	pContext->get_text_extents(pText, pExtents);
	Gdk::Cairo::set_source_rgba(pContext, foreground);
	pContext->rel_move_to(width - pExtents.width, 0);
	pContext->text_path(pText);
	pContext->fill();
	pContext->move_to(lastPos.x, lastPos.y);
}

} // namespace mer::editor::mvp

// mvp