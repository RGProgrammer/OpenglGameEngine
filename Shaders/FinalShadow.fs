#version 330 
#define MAX_LIGHTS 25

struct Light {
	mat4 ProjMatrix ;
	mat4 LightViewMtx ;
	sampler2D ShadowMap ;
};

uniform Light Sources[MAX_LIGHTS] ;
uniform int NumLights ;
uniform mat4 CameraProMtx ;
uniform mat4 CameraViewMtx ;
uniform sampler2D CameraDepthMap ;

vec4 EyeSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx)
{
	vec3 NDC;
	NDC.xy=2.0*texCoord-1.0 ;
	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);

	vec4 clipPos;
	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
	clipPos.xyz=NDC*clipPos.w ;
	return inverse(projMtx)*clipPos  ;
}
vec4 EyetoWorldSpace(vec4 EyeSpace,mat4 ViewMtx){
	return inverse(ViewMtx)*EyeSpace;
	//return ViewMtx*EyeSpace;
}
//in vec4 WSFragCoord0 ;
in vec2 texcoord0;
void main (){
	float value=1.0;//means shadowed
	int numlights0 ;
	vec4 ShadowMapTexCoord=vec4(0.0);
	vec4 ShadowViewCoord=vec4(0.0) ;
	vec4 LightViewCoord=vec4(0.0);
	vec4 WSFragCoord0=EyetoWorldSpace(EyeSpaceFragCoord(CameraDepthMap,texcoord0,CameraProMtx),CameraViewMtx) ;
	if(NumLights > MAX_LIGHTS)
		numlights0=25 ;
	else
		numlights0=NumLights ;

	for(int i=0 ; i< numlights0; ++i){
		ShadowViewCoord=Sources[i].LightViewMtx*WSFragCoord0;
		ShadowMapTexCoord=Sources[i].ProjMatrix*ShadowViewCoord ;
		//ShadowMapTexCoord/=ShadowMapTexCoord.w ;
		ShadowMapTexCoord=ShadowMapTexCoord*0.5+0.5;
		LightViewCoord=EyeSpaceFragCoord(Sources[i].ShadowMap,ShadowMapTexCoord.xy,Sources[i].ProjMatrix);
		if(ShadowViewCoord.z <= LightViewCoord.z){
			value=0.0 ;
			break ;
		}
		//if(ShadowMapTexCoord.z<=texture2D(Sources[0].ShadowMap,ShadowMapTexCoord.xy).r){
		//	value=0.0 ; //means lit
		//	break ;
		//}
	}
	gl_FragData[0]=vec4(value,value,value,1.0);
	//gl_FragData[0]=vec4(ShadowMapTexCoord.rgb,1.0);

}