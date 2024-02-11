//
// Created by alexus on 01.02.24.
//

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H
#include <EngineSDK/renderer/shaders/Shader.h>

namespace n::sdk::renderer {
class VertexShader : public Shader {
public:
	VertexShader() : Shader(ShaderTypeEnum::VERTEX_SHADER) {}
};
} // namespace n::sdk::renderer


#endif //VERTEXSHADER_H
