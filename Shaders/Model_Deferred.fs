#version 330

layout(location=0) out vec4 DiffuseColor ;
layout(location=1) out vec4 SpecularColor ;
layout(location=2) out vec4 NormalColor ;


in mat3	TBN;
in vec2 Texcoord0 ;

uniform sampler2D	DiffuseTexture ;
uniform sampler2D	NormalTexture ;
uniform sampler2D	SpecularTexture ;


void main(){
	vec3 normalcolor0 ;
	normalcolor0 = texture2D(NormalTexture, Texcoord0).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	NormalColor=vec4(normalcolor0,0.0);
	DiffuseColor=vec4(texture2D(DiffuseTexture,Texcoord0).rgb,1.0);
	SpecularColor=texture2D(SpecularTexture,Texcoord0);
}
