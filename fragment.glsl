#version 460 core
out vec4 FragColor;
in vec3 vertexOutColor;
void main()
{
   FragColor = vec4(vertexOutColor, 1.0); 
}