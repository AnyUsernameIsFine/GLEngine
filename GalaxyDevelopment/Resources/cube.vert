#version 430 core

layout(location = 0) in vec4 vertPosition;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertTexCoords;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = view * model * vertPosition;

	fragPosition = vec3(view * model * vertPosition);
	fragNormal = mat3(transpose(inverse(view * model))) * vertNormal;
	fragTexCoords = vertTexCoords;

	gl_Position = projection * gl_Position;
}
