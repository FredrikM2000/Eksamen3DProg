#version 410 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 normalTransposed;

//out vec4 color;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main(void)
{
    fragmentPosition = vec3(mMatrix * vec4(vertexPosition, 1.0));
    normalTransposed = vertexNormal;

//    color = vec4(vertexNormal,1.0);
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(vertexPosition, 1.0);
}
