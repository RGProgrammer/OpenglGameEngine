#version 330 
#define MAX_LIGHTS 25

struct Light {
	mat4 ProjMatrix ;
	mat4 LightBiasVPMtx ;//not bias yet, it just ViewProjection
	sampler2D ShadowMap ;
};
uniform Light Sources[MAX_LIGHTS] ;
uniform int NumLights ;
uniform	mat4 CameraViewMtx ;

in vec4 VSFragCoord0 ;
in vec2 texcoord0;
vec4 EyeSpaceFragCoord(in sampler2D Depthtex,in vec2 texCoord,in mat4 projMtx){
	vec3 NDC;
	NDC.xy=(2.0*texCoord-1.0);
	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);

	vec4 clipPos;
	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
	clipPos.xyz=NDC*clipPos.w ;
	return inverse(projMtx)*clipPos ;

}
void main (){
	
	float value=1.0;//means shadowed
	vec4 FragLightEyeSpaceCoord;
	vec2 ShadowMapTexCoord;
	int numlights ;
	if(NumLights > MAX_LIGHTS)
		numlights=25 ;
	else
		numlights=NumLights ;
	for(int i=0 ; (i< numlights); ++i){
		//ShadowMapTexCoord= (Sources[i].LightBiasVPMtx*inverse(CameraViewMtx)*VSFragCoord0).xy;
		//FragLightEyeSpaceCoord=EyeSpaceFragCoord(Sources[i].ShadowMap,ShadowMapTexCoord,Sources[i].ProjMatrix);
		//if(length(VSFragCoord0)-length(FragLightEyeSpaceCoord) <=0.005){
			//value=length(VSFragCoord0)-length(FragLightEyeSpaceCoord);
			//break ;
		//}
		value=texture(Sources[i].ShadowMap,texcoord0).r*2.0-1.0 ;
	}
	gl_FragColor=vec4(value,value,value,1.0);
}