#version 330 

layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;

//uniform mat4 CameraProMtx ;
//uniform mat4 CameraViewMtx ;
//uniform sampler2D CameraDepthMap ;
//out vec4 WSFragCoord0 ;
out vec2 texcoord0;
//vec4 WorldSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx, mat4 ViewMtx)
//{
//	vec3 NDC;
//	NDC.xy=(2.0*texCoord-1.0);
//	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);
//
//	vec4 clipPos;
//	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
//	clipPos.xyz=NDC*clipPos.w ;
//	vec4 coord =inverse(projMtx)*clipPos ;
//	return inverse(ViewMtx)*coord ;
//}

void main(){
	//WSFragCoord0=WorldSpaceFragCoord(CameraDepthMap,texcoord,CameraProMtx,CameraViewMtx) ;
	texcoord0=texcoord;
	texcoord0.y=1.0-texcoord0.y;
	gl_Position=vec4(position,0.0,1.0);
}