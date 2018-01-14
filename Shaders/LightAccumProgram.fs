#version 410 


float unpack (vec4 colour)
{
	return colour.r ;
}
vec4 EyeSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx){
	float DepthValue=unpack(texture2D(Depthtex,texCoord));
	vec3 NDC ;
	NDC.xy=2.0*texCoord-1.0 ;
	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);
	vec4 clipPos;
	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
	clipPos.xyz=NDC*clipPos.w ;
	return inverse(projMtx)*clipPos ;
}

struct Light {
	mat4	WorldMtx ;
	vec3	DiffuseColor;
	vec3    SpecularColor ;
	float 	Distance ;
	float	CutoffAngle ;
	float   Shininess ;
};

uniform sampler2D NormalMap;
uniform sampler2D Depth ;

uniform mat4 CameraViewMtx;
uniform mat4 CameraProjMtx ;
uniform mat4 LightProjMatrix ;
uniform mat4 LightViewMtx ;
uniform Light Source ;

in vec2 texcoord0 ;
in vec2 texcoord1;
void main()
{
	float Attinuation;
	float LightFragDistance ;
	float Intensity ;
	vec3 FragCoord=EyeSpaceFragCoord(Depth,texcoord1,CameraProjMtx).xyz ;
	vec3 NormalColor=texture2D(NormalMap,texcoord1).xyz;
	vec3 LightEyeSpacePos ;
	vec3 LightDir;
	vec3 HalfV;
	vec3 SpotDirection ;
	vec3 DiffuseColor=vec3(0.0) ;
	vec3 SpecularColor=vec3(0.0) ;


	if(Source.Distance < 0.0){ //is a Directionnal light
		LightDir=(CameraViewMtx*Source.WorldMtx[2]).xyz*(-1);
		Intensity=max(dot(NormalColor,LightDir),0.0);
		if(Intensity>0.0){
			HalfV=normalize(normalize(FragCoord)+LightDir);
			SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
			DiffuseColor=Intensity*Source.DiffuseColor.rgb;
		}
	}else{
		LightEyeSpacePos=(CameraViewMtx*Source.WorldMtx[3]).xyz;
		LightFragDistance=distance(FragCoord,LightEyeSpacePos);
		Attinuation=1.0/(LightFragDistance/Source.Distance) ;
		Attinuation*=Attinuation;
		LightDir=normalize(LightEyeSpacePos-FragCoord);
					
		if(Source.CutoffAngle < 0.0){// is a Point light
			Intensity=max(dot(NormalColor,LightDir),0.0);
			if(Intensity > 0.0){
				HalfV=normalize(LightDir+normalize(-FragCoord));
				SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
				DiffuseColor= Attinuation*Intensity*Source.DiffuseColor;
			}

		}else{// is a SpotLight
			SpotDirection=normalize(CameraViewMtx*Source.WorldMtx[2]).xyz*(-1);
			if(acos(dot(SpotDirection,LightDir))<= Source.CutoffAngle){
				Intensity=max(dot(NormalColor,LightDir),0.0);
				if(Intensity > 0.0){
					HalfV=normalize(LightDir+normalize(-FragCoord));
					SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
					DiffuseColor= Attinuation*Intensity*Source.DiffuseColor;
				}
			}
		}
	}
	gl_FragData[0] = vec4(DiffuseColor,1.0);
	gl_FragData[1] = vec4(SpecularColor,1.0);

}