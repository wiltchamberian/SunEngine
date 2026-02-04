#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec3 TexCoord_cube;
uniform samplerCube skybox;

void main()
{
   FragColor = texture(skybox,TexCoord_cube);
};