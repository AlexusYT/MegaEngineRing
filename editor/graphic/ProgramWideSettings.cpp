//
// Created by alexus on 12.07.23.
//
#if 0

	#include "ProgramWideSettings.h"

	#include <Render/opengl/buffers/SSBO.h>

ProgramWideSettings::ProgramWideSettings() {}

bool ProgramWideSettings::init() {
	ssbo = rel::opengl::SSBO::Builder().setBufferData(sizeof(data), &data).setUsage(rel::opengl::DYNAMIC_DRAW).build();
	if (!ssbo) {
		std::cerr << "Not enough memory to allocate a new instance of the SSBO" << std::endl;
		return false;
	}
	return true;
}

void ProgramWideSettings::update() {
	if (raw) {
		raw = false;
		ssbo->bufferSubDataBind(0, sizeof(data), &data);
	}
}

#endif
