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
// Created by alexus on 31.01.24.
//

#ifndef SHADERTYPEENUM_H
#define SHADERTYPEENUM_H

namespace ke {
enum class ShaderTypeEnum {
	/// Shader that is intended to run on the programmable compute processor.
	COMPUTE_SHADER,
	/// Intended to run on the programmable vertex processor.
	VERTEX_SHADER,
	/// Intended to run on the programmable tessellation processor in the control stage.
	TESS_CONTROL_SHADER,
	/// Intended to run on the programmable tessellation processor in the evaluation stage.
	TESS_EVALUATION_SHADER,
	/// Intended to run on the programmable geometry processor.
	GEOMETRY_SHADER,
	/// Intended to run on the programmable fragment processor.
	FRAGMENT_SHADER,

	MAX_SHADER_TYPE
};

inline const char* to_string(ShaderTypeEnum pE) {
	switch (pE) {
		case ShaderTypeEnum::COMPUTE_SHADER: return "COMPUTE_SHADER";
		case ShaderTypeEnum::VERTEX_SHADER: return "VERTEX_SHADER";
		case ShaderTypeEnum::TESS_CONTROL_SHADER: return "TESS_CONTROL_SHADER";
		case ShaderTypeEnum::TESS_EVALUATION_SHADER: return "TESS_EVALUATION_SHADER";
		case ShaderTypeEnum::GEOMETRY_SHADER: return "GEOMETRY_SHADER";
		case ShaderTypeEnum::FRAGMENT_SHADER: return "FRAGMENT_SHADER";
		case ShaderTypeEnum::MAX_SHADER_TYPE: return "MAX_SHADER_TYPE";
		default: return "unknown";
	}
}
} // namespace ke


#endif //SHADERTYPEENUM_H
