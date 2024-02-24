//
// Created by alexus on 01.02.24.
//

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H
#include <EngineSDK/renderer/shaders/Shader.h>

namespace mer::sdk::renderer {
class VertexShader : public Shader {
public:
	VertexShader() : Shader(ShaderTypeEnum::VERTEX_SHADER) {}
};
} // namespace mer::sdk::renderer


#endif //VERTEXSHADER_H
