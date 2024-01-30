#version 430

const int VIEW_RENDER_TEXT = 1;
const int VIEW_RENDER_GRADIENT  = 2;
const int VIEW_RENDER_COLORED_TEXTURE = 3;

layout(std430, binding = 1) buffer ModelWideSettings {
	mat4 modelViewMatrix;
	vec4 viewPosStart;
	vec4 viewPosEnd;
};
layout(origin_upper_left) in vec4 gl_FragCoord;

uniform sampler2D text;

in vec2 uvVS;
in vec4 colorVS;
flat in int typeVS;

out vec4 colorOut;

void main() {
	if (gl_FragCoord.x <= viewPosStart.x || gl_FragCoord.x >= viewPosEnd.x ||
	gl_FragCoord.y <= viewPosStart.y || gl_FragCoord.y >= viewPosEnd.y){
		discard;
	}
	switch (typeVS){
		case VIEW_RENDER_TEXT:{
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, uvVS).r);
			colorOut = colorVS * sampled;
			break;
		}
		case VIEW_RENDER_COLORED_TEXTURE:{
			vec4 sampled = texture(text, uvVS);
			colorOut = colorVS *(colorVS.a) +  sampled*(1.0-colorVS.a);
			break;
		}
		default :{
			colorOut = colorVS;
		}

	}
}