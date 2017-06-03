#version 410 
#define MAX_LIGHTS 25
struct Light {
	mat4	WorldMtx ;
	vec3	DiffuseColor;
	vec3    SpecularColor ;
	float 	Distance ;
	float	CutoffAngle ;
	float   Shininess ;
};


uniform Light Sources[MAX_LIGHTS];
uniform int numLights ;
uniform sampler2D Specular ;
uniform sampler2D Normal ;
uniform sampler2D Diffuse ;
uniform sampler2D Shadow ;
uniform sampler2D Depth ;
uniform mat4 CameraViewMtx;
uniform mat4 CameraProjMtx ;
uniform vec2 DepthRange ; 


in vec2 texcoord0 ;
float unpack (vec4 colour)
{
	//const vec4 bitShifts = vec4(1.0,
	//				1.0 / 255.0,
	//				1.0 / (255.0 * 255.0),
	//				1.0 / (255.0 * 255.0 * 255.0));
	//return dot(colour, bitShifts);
	return colour.r ;
}
vec4 EyeSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx){
	float DepthValue=unpack(texture2D(Depthtex,texCoord));
	vec3 NDC;
	NDC.xy=2.0*texCoord-1.0 ;
	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);
	vec4 clipPos;
	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
	clipPos.xyz=NDC*clipPos.w ;
	return inverse(projMtx)*clipPos ;
}


void main(){
	
	vec3 FinalColor=vec3(0.0);
	vec3 FinalDiffuse=vec3(0.0) ;
	vec3 FinalSpecular=vec3(0.0);
	vec3 FragCoord=EyeSpaceFragCoord(Depth,texcoord0,CameraProjMtx).xyz ;

	
	vec4 DiffuseColor = texture2D(Diffuse,texcoord0);
	vec4 specularColor =texture2D(Specular,texcoord0);
	vec3 NormalColor=texture2D(Normal,texcoord0).xyz;
	vec3 ShadowValue=texture2D(Shadow,texcoord0).rgb;
	
	float Attinuation;
	float LightFragDistance ;
	float DepthValue=texture2D(Depth,texcoord0).r*2.0-1.0 ;
	vec3 LightEyeSpacePos ;

	//
	float Intensity ;
	vec3 LightDir;
	vec3 HalfV;
	vec3 SpotDirection ;

		if(numLights > 0 ){
		
			for(int i=0;i < numLights; i++){
				if(Sources[i].Distance < 0.0){ //is a Directionnal light
					LightDir=(CameraViewMtx*Sources[i].WorldMtx[2]).xyz*(-1);
					Intensity=max(dot(NormalColor,LightDir),0.0);
					if(Intensity>0.0){
						FinalDiffuse=Intensity*(DiffuseColor.rgb*Sources[i].DiffuseColor);
						HalfV=normalize(LightDir-FragCoord);
						FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
						FinalColor+=FinalDiffuse+FinalSpecular ;
					}
				}else{
					LightEyeSpacePos=(CameraViewMtx*Sources[i].WorldMtx[3]).xyz;
					LightFragDistance=distance(FragCoord,LightEyeSpacePos);
					Attinuation=1.0/(LightFragDistance/Sources[i].Distance) ;
					Attinuation*=Attinuation;
					LightDir=normalize(LightEyeSpacePos-FragCoord);
					
					if(Sources[i].CutoffAngle < 0.0){// is a Point light
						Intensity=max(dot(NormalColor,LightDir),0.0);
						if(Intensity > 0.0){
							FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
							HalfV=normalize(LightDir-normalize(-FragCoord));
							FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
							FinalColor+= Attinuation*(FinalDiffuse+FinalSpecular) ;
						}

					}else{// is a SpotLight
						SpotDirection=normalize(CameraViewMtx*Sources[i].WorldMtx[2]).xyz*(-1);
						if(acos(dot(SpotDirection,LightDir))<= Sources[i].CutoffAngle){
							Intensity=max(dot(NormalColor,LightDir),0.0);
							if(Intensity > 0.0){
								FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
								HalfV=normalize(LightDir+normalize(-FragCoord));
								FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
								FinalColor+= Attinuation*(FinalDiffuse+FinalSpecular) ;
							}
						}
					}
				}
			}
		}else{
			FinalColor=normalize(DiffuseColor.rgb+specularColor.rgb);
		}
	FinalColor*=ShadowValue ;
	gl_FragColor=vec4(FinalColor,1.0);
}