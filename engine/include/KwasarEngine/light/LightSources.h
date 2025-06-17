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
// Created by alexus on 24.10.24.
//

#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H
#include "ILightSources.h"
#include "LightInstanceData.h"

namespace ke {
class LightSources : public ILightSources {
	std::vector<std::shared_ptr<ILightInstance>> lights{};
	std::vector<LightInstanceData> lightsData{};
	std::shared_ptr<SSBO> lightSsbo{};
	bool dirty{true};

	LightSources();

public:
	static std::shared_ptr<LightSources> create();

	[[nodiscard]] const std::shared_ptr<SSBO> &getLightSsbo() const override { return lightSsbo; }

	void updateSsbo() override;

	void onInstanceDataChanged(ILightInstance* pInstance) override;

	void addLightInstance(const std::shared_ptr<ILightInstance> &pLightInstance) override;

	void removeLightInstance(const std::shared_ptr<ILightInstance> &pLightInstance) override;
};
} // namespace ke

#endif //LIGHTSOURCES_H
