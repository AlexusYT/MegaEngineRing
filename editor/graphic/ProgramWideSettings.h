// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 12.07.23.
//

#ifndef MAIN_PROGRAMWIDESETTINGS_H
#define MAIN_PROGRAMWIDESETTINGS_H
#if 0

	#include <Render/opengl/buffers/SSBOFwd.h>
	#include <Render/opengl/shaders/ShaderProgramFwd.h>

	#include "ProgramWideSettingsFwd.h"

class ProgramWideSettingsData {
	friend class ProgramWideSettings;
	glm::mat4 viewProjMatrix;


public:
	[[nodiscard]] const glm::mat4 &getViewProjMatrix() const { return viewProjMatrix; }

	void setViewProjMatrix(const glm::mat4 &newViewProjMatrix) {
		ProgramWideSettingsData::viewProjMatrix = newViewProjMatrix;
	}
};

class ProgramWideSettings {
	rel::opengl::SSBOSPtr ssbo;
	bool raw = true;

	ProgramWideSettingsData data{};

private:
	ProgramWideSettings();

public:
	static ProgramWideSettingsUPtr create() {
		return ProgramWideSettingsUPtr(new (std::nothrow) ProgramWideSettings());
	}

	bool init();

	void setViewProjMatrix(const glm::mat4 &matrix) {
		if (data.viewProjMatrix == matrix) return;
		data.viewProjMatrix = matrix;
		raw = true;
	}

	void invalidate() { raw = true; }

	[[nodiscard]] const rel::opengl::SSBOSPtr &getSSBO() const { return ssbo; }

	const ProgramWideSettingsData &getData() const { return data; }

	void update();
};

#endif


#endif //MAIN_PROGRAMWIDESETTINGS_H
