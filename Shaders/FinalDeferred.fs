#version 330 
#define MAX_LIGHTS 100
struct Light {
	mat4	WorldMtx ;
	vec3	DiffuseColor;
	vec3    SpecularColor ;
	float 	Attinuation ;
	float	CutoffAngle ;
	float   Shininess ;
};

varying vec2 texcoord0 ;
uniform mat4 ViewMtx ;
uniform mat4 ProjMtx ;
uniform Light Sources[MAX_LIGHTS];
uniform int numLights ;
uniform sampler2D Specular ;
uniform sampler2D Normal ;
uniform sampler2D Diffuse ;
uniform sampler2D Depth ;

vec4 ProjtoView(vec2 texcoord,mat4 ProjectMtx,sampler2D Depthtex){
	vec3 Color=texture2D(Depthtex, texcoord).rgb; 
	float z = Color.r + Color.g*256.0+ Color.b*256.0*256.0 ; 
    float x = texcoord.x * 2 - 1;
    float y = (1 - texcoord.y) * 2 - 1;
    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    mat4 inverseProjectMtx=inverse(ProjectMtx);
    vec4 vPositionVS = mul(vProjectedPos, inverseProjectMtx);  
    return vec4(vPositionVS.xyz / vPositionVS.w,1.0);  
}

vec3 ViewtoWorld(vec4 projPosition,mat4 ViewMtx){
	mat4 inverseViewMtx= inverse(ViewMtx);
	return mul(inverseViewMtx,projPosition).xyz ;
}


void main(){
	// final Color
	vec3 FinalColor=vec3(0.0,0.0,0.0);
	vec3 FinalDiffuse ;
	vec3 FinalSpecular;
	vec3 SpotDirection ;

	//
	vec4 DiffuseColor = texture2D(Diffuse,texcoord0);
	vec3 NormalColor=texture2D(Normal,texcoord0).rgb;
	vec4 specularColor =texture2D(Specular,texcoord0);
	float Roughness = specularColor.a;
	//
	float Intensity ;
	vec3 fragPosition ;
	vec3 LightDir;
	vec3 HalfV;
	if(numLights > 0 ){
		fragPosition=ViewtoWorld(ProjtoView(texcoord0,ProjMtx,Depth),ViewMtx);
		for(int i=0;i < numLights; i++){
			if(Sources[i].Attinuation < 0.0){ //is a Directionnal light
				LightDir=Sources[i].WorldMtx[2].xyz;
				Intensity=max(dot(NormalColor,LightDir),0.0);
				if(Intensity>0.0){
					FinalDiffuse=Intensity*(DiffuseColor.rgb*Sources[i].DiffuseColor);
					HalfV=normalize(LightDir+ViewMtx[2].xyz);
					FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess*Roughness)*specularColor.rgb*Sources[i].SpecularColor;
					FinalColor+=FinalDiffuse+FinalSpecular ;
				}
			}else{
				if(Sources[i].CutoffAngle < 0.0){// is a Point light
					LightDir=fragPosition-Sources[i].WorldMtx[2].xyz;
					Intensity=max(dot(NormalColor,LightDir),0.0);
					if(Intensity>0.0){
						FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
						HalfV=normalize(LightDir+ViewMtx[2].xyz);
						FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess*Roughness)*specularColor.rgb*Sources[i].SpecularColor;
						FinalColor+= Sources[i].Attinuation*(FinalDiffuse+FinalSpecular) ;
					}

				}else{// is a SpotLight
					LightDir=fragPosition-Sources[i].WorldMtx[2].xyz;
					SpotDirection=Sources[i].WorldMtx[2].xyz;
					if(dot(SpotDirection,LightDir)> Sources[i].CutoffAngle){
						Intensity=max(dot(NormalColor,LightDir),0.0);
						if(Intensity>0.0){
							FinalDiffuse=Intensity*DiffuseColor.rgb*Sources[i].DiffuseColor;
							HalfV=normalize(LightDir+ViewMtx[2].xyz);
							FinalSpecular=pow(dot(NormalColor,HalfV),Sources[i].Shininess*Roughness)*specularColor.rgb*Sources[i].SpecularColor;
							FinalColor+=Sources[i].Attinuation*(FinalDiffuse+FinalSpecular);
						}
					}
				}
			}
		}
	}else{
		FinalColor=normalize(DiffuseColor.rgb+specularColor.rgb);
	}
	gl_FragColor=vec4(FinalColor,DiffuseColor.a);
}


