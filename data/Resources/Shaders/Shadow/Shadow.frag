#version 430 core

in vec3 FragPos;
in vec3 LightPos;
void main(){
	gl_FragDepth = length(FragPos - LightPos) / 25.0f;
	//gl_FragDepth = depth;
}