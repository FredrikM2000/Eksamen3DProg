#version 410 core

layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 colorIn;       // 2nd attribute buffer = colors
out vec4 color;                             //color sent to rest of pipeline
uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;                        //the matrix for the model

void main() {
   color = colorIn;                         //passing on the vertex color
   //color = vec4(0,0,1,0);
   //gl_Position = matrix * positionIn;       //calculate the position of the model
   gl_Position = pMatrix * vMatrix * mMatrix * positionIn;
}
