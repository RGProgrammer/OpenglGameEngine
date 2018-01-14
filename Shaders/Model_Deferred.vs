#version 410
layout(location=0) in vec3 Position ;
layout(location=1) in vec3 Normal ;
layout(location=2) in vec2 Texcoord ;
layout(location=3) in vec3 Tangent ;
layout(location=4) in vec3 Bitangent ;

uniform sampler2D Diffusemap ;
uniform sampler2D Specularmap ;
uniform sampler2D Normalmap ;
uniform mat4 WorldMtx ;
uniform mat4 ViewMtx ;
uniform mat4 ProjMtx ;

out vec4 PositionColor ;
out vec3 Normal0 ;

out mat3 TBN ;
out vec2 texcoord0;


void main(){
	vec3 T = normalize(vec3(WorldMtx * vec4(Tangent,   0.0)));
	vec3 B = normalize(vec3(WorldMtx * vec4(Bitangent, 0.0)));
   	vec3 N= normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
   	Normal0=normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
	TBN = mat3(T, B, N);

	texcoord0=Texcoord ;

	PositionColor=WorldMtx*vec4(Position,1.0);

	gl_Position=ProjMtx*ViewMtx*WorldMtx*vec4(Position,1.0);

}