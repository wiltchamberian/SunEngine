#version 430 core
out vec4 FragColor;

in vec4 Color0;
in vec2 TexCoord0;
uniform sampler2D ourTexture;

void main()
{
   FragColor = Color0;
};