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

#include "EngineSDK/buffers/Framebuffer.h"

#include <epoxy/gl.h>

namespace mer::sdk {
Framebuffer::Framebuffer() {}

Framebuffer::~Framebuffer() { uninitialize(); }

uint32_t Framebuffer::getFrameTexture() const { return texture; }

void Framebuffer::setSize(const int pWidth, const int pHeight) {
	width = pWidth;
	height = pHeight;
	if (textureMs)
		glDeleteTextures(1, &textureMs);
	glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &textureMs);
	glTextureStorage2DMultisample(textureMs, samples, GL_RGB8, pWidth, pHeight, true);
	glNamedFramebufferTexture(fboMs, GL_COLOR_ATTACHMENT0, textureMs, 0);

	glNamedRenderbufferStorageMultisample(rboMs, samples, GL_DEPTH24_STENCIL8, pWidth, pHeight);
	glNamedFramebufferRenderbuffer(fboMs, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMs);


	if (texture)
		glDeleteTextures(1, &texture);
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGB8, pWidth, pHeight);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, texture, 0);

	glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, pWidth, pHeight);
	glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, fboMs);
	glViewport(0, 0, width, height);
}

void Framebuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBlitNamedFramebuffer(fboMs, fbo, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

bool Framebuffer::isComplete() const {
	return glCheckNamedFramebufferStatus(fboMs, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

ReportMessagePtr Framebuffer::onInitialize() {
	glCreateFramebuffers(1, &fboMs);
	glCreateRenderbuffers(1, &rboMs);
	glCreateFramebuffers(1, &fbo);
	glCreateRenderbuffers(1, &rbo);
	//if (glCheckNamedFramebufferStatus(GL_FRAMEBUFFER, fbo) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	return Initializable::onInitialize();
}

void Framebuffer::onUninitialize() {
	if (fboMs)
		glDeleteFramebuffers(1, &fboMs);
	if (fbo)
		glDeleteFramebuffers(1, &fbo);
	if (textureMs)
		glDeleteTextures(1, &textureMs);
	if (texture)
		glDeleteTextures(1, &texture);
	if (rboMs)
		glDeleteRenderbuffers(1, &rboMs);
	if (rbo)
		glDeleteRenderbuffers(1, &rbo);
}
} // namespace mer::sdk
