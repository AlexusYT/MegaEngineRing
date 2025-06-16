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

#ifndef SAMPLER_H
#define SAMPLER_H
#include "KwasarEngine/render/Initializable.h"

namespace Microsoft::glTF {
struct Sampler;
}

namespace mer::sdk {
enum class MagFilterMode {
	NEAREST = 9728,
	LINEAR  = 9729,
};

enum class MinFilterMode {
	NEAREST                = 9728,
	LINEAR                 = 9729,
	NEAREST_MIPMAP_NEAREST = 9984,
	LINEAR_MIPMAP_NEAREST  = 9985,
	NEAREST_MIPMAP_LINEAR  = 9986,
	LINEAR_MIPMAP_LINEAR   = 9987
};

enum class WrapMode { REPEAT = 10497, CLAMP_TO_EDGE = 33071, MIRRORED_REPEAT = 33648 };

class Sampler : public Initializable {
	uint32_t samplerId{};
	MagFilterMode magFilter;
	MinFilterMode minFilter;
	WrapMode wrapS;
	WrapMode wrapT;

	explicit Sampler(const Microsoft::glTF::Sampler &pSampler);

public:
	static std::shared_ptr<Sampler> create(const Microsoft::glTF::Sampler &pSampler);

	[[nodiscard]] uint32_t getSamplerId() const { return samplerId; }

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};
} // namespace mer::sdk

#endif //SAMPLER_H
