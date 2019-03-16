#version 430 




struct Light {
	mat4	WorldMtx ;
	vec3	DiffuseColor;
	vec3    SpecularColor ;
	float 	Distance ;
	float	CutoffAngle ;
	float   Shininess ;
};

uniform sampler2D NormalMap;
uniform sampler2D PositionMap ;
uniform vec3 	  CameraPos ;
uniform Light Source ;

in vec2 texcoord0 ;

void main()
{
	float Attinuation;
	float LightFragDistance ;
	float Intensity=0.0 ;
	vec3 FragCoord= texture2D(PositionMap,texcoord0).xyz ;
	vec3 NormalColor=texture2D(NormalMap,texcoord0).xyz;
	vec3 LightPos=Source.WorldMtx[3].xyz ;
	vec3 LightDir=Source.WorldMtx[2].xyz*(-1);
	vec3 HalfV;
	vec3 SpotDirection ;
	vec3 DiffuseColor=vec3(0.0) ;
	vec3 SpecularColor=vec3(0.0) ;


	if(Source.Distance < 0.0){ //is a Directionnal light
		Intensity=max(dot(NormalColor,LightDir),0.0);
		if(Intensity>0.0){
			HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
			SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
			DiffuseColor=Intensity*Source.DiffuseColor.rgb;
		}
	}else{
		LightFragDistance=distance(FragCoord,LightPos);
		Attinuation=1.0/(LightFragDistance/Source.Distance) ;
		Attinuation*=Attinuation;
		LightDir=normalize(LightPos-FragCoord);
					
		if(Source.CutoffAngle < 0.0){// is a Point light
			Intensity=max(dot(NormalColor,LightDir),0.0);
			if(Intensity > 0.0){
				HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
				SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
				DiffuseColor= Attinuation*Intensity*Source.DiffuseColor;
			}

		}else{// is a SpotLight
			SpotDirection=normalize(Source.WorldMtx[2]).xyz*(-1);
			if(acos(dot(SpotDirection,LightDir))<= Source.CutoffAngle){
				Intensity=max(dot(NormalColor,LightDir),0.0);
				if(Intensity > 0.0){
					HalfV= normalize(LightDir)+normalize(CameraPos-FragCoord);
					SpecularColor=pow(dot(NormalColor,HalfV),Source.Shininess)*Source.SpecularColor;
					DiffuseColor= Attinuation*Intensity*Source.DiffuseColor;
				}
			}
		}
	}
	gl_FragData[0] = vec4(DiffuseColor,1.0);
	gl_FragData[1] = vec4(SpecularColor,1.0);

}