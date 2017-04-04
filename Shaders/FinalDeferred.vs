#version 330

layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;
uniform mat4 ViewMtx;
uniform mat4 ProjMtx ;
uniform vec3 CameraPos;

out vec2 texcoord0 ;
out vec3 CameraDir;
out vec3 EyeDirection ;


void main(){
	vec2 halfSizeNearPlane=vec2(1.0/ProjMtx[0][0],1.0/ProjMtx[1][1]);
	texcoord0=texcoord ;
	CameraDir = vec3(ViewMtx[0][2],ViewMtx[1][2],ViewMtx[2][2]) ;
	CameraDir*=(-1);
	EyeDirection = vec3((2.0 * halfSizeNearPlane * texcoord) - halfSizeNearPlane , -1.0);
	gl_Position=vec4(position,0.0,1.0) ;
}