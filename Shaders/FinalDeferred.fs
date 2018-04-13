#version 410 

uniform sampler2D Specular ;
uniform sampler2D Diffuse ;
uniform sampler2D Shadow ;
uniform sampler2D LightDiffuse ;
uniform sampler2D LightSpecular ;
uniform int noLight ;

in vec2 texcoord0 ;


void main(){
	float alpha=1.0 ;
	vec3 FinalColor=vec3(0.0);
	vec3 DiffuseColor=vec3(1.0);
	vec3 SpecularColor=vec3(0.0) ;
	vec3 shadowcolor=vec3(1.0);
	if(noLight==0){
		alpha=texture2D(Diffuse,texcoord0).a ;
		DiffuseColor=texture2D(Diffuse,texcoord0).rgb*texture2D(LightDiffuse,texcoord0).rgb ;
		SpecularColor=texture2D(Specular,texcoord0).rgb*texture2D(LightSpecular,texcoord0).rgb ;
		shadowcolor=texture2D(Shadow,texcoord0).rgb;
		
	}else{
		DiffuseColor=texture2D(Diffuse,texcoord0).rgb ;
		alpha=texture2D(Diffuse,texcoord0).a ;
	}

	FinalColor=(DiffuseColor+SpecularColor)*shadowcolor ;
	gl_FragColor=vec4(FinalColor ,alpha);
}
