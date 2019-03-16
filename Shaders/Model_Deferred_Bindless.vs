#version 430


layout(location=0) in vec3 Position ;
layout(location=1) in vec3 Normal ;
layout(location=2) in vec2 Texcoord ;
layout(location=3) in vec3 Tangent ;
layout(location=4) in vec3 Bitangent ;


layout(std140, binding=0) uniform TransformsMtx {
	mat4 ViewMtx;
	mat4 ProjMtx ;
};


uniform mat4 WorldMtx ;
out mat3 TBN ;
out vec4 PositionColor ;
out vec2 texcoord0;
out vec3 normal ;

void main(){

	vec3 T = normalize(vec3(WorldMtx * vec4(Tangent,   0.0)));
	vec3 B = normalize(vec3(WorldMtx * vec4(Bitangent, 0.0)));
   	vec3 N= normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
	normal=N ;
	TBN = mat3(T, B, N);
	texcoord0=Texcoord.xy;
	PositionColor=WorldMtx*vec4(Position,1.0);
	gl_Position=ProjMtx*ViewMtx*WorldMtx*vec4(Position,1.0);

}