#version 330

layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;
uniform mat4 ViewMtx;
uniform mat4 ProjMtx ;
uniform vec3 CameraPos;

out vec2 texcoord0 ;



void main(){
	texcoord0=texcoord ;
	gl_Position=vec4(position,0.0,1.0) ;
}