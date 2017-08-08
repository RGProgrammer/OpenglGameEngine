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

out vec4 DiffuseColor ;
out vec4 NormalColor;
out vec4 SpecularColor ;
out vec4 MaterialColor ;
out vec4 PositionColor ;

void main(){
	vec4 D = texture2D(Diffusemap,Texcoord);
	vec3 T = normalize(vec3(WorldMtx * vec4(Tangent,   0.0)));
  	vec3 B = normalize(vec3(WorldMtx * vec4(Bitangent, 0.0)));
   	vec3 N= normalize(vec3(WorldMtx * vec4(Normal,    0.0)));
  	mat3 TBN = mat3(T, B, N);
  	vec3 normalcolor0 ;
	normalcolor0 = texture2D(Normalmap, Texcoord).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);

	DiffuseColor=D;

	SpecularColor=texture2D(Specularmap,Texcoord);

	NormalColor=ViewMtx*vec4(normalize(normalcolor0),0.0);
	
	MaterialColor=vec4(D.a,D.a,D.a,1.0);

	PositionColor=WorldMtx*vec4(Position,1.0);

	gl_Position=ProjMtx*ViewMtx*WorldMtx*vec4(Position,1.0);
}