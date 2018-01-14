#version 410 

uniform sampler2D Specular ;
uniform sampler2D Diffuse ;
uniform sampler2D Shadow ;
uniform sampler2D LightDiffuse ;
uniform sampler2D LightSpecular ;



in vec2 texcoord0 ;


void main(){
	vec3 FinalColor=vec3(0.0);
	vec3 DiffuseColor=texture2D(Diffuse,texcoord0).rgb*texture2D(LightDiffuse,texcoord0).rgb ;
	vec3 SpecularColor=texture2D(Specular,texcoord0).rgb*texture2D(LightSpecular,texcoord0).rgb ;
	
	FinalColor=(DiffuseColor+SpecularColor)*texture2D(Shadow,texcoord0).rgb ;
	gl_FragColor=vec4(FinalColor ,1.0);
}
