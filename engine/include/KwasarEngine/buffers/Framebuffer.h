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
// Created by alexus on 27.02.25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "KwasarEngine/render/Initializable.h"

namespace ke {
class Framebuffer : public Initializable {
	uint32_t fboMs{};
	uint32_t textureMs{};
	uint32_t rboMs{};
	uint32_t fbo{};
	uint32_t texture{};
	uint32_t rbo{};
	int width{};
	int height{};
	int samples{4};

public:
	Framebuffer();

	~Framebuffer() override;

	uint32_t getFrameTexture() const;

	void setSize(int pWidth, int pHeight);

	[[nodiscard]] int getWidth() const { return width; }

	[[nodiscard]] int getHeight() const { return height; }

	void bind() const;

	void unbind() const;

	bool isComplete() const;

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};
} // namespace ke

#endif //FRAMEBUFFER_H