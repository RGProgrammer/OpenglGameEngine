#version 410

//in vec4 DiffuseColor ;
//in vec4 NormalColor;
//in vec4 SpecularColor ;
//in vec4 MaterialColor ;
in vec4 PositionColor ;

uniform sampler2D Diffusemap ;
uniform sampler2D Specularmap ;
uniform sampler2D Normalmap ;
uniform mat4 ViewMtx ;
in  mat3 TBN ;
in vec2 texcoord0;



void main(){
	//gl_FragData[0]=DiffuseColor;// diffuse buffer
	//gl_FragData[1]=SpecularColor;// specular buffer
	//gl_FragData[2]=NormalColor; // normal buffer 
	//gl_FragData[3]=MaterialColor ;//Material Property
	gl_FragData[4]=PositionColor; // Position buffer

	gl_FragData[0]=texture2D(Diffusemap,texcoord0);
	gl_FragData[1]=texture2D(Specularmap,texcoord0);
	vec3 normalcolor0 ;
	normalcolor0 = texture2D(Normalmap, texcoord0).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	gl_FragData[2]=ViewMtx*vec4(normalize(normalcolor0),0.0);
	gl_FragData[3]=vec4(0.0);

}