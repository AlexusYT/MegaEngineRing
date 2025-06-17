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

#ifndef TEXTURE_H
#define TEXTURE_H
#include "KwasarEngine/render/Initializable.h"

namespace ke {
class Sampler;
}

namespace Microsoft::glTF {
struct Texture;
}

namespace ke {
class Image;

class Texture : public Initializable {
	std::shared_ptr<Image> image;
	uint32_t textureId{};
	std::shared_ptr<Sampler> sampler;
	uint64_t bindlessHandle{};

	explicit Texture(const Microsoft::glTF::Texture &pTexture, const std::shared_ptr<Image> &pImage,
					 const std::shared_ptr<Sampler> &pSampler);

public:
	static std::shared_ptr<Texture> create(const Microsoft::glTF::Texture &pTexture,
										   const std::shared_ptr<Image> &pImage,
										   const std::shared_ptr<Sampler> &pSampler);

	[[nodiscard]] uint64_t getBindlessHandle() const { return bindlessHandle; }

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};
} // namespace ke

#endif //TEXTURE_H
