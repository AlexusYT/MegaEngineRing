//uniform float gamma;
float gamma = 2.2;
vec3 _gammaCoef_ = vec3(1.0/gamma);
vec4 applyGamma(in vec4 color){
	return pow(color, vec4(_gammaCoef_, 1));
}
vec3 applyGamma(in vec3 color){
	return pow(color, _gammaCoef_);
}
void applyGamma(out vec4 colorOut, in vec4 colorIn){
	colorOut = pow(colorIn, vec4(_gammaCoef_, 1));
}
void applyGamma(out vec3 colorOut, in vec3 colorIn){
	colorOut = pow(colorIn, _gammaCoef_);
}