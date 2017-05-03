#version 330 
#define MAXSOURCENUMBER 50
uniform CameraProMtx ;
uniform	CameraWorldMtx ;
uniform CameraDepthMap ;
struct Light {
	mat4 ProjMatrix ;
	mat4 LightViewMtx ;
	sampler2D ShadowMap ;
};
uniform int NumLights ;
uniform Light Sources[MAXSOURCENUMBER] ;

in vec4 VSFragCoord;



void main (){
	float value=1.0 ;//means shadowed
	vec4 coord,Depth;
	vec4 VSFragCoord=EyeSpaceFragCoord(CameraDepthMap,texcoord0,CameraProMtx);
	//determine previous frag coodinate equivalent in each light depth map(shadow map)
	//if the fragment is light by at least one light source stop loop and process the next fragment
	for(int i=0 ; i<NumLights ; ++i){
		coord=Sources[i].ProjMatrix*CameraWorldMtx*Sources[i].LightViewMtx*VSFragCoord;
		coord=(coord/coord.w -1.0)/2.0;
		Depth=texture2D(Sources[i].ShadowMap,coord.xy);
		if(Depth.z-coord.z<=0.005){
			value=0.0 ;
			break ;
		}
	}
	gl_FragColor=vec4(value);
}