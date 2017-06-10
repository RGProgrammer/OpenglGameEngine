#version 410 

#define DIRECTIONNAL 1
#define POINT 2
#define SPOT 3

uniform sampler2D PositionMap ;
uniform int lighttype ;
uniform mat4 LightProjMatrix ;
uniform mat4 LightViewMtx ;
uniform sampler2D ShadowMap ;

//uniform mat4 CameraProMtx ;
//uniform mat4 CameraViewMtx ;
//uniform sampler2D CameraDepthMap ;
float unpack (vec4 colour)
{
	//const vec4 bitShifts = vec4(1.0,
	//				1.0 / 255.0,
	//				1.0 / (255.0 * 255.0),
	//				1.0 / (255.0 * 255.0 * 255.0));
	//return dot(colour, bitShifts);
	return colour.r ;
}
//vec4 ViewSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx)
//{
//	float DepthValue=unpack(texture2D(Depthtex,texCoord));
//	vec4 clipPos ;
//	clipPos.xy=2.0*texCoord-1.0 ;
//	clipPos.z=DepthValue*2.0-1.0 ;
//	clipPos.w=1.0 ;
//	vec4 Pos=inverse(projMtx)*clipPos ;
//	Pos/=Pos.w ;
//	return Pos ;
//}
//vec4 EyetoWorldSpace(vec4 EyeSpace,mat4 ViewMtx){
//	return inverse(ViewMtx)*EyeSpace;
//}
in vec2 texcoord0;
vec4 WSFragCoord0 ;
vec3 color ;
void main (){
	
	vec4 ViewSpaceCoord ;
	vec4 ProjectionCoord=vec4(0.0);
	vec4 ShadowMapTexCoord=vec4(0.0);
	//WSFragCoord0=EyetoWorldSpace(ViewSpaceFragCoord(CameraDepthMap,texcoord0,CameraProMtx),CameraViewMtx);
	WSFragCoord0=texture2D(PositionMap,texcoord0) ;
	color=vec3(0.5);
	ProjectionCoord=LightProjMatrix*LightViewMtx*WSFragCoord0 ;
	ProjectionCoord/=ProjectionCoord.w ;
	ShadowMapTexCoord=ProjectionCoord*0.5+0.5;
	
	if((ShadowMapTexCoord.x>=0.0 && ShadowMapTexCoord.x<=1.0) &&(ShadowMapTexCoord.y>=0.0 && ShadowMapTexCoord.y<=1.0)){
		if(SPOT==lighttype){
			if(length(ProjectionCoord.xy)<=1.0)
				if((ShadowMapTexCoord.z)-0.001 <= unpack(texture2D(ShadowMap,ShadowMapTexCoord.xy))){
					color=vec3(1.0);
				}
		}
		else {
			if((ShadowMapTexCoord.z)-0.001 <= unpack(texture2D(ShadowMap,ShadowMapTexCoord.xy))){
				color=vec3(1.0);
			}
		}
	}
	gl_FragColor=vec4(color,1.0);

}