#version 130

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform vec2 u_boundaries;
uniform int u_zoom;
uniform sampler2D u_colorPalette;

const float xOffsetMax = 1.75;
const float yOffsetMax = 2.5;
const float xOffsetMin = 0.1;
const float yOffsetMin = 0.1;
vec2 boundaries;
vec2 center;

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

vec3 coloringPixel(vec2 coord) {
	coord.x = map(coord.x, -1.0, 1.0, center.x - boundaries.x, center.x + boundaries.x);
	coord.y = map(coord.y, -1.0, 1.0, center.y - boundaries.y, center.y + boundaries.y);
	float x = 0.0;
	float y = 0.0;

	int iteration = 0;
	int maxIteration = 100 * u_zoom;

	float p = sqrt(pow(coord.x - 1/4, 2) + coord.y*coord.y);
	float phi = atan(coord.y, coord.x - 1/4);
	float pc = 1/2 - 1/2 * cos(phi);

	if (p <= pc) {
		iteration = maxIteration;
	}

	while (x*x + y*y <= 2*2 && iteration < maxIteration) {
		float tempX = x*x - y*y + coord.x;
		y = 2*x*y + coord.y;
		x = tempX;
		iteration++;
	}

	float colorX = map(iteration, 0, maxIteration, 0.0, 2.0);

	return texture(u_colorPalette, vec2(colorX, 1.0)).rgb;
}

void main() {
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;

	center = u_center;
	boundaries = u_boundaries;

	// center = vec2 (-0.651631, -0.476737);
	// boundaries = vec2 (1.59852e-05, 2.2836e-05);	

	//gl_FragColor = (uv.x > -0.001 && uv.x < 0.001 && uv.y > -0.001 && uv.y < 0.001) ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(coloringPixel(uv), 1.0);
	gl_FragColor = vec4(coloringPixel(uv), 1.0);
}