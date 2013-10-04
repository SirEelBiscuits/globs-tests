#version 150

in vec4 colour_from_vshader;
in vec2 texture_from_vshader;
out vec4 out_colour;

uniform sampler2D diffuseTex;

void main() {
	out_colour = colour_from_vshader * texture(diffuseTex, texture_from_vshader);
}

