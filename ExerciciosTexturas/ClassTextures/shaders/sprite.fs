#version 450

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex_buffer0;
uniform vec2 offsets;
void main()
{
    color = texture(tex_buffer0, texCoord+offsets);
}