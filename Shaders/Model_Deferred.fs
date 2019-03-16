#version 430
#extension GL_ARB_bindless_texture : require
uniform sampler2D DiffuseMap ;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap ;
uniform float IOR ;
uniform float Opacity ;

in vec4 PositionColor ;
in  mat3 TBN ;
in vec2 texcoord0;

void main(){
	vec3 normalcolor0 ;

	normalcolor0 = texture2D(NormalMap, texcoord0).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	vec4 basecolor=texture2D(DiffuseMap,texcoord0);
	
	//if(Opacity == 1.0)
		gl_FragData[0]=basecolor ;  // Duffise 1 (Solid Colors)			
//	else
		gl_FragData[1]=basecolor	;	// Diffuse 2 ( Transparent Color)

		
	gl_FragData[2]= vec4(texture2D(SpecularMap,texcoord0).rgb,1.0);	// Specular 
	gl_FragData[3]= vec4(normalcolor0,1.0);							// world normal
	gl_FragData[4]= vec4(0.0,0.0,0.0,1.0); // Material ID
	gl_FragData[5]= vec4(PositionColor.rgb,1.0);						// World Postion
	gl_FragData[6]= vec4(0.0,0.0,0.0,1.0); // Transprency

}