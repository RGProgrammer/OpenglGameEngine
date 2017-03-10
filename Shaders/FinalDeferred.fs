#version 410 
#define MAX_LIGHTS 100
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
uniform mat4 ViewMtx;
uniform mat4 ProjMtx ;
uniform vec3 CameraPos;
uniform sampler2D Specular ;
uniform sampler2D Normal ;
uniform sampler2D Diffuse ;
uniform sampler2D Position ;
uniform sampler2D Depth ;

in vec2 texcoord0 ;
in vec3 CameraDir ;
in vec3 EyeDirection ;

void main(){
	// final Colors
	vec3 FinalColor=vec3(0.0);
	vec3 FinalDiffuse=vec3(0.0) ;
	vec3 FinalSpecular=vec3(0.0);
	

	//next will be pushed to the vertex shader ;
	vec4 DiffuseColor = texture2D(Diffuse,texcoord0);
	vec4 specularColor =texture2D(Specular,texcoord0);
	vec3 NormalColor=texture2D(Normal,texcoord0).xyz;
	vec3 PositionColor=texture2D(Position,texcoord0).xyz;
	float roughness = specularColor.a;
	float Attinuation;

	//
	float Intensity ;
	vec3 LightDir;
	vec3 HalfV;
	vec3 SpotDirection ;
	if(numLights > 0 ){
		
		for(int i=0;i < numLights; i++){
			if(Sources[i].Distance < 0.0){ //is a Directionnal light
				LightDir=(Sources[i].WorldMtx[2].xyz);
				Intensity=max(dot(NormalColor,LightDir),0.0);
				if(Intensity>0.0){
					FinalDiffuse=Intensity*(DiffuseColor.rgb*Sources[i].DiffuseColor);
					HalfV=normalize(LightDir+(CameraPos-PositionColor));
					FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
					FinalColor+=FinalDiffuse+FinalSpecular ;
				}
			}else{
				
				Attinuation=clamp ( 1.0/(Sources[i].Distance*Sources[i].Distance),0.0,1.0) ;
				if(Sources[i].CutoffAngle < 0.0){// is a Point light
					LightDir=PositionColor-Sources[i].WorldMtx[3].xyz;
					Intensity=max(dot(NormalColor,LightDir),0.0);
					if(Intensity>0.0){
						FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
						HalfV=normalize(LightDir+(CameraPos-PositionColor));
						FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
						FinalColor+= Attinuation*(FinalDiffuse+FinalSpecular) ;
					}

				}else{// is a SpotLight
					LightDir=PositionColor-Sources[i].WorldMtx[3].xyz;
					SpotDirection=Sources[i].WorldMtx[2].xyz;
					if(dot(SpotDirection,LightDir)> Sources[i].CutoffAngle){
						Intensity=max(dot(NormalColor,LightDir),0.0);
						if(Intensity>0.0){
							FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
							HalfV=normalize(LightDir+(CameraPos-PositionColor));
							FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess)*specularColor.rgb*Sources[i].SpecularColor;
							FinalColor+= Attinuation*(FinalDiffuse+FinalSpecular) ;
						}
					}
				}
			}
		}
	}else{
		FinalColor=normalize(DiffuseColor.rgb+specularColor.rgb);
		//FinalColor=specularColor.xyz;
		//FinalColor=vec3(getDepthValue(texcoord0,Depth)) ;
		//FinalColor=abs(NormalColor);
		FinalColor=abs(PositionColor);
	}
	gl_FragColor=vec4(FinalColor,1.0);
}


