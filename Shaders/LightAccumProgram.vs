#version 410 

layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;

uniform mat4 CameraViewMtx;
uniform	vec3 CameraPos ;

out vec2 texcoord0;
out vec2 texcoord1 ;
out vec3 CameraEyeSpacePos ;

void main(){
	texcoord0=texcoord;
	texcoord1=texcoord;
	texcoord1.y=1.0-texcoord.y;
	CameraEyeSpacePos=(CameraViewMtx*vec4(CameraPos,1.0)).xyz;
	gl_Position=vec4(position,0.0,1.0);
}