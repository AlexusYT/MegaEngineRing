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
// Created by alexus on 26.02.25.
//

#include "EngineSDK/gltf/Sampler.h"

#include <epoxy/gl.h>

#include "GLTFSDK/GLTF.h"

namespace mer::sdk {
Sampler::Sampler(const Microsoft::glTF::Sampler &pSampler) {
	wrapS = static_cast<WrapMode>(pSampler.wrapS);
	wrapT = static_cast<WrapMode>(pSampler.wrapT);
	if (pSampler.magFilter.HasValue()) magFilter = static_cast<MagFilterMode>(pSampler.magFilter.Get());
	else
		magFilter = MagFilterMode::NEAREST;
	if (pSampler.minFilter.HasValue()) minFilter = static_cast<MinFilterMode>(pSampler.minFilter.Get());
	else
		minFilter = MinFilterMode::NEAREST;
}

std::shared_ptr<Sampler> Sampler::create(const Microsoft::glTF::Sampler &pSampler) {
	return std::shared_ptr<Sampler>(new Sampler(pSampler));
}

ReportMessagePtr Sampler::onInitialize() {
	glCreateSamplers(1, &samplerId);
	glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
	glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
	glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	return Initializable::onInitialize();
}

void Sampler::onUninitialize() {
	glDeleteSamplers(1, &samplerId);
}
} // namespace mer::sdk
