#version 450

#define NUMMAXMATERIALS 200

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader5 : require  //uint64_t

struct Materials {
	uint64_t DiffuseBindless ;
	uint64_t SpecularBindless ;
	uint64_t NormalBindless ;
	float 		IOR ;
	float		Opacity ;
} ;


layout(std140, binding=2) uniform MaterialBuffer {
	Materials allMaterials[NUMMAXMATERIALS] ;
} ;

//sampler2D s = sampler2D(allMaterials[index].DiffuseBindless);
in vec4 PositionColor ;
in  mat3 TBN ;
in vec2 texcoord0;
in vec3 normal ;
void main(){

	//vec3 normalcolor0 ;

	//normalcolor0 = texture2D(Normalmap, texcoord0).rgb;
	//normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	//normalcolor0 = normalize(TBN * normalcolor0);
	//vec4 basecolor=texture2D(Diffusemap,texcoord0);
	vec4 basecolor= texture2D(matData.NormalBindless,texcoord0);
	//if(Opacity == 1.0)
		gl_FragData[0]=basecolor ;  // Duffise 1 (Solid Colors)			
//	else
		gl_FragData[1]=basecolor	;	// Diffuse 2 ( Transparent Color)

		
	//gl_FragData[2]= vec4(texture2D(Specularmap,texcoord0).rgb,1.0);	// Specular 
	gl_FragData[2]= texture2D(matData.SpecularBindless,texcoord0);
	gl_FragData[3]= vec4(normal,1.0);							// world normal
	gl_FragData[4]= vec4(0.0,0.0,0.0,1.0); // Material ID
	gl_FragData[5]= vec4(PositionColor.rgb,1.0);						// World Postion
	gl_FragData[6]= vec4(0.0,0.0,0.0,1.0); // Transprency

}