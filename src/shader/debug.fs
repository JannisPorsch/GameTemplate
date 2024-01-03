#version 460 core
out vec4 FragColor;


in vec2 pos;


void main()
{
    if ((((pos.x - 0.95) * (pos.x - 0.95)) + ((pos.y - 0.95) * (pos.y - 0.95))) > 0.002) discard;
    FragColor = vec4(1, 0, 0, 1);
}
