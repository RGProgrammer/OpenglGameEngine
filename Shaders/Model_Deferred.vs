#version 410
layout(location=0) in vec3 Position ;
layout(location=1) in vec3 Normal ;
layout(location=2) in vec2 Texcoord ;
layout(location=3) in vec3 Tangent ;
layout(location=4) in vec3 Bitangent ;

uniform mat4 WorldMtx ;
uniform mat4 ViewMtx ;
uniform mat4 ProjMtx ;

out vec4 PositionColor ;
out mat3 TBN ;
out vec2 texcoord0;
out mat4 ViewMtx0;

void main(){
	vec3 T = normalize(vec3(WorldMtx * vec4(Tangent,   0.0)));
	vec3 B = normalize(vec3(WorldMtx * vec4(Bitangent, 0.0)));
   	vec3 N= normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
	TBN = mat3(T, B, N);

	texcoord0=Texcoord ;

	PositionColor=WorldMtx*vec4(Position,1.0);
	ViewMtx0= ViewMtx ;
	gl_Position=ProjMtx*ViewMtx*WorldMtx*vec4(Position,1.0);

}