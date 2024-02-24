//
// Created by alexus on 01.02.24.
//

#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H
#include <EngineSDK/renderer/shaders/Shader.h>

namespace mer::sdk::renderer {
class FragmentShader : public Shader {
public:
	FragmentShader() : Shader(ShaderTypeEnum::FRAGMENT_SHADER) {}
};
} // namespace mer::sdk::renderer

#endif //FRAGMENTSHADER_H
