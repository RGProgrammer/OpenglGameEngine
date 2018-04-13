#version 410 

#define DIRECTIONNAL 1
#define POINT 2
#define SPOT 3

uniform sampler2D PositionMap ;
uniform int lighttype ;
uniform float shadowStrengh ;
uniform mat4 LightProjMatrix ;
uniform mat4 LightViewMtx ;
uniform sampler2D ShadowMap ;

float unpack (vec4 colour)
{
	return colour.r ;
}

in vec2 texcoord0;
vec4 WSFragCoord0 ;
vec3 color ;
void main (){
	
	vec4 ViewSpaceCoord ;
	vec4 ProjectionCoord=vec4(0.0);
	vec4 ShadowMapTexCoord=vec4(0.0);
	WSFragCoord0=texture2D(PositionMap,texcoord0) ;
	color=vec3(shadowStrengh);
	ProjectionCoord=LightProjMatrix*LightViewMtx*WSFragCoord0 ;
		ProjectionCoord/=ProjectionCoord.w ;
	ShadowMapTexCoord=ProjectionCoord*0.5+0.5;
	
	if((ShadowMapTexCoord.x>=0.0 && ShadowMapTexCoord.x<=1.0) &&(ShadowMapTexCoord.y>=0.0 && ShadowMapTexCoord.y<=1.0)){
		if(SPOT==lighttype){
			if(length(ProjectionCoord.xy)<=1.0)
				if((ShadowMapTexCoord.z)-0.009 <= unpack(texture2D(ShadowMap,ShadowMapTexCoord.xy))){
					color=vec3(1.0);
				}
		}
		else {
			if((ShadowMapTexCoord.z)-0.009 <= unpack(texture2D(ShadowMap,ShadowMapTexCoord.xy))){
				color=vec3(1.0);
			}
		}
	}
	gl_FragColor=vec4(color,1.0);
	

}