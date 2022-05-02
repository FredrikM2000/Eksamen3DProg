#version 410 core

in vec4 color;              //color received from the pipeline (and vertex shader)
out vec4 fragmentColor;     //color sent to fragment on screen

//uniform float ambientStrength = 0.8;
//uniform vec4 ambientColor = vec4(1, 1, 1, 1);

void main() {
   fragmentColor = color/* * ambientColor * ambientStrength*/;   //color given to current fragment (pixel)
   //fragmentColor = vec4(1,0,0,0);
}
