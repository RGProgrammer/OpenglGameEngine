#version 410

in vec4 DiffuseColor ;
in vec4 NormalColor;
in vec4 SpecularColor ;
in vec4 PositionColor;

void main(){
	gl_FragData[0]=DiffuseColor;// diffuse buffer
	gl_FragData[1]=SpecularColor;// specular buffer
	gl_FragData[2]=PositionColor ; // Position Color 
	gl_FragData[3]=NormalColor; // normal buffer 
}
