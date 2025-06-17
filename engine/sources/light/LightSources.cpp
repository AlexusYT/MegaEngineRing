//  KwasarEngine is an SDK that can help you speed up game development.
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

#include "KwasarEngine/light/LightSources.h"

#include "KwasarEngine/buffers/SSBO.h"
#include "KwasarEngine/light/ILightInstance.h"

namespace ke {
LightSources::LightSources() : lightSsbo(std::make_shared<SSBO>()) { lightSsbo->setUsage(DYNAMIC_DRAW); }

std::shared_ptr<LightSources> LightSources::create() { return std::shared_ptr < LightSources > (new LightSources()); }

void LightSources::updateSsbo() {
	if (!dirty) return;
	const int64_t bufSize = static_cast<int64_t>(lightsData.size() * sizeof(LightInstanceData));
	if (lightSsbo->getSize() < bufSize) { lightSsbo->reallocate(bufSize, lightsData.data()); } else
		lightSsbo->bufferSubData(0, bufSize, lightsData.data());
	dirty = false;
}

void LightSources::onInstanceDataChanged(ILightInstance* /*pInstance*/) {
	lightsData.clear();
	for (auto lightInstance: lights) { lightsData.emplace_back(lightInstance->getLightInstanceData()); }
	dirty = true;
}

void LightSources::addLightInstance(const std::shared_ptr<ILightInstance> &pLightInstance) {
	lights.emplace_back(pLightInstance);
	pLightInstance->setLightSources(this);
	onInstanceDataChanged(pLightInstance.get());
}

void LightSources::removeLightInstance(const std::shared_ptr<ILightInstance> &pLightInstance) {
	pLightInstance->setLightSources(nullptr);
	erase(lights, pLightInstance);
	onInstanceDataChanged(pLightInstance.get());
}
} // namespace ke