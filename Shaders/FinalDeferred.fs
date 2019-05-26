#version 430 


uniform sampler2D Specular ;
uniform sampler2D Diffuse ;
uniform sampler2D Normal ;
uniform sampler2D Shadow ;
uniform sampler2D LightDiffuse ;
uniform sampler2D LightSpecular ;
uniform sampler2D MaterialID ;
uniform sampler2D Transparency ;
uniform sampler2D Emissive ;
uniform int noLight ;
in vec2 texcoord0 ;
float factors[5]= float[](0.227027,0.1945946, 0.1216216,0.054054,0.016216);

void main(){
	
	vec3 DiffuseColor=texture2D(Diffuse,texcoord0).rgb;
	vec3 MaterialIDColor=texture2D(MaterialID,texcoord0).rgb;
	vec3 NormalColor = vec3(0.0);
	vec3 SpecularColor=vec3(0.0) ;
	vec3 shadowcolor=vec3(1.0);
	
	vec3 emissiveSample = vec3(0.0);
	vec2 TexelOffset= (1.0/textureSize(Emissive,0));
	vec3 EmissiveColor = texture2D(Emissive,texcoord0).rgb*factors[0];
	
	//post processing part 
	//bloom
	for(int i=-4 ; i<5 ; i++){
		for(int j=-4;j<5 ; j++){
			EmissiveColor+=
				texture2D(Emissive,texcoord0+vec2(TexelOffset.x*i,TexelOffset.y*j)).rgb* factors[abs(i)];	
		}
	}


	//generate the final color 
	if(noLight==0)
		if( MaterialIDColor.x!=1.0){
			DiffuseColor*=texture2D(LightDiffuse,texcoord0).rgb ;
			SpecularColor=texture2D(Specular,texcoord0).rgb*texture2D(LightSpecular,texcoord0).rgb ;
			shadowcolor=texture2D(Shadow,texcoord0).rgb;
		}
	gl_FragColor=vec4((DiffuseColor+SpecularColor)*shadowcolor+EmissiveColor,1.0);
			
}
	
