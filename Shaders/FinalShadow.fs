#version 330 
#define MAX_LIGHTS 50

struct Light {
	mat4 ProjMatrix ;
	mat4 LightViewMtx ;
	sampler2D ShadowMap ;
};

uniform	mat4 CameraWorldMtx ;
uniform Light Sources[MAX_LIGHTS] ;
uniform int NumLights ;

in vec4 VSFragCoord;


void main (){
	vec4 coord;
	vec4 Depth;
	int i ;
	float value=1.0;//means shadowed

	
	for(i=0 ; i< NumLights && value==1.0; ++i){
		coord=Sources[i].ProjMatrix*CameraWorldMtx*Sources[i].LightViewMtx*VSFragCoord;
		coord=coord/coord.w ;
		coord.xyz=(coord.xyz-1.0)/2.0;
		Depth=texture2D(Sources[i].ShadowMap,coord.xy);
		if((Depth.z-coord.z)<=0.005){
			value=0.0 ;
		}
	}
	gl_FragColor=vec4(value);
}