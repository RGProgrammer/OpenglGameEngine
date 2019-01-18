#version 410

uniform sampler2D Diffusemap ;
uniform sampler2D Specularmap ;
uniform sampler2D Normalmap ;
uniform samplerCube ReflectMap ;
uniform int 	hasReflectMap ;
uniform float 	Opacity ;
in vec4 PositionColor ;
in  mat3 TBN ;
in vec2 texcoord0;
in vec3 normal ;


void main(){
	vec3 normalcolor0 ;

	normalcolor0 = texture2D(Normalmap, texcoord0).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	vec4 basecolor=texture2D(Diffusemap,texcoord0);
	if(Opacity == 1.0)
		gl_FragData[0]=basecolor ;  // Duffise 1 (Solid Colors)			
	else
		gl_FragData[1]=basecolor	;	// Diffuse 2 ( Transparent Color)
	gl_FragData[2]= vec4(texture2D(Specularmap,texcoord0).rgb,1.0);	// Specular 
	gl_FragData[3]= vec4(normal,1.0);							// world normal
	gl_FragData[4]= vec4(0.0,0.0,0.0,1.0); // Material ID
	gl_FragData[5]= vec4(PositionColor.rgb,1.0);						// World Postion
	gl_FragData[6]= vec4(vec3(1.0-texture2D(Diffusemap,texcoord0).a),1.0); // Transprency

}