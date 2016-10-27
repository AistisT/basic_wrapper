// Minimal fragment shader

#version 400

in vec4 fcolour;

uniform uint emitmode;

in vec3 fnormal, flightdir, fposition;
in vec4 fdiffusecolour, fambientcolour;


out vec4 outputColor;
void main()
{
	 diffuse = max(dot(N, L), 0.0) * fdiffusecolour;

	outputColor = fcolour;
}