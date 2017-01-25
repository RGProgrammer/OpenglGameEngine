#version 330

layout(location=0)in vec3 position ;
layout(location=1)in vec2 texcoord ;

uniform mat4 ProjMtx ;

varying vec2 texcoord0 ;
void main(){
	texcoord0=texcoord ;
	gl_Position=ProjMtx*vec4(position,1.0) ;
}