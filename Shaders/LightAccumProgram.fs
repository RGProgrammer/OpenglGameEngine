#version 430 
#define MAXNUMLIGHTSOURCES  100



struct Light {
	mat4	WorldMtx ;
	vec4	DiffuseColor;
	vec4    SpecularColor ;
	float 	Distance ;
	float	CutoffAngle ;
	float   Shininess ;
	float   padding ;
};


layout(std140, binding=0) uniform TransformsMtx {
	Light Sources[MAXNUMLIGHTSOURCES];
};
uniform int 	 NumLights ;
uniform sampler2D NormalMap;
uniform sampler2D PositionMap ;
uniform vec3 	  CameraPos ;


in vec2 texcoord0 ;

void main()
{
	float Attinuation;
	float LightFragDistance ;
	float Intensity=0.0 ;
	vec3 FragCoord= texture2D(PositionMap,texcoord0).xyz ;
	vec3 NormalColor=texture2D(NormalMap,texcoord0).xyz;
	vec3 LightPos;
	vec3 LightDir;
	vec3 HalfV;
	vec3 SpotDirection ;
	vec3 DiffuseColor=vec3(0.0) ;
	vec3 SpecularColor=vec3(0.0) ;

	for(int i=0 ; i< NumLights ; ++i){
	 LightPos=Sources[i].WorldMtx[3].xyz ;
	 LightDir=Sources[i].WorldMtx[2].xyz*(-1);
	if(Sources[i].Distance < 0.0){ //is a Directionnal light
		Intensity=max(dot(NormalColor,LightDir),0.0);
		if(Intensity>0.0){
			HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
			SpecularColor+=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*Sources[i].SpecularColor.rgb;
			DiffuseColor+=Intensity*Sources[i].DiffuseColor.rgb;
		}
	}else{
		LightFragDistance=distance(FragCoord,LightPos);
		Attinuation=1.0/(LightFragDistance/Sources[i].Distance) ;
		Attinuation*=Attinuation;
		LightDir=normalize(LightPos-FragCoord);
					
		if(Sources[i].CutoffAngle < 0.0){// is a Point light
			Intensity=max(dot(NormalColor,LightDir),0.0);
			if(Intensity > 0.0){
				HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
				SpecularColor+=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*Sources[i].SpecularColor.rgb;
				DiffuseColor+= Attinuation*Intensity*Sources[i].DiffuseColor.rgb;
			}

		}else{// is a SpotLight
			SpotDirection=normalize(Sources[i].WorldMtx[2]).xyz*(-1);
			if(acos(dot(SpotDirection,LightDir))<= Sources[i].CutoffAngle){
				Intensity=max(dot(NormalColor,LightDir),0.0);
				if(Intensity > 0.0){
					HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
					SpecularColor+=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*Sources[i].SpecularColor.rgb;
					DiffuseColor+= Attinuation*Intensity*Sources[i].DiffuseColor.rgb;
				}
			}
		}
	}
	}
	gl_FragData[0] = vec4(DiffuseColor,1.0);
	gl_FragData[1] = vec4(SpecularColor,1.0);

}