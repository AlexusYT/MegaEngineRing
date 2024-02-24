//
// Created by alexus on 31.01.24.
//

#ifndef SHADERTYPEENUM_H
#define SHADERTYPEENUM_H

namespace mer::sdk::renderer {
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
}


#endif //SHADERTYPEENUM_H
