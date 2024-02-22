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
