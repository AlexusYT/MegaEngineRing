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

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

namespace mer::editor::mvp {
class CanvasElement;
}

namespace mer::editor::mvp {

class CanvasWidget : public Gtk::Widget {
	std::shared_ptr<Gtk::GestureDrag> gestureDrag;
	Gtk::Border padding;
	Gdk::RGBA foreground{0.0f, 0.0f, 1.0f};
	Gdk::RGBA background{0.2f, 0.2f, 0.2f};
	Gdk::RGBA bottomPanelBackground{0.1f, 0.1f, 0.1f};

	double zoom{1};
	glm::dvec2 canvasOffset{};
	glm::dvec2 prevCanvasOffset{};
	std::set<CanvasElement*> elements;
	CanvasElement* selectedElement{};
	std::optional<size_t> selectedElementIndex{};
	CanvasElement* movingElement{};
	std::optional<size_t> movingElementIndex{};
	glm::dvec2 prevElementPos{};
	bool moving{};

public:
	CanvasWidget();

	/**
	 * @brief 
	 * @param pElement 
	 * @return element index in array
	 */
	size_t addElement(CanvasElement* pElement);

	sdk::utils::ReportMessagePtr removeElement(CanvasElement* pElement);

	void setSelectedElement(CanvasElement* pSelectedElement) {
		if (selectedElement == pSelectedElement) return;
		selectedElement = pSelectedElement;
		queue_draw();
	}

	[[nodiscard]] CanvasElement* getSelectedElement() const { return selectedElement; }

	[[nodiscard]] const std::optional<size_t> &getSelectedElementIndex() const { return selectedElementIndex; }

	[[nodiscard]] CanvasElement* getMovingElement() const { return movingElement; }

	[[nodiscard]] const std::optional<size_t> &getMovingElementIndex() const { return movingElementIndex; }

	[[nodiscard]] double getZoom() const { return zoom; }

	[[nodiscard]] const glm::dvec2 &getCanvasOffset() const { return canvasOffset; }

	uint32_t getCurrentDragButton() const;

	sigc::connection connectDragBegin(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter);

	sigc::connection connectDragUpdate(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter);

	sigc::connection connectDragEnd(const sigc::slot<void(double pX, double pY)> &pSlot, bool pAfter);


protected:
	void measure_vfunc(const Gtk::Orientation pOrientation, int /*pForSize*/, int &pMinimum, int &pNatural,
					   int &pMinimumBaseline, int &pNaturalBaseline) const override {
		if (pOrientation == Gtk::Orientation::HORIZONTAL) {
			pMinimum = 60;
			pNatural = 100;
		} else {
			pMinimum = 50;
			pNatural = 70;
		}
		pMinimumBaseline = -1;
		pNaturalBaseline = -1;
	}

	void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot> &pSnapshot) override;
	void drawBottomText(const std::shared_ptr<Cairo::Context> &pContext, const std::string &pText,
						Cairo::TextExtents &pExtents) const;
};

} // namespace mer::editor::mvp

// mvp

#endif //CANVASWIDGET_H
