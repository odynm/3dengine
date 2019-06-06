#version 330 core
out vec4 FragColor;

uniform vec4 color;
uniform vec2 textureCoords[2];

uniform sampler2D _texture;

flat in int InstanceID;
in vec2 texCoord;

void main()
{
    FragColor = texture(_texture, texCoord);
}