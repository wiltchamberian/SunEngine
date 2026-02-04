#version 430 core

layout (location = 0) out vec4 FragColor;
uniform sampler2D output_image;

in vec2 TexCoord;

void main()
{
   //FragColor = texture(output_image, vec2(gl_FragCoord.xy) / vec2(textureSize(output_image, 0)));

   FragColor = texture(output_image, TexCoord);
};