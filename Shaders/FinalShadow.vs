#version 330 

layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;

out vec2 texcoord0;


void main(){
	texcoord0=texcoord;
	texcoord0.y=1.0-texcoord0.y;
	gl_Position=vec4(position,0.0,1.0);
}