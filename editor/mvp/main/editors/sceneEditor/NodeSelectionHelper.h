//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 10.04.25.
//

#ifndef NODESELECTIONHELPER_H
#define NODESELECTIONHELPER_H
#include <vector>

namespace mer::sdk {
class Node;
}

namespace mer::editor::mvp {

class NodeSelectionHelper {
	std::vector<sdk::Node*> selectedNodes{};
	sigc::signal<void(const std::vector<sdk::Node*> &pNodes, bool pSelected)> onNodeSelectionChanged;

public:
	void addNode(sdk::Node* pNode) {
		selectedNodes.push_back(pNode);
		onNodeSelectionChanged(std::vector{pNode}, true);
	}

	void addNodes(const std::vector<sdk::Node*> &pNodes) {
		selectedNodes.insert(selectedNodes.cend(), pNodes.begin(), pNodes.end());
		onNodeSelectionChanged(pNodes, true);
	}

	void clearSelection() {
		onNodeSelectionChanged(selectedNodes, false);
		selectedNodes.clear();
	}

	[[nodiscard]] const std::vector<sdk::Node*> &getSelectedNodes() const { return selectedNodes; }

	sigc::connection connectOnNodeSelectionChanged(
		const sigc::slot<void(const std::vector<sdk::Node*> &pNodes, bool pSelected)> &pSlot) {
		return onNodeSelectionChanged.connect(pSlot);
	}

	}
};
} // namespace mer::editor::mvp

#endif //NODESELECTIONHELPER_H
