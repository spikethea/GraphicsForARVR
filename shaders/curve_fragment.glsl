#version 450 core

uniform vec4 uColor;

out vec4 FinalColor;

void main()
{
	FinalColor = uColor;
}