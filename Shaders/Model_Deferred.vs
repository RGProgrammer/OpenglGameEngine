#version 330

layout(location=0) in vec3 Position ;
layout(location=1) in vec3 Normal ;
layout(location=2) in vec2 Texcoord ;
layout(location=3) in vec3 Tangent ;
layout(location=4) in vec3 Bitangent ;


uniform mat4 WorldMtx ;
uniform mat4 ViewMtx ;
uniform mat4 ProjMtx ;

out mat3 TBN ;
out vec2 Texcoord0 ;


void main(){
	vec3 T = normalize(vec3(WorldMtx * vec4(Tangent,   0.0)));
  	vec3 B = normalize(vec3(WorldMtx * vec4(Bitangent, 0.0)));
   	vec3 N = normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
  	mat3 TBN = mat3(T, B, N);
	Texcoord0=Texcoord ;
	gl_Position=ProjMtx*ViewMtx*WorldMtx*vec4(Position,1.0);
}