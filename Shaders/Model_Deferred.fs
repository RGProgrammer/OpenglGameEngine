#version 410

in vec4 DiffuseColor ;
in vec4 NormalColor;
in vec4 SpecularColor ;
in vec4 MaterialColor ;
in vec4 PositionColor ;

void main(){
	gl_FragData[0]=DiffuseColor;// diffuse buffer
	gl_FragData[1]=SpecularColor;// specular buffer
	gl_FragData[2]=NormalColor; // normal buffer 
	gl_FragData[3]=MaterialColor ;//Material Property
	gl_FragData[4]=PositionColor; // Position buffer
}