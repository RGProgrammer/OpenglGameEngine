#version 330

in vec4 DiffuseColor ;
in vec4 NormalColor;
in vec4 SpecularColor ;


void main(){
	gl_FragData[0]=DiffuseColor;// diffuse buffer
	gl_FragData[1]=SpecularColor;// specular buffer
	gl_FragData[2]=NormalColor; // normal buffer 
}