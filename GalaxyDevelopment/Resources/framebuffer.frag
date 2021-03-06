#version 430 core

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D texture1;

void main()
{
	color = texture(texture1, fragTexCoords);
}
