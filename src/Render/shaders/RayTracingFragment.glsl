#version 430 core


uniform sampler2D output_image;
//out vec4 FragColor;
in vec2 TexCoord;

void main()
{
   //FragColor = texture(output_image, vec2(gl_FragCoord.xy) / vec2(textureSize(output_image, 0)));

   //FragColor = texture(output_image, TexCoord);
   gl_FragColor = texture(output_image,TexCoord);
};