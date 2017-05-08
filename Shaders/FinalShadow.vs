#version 330 


layout(location=0)in vec2 position ;
layout(location=1)in vec2 texcoord ;


uniform mat4 CameraProMtx ;
uniform sampler2D CameraDepthMap ;

out vec4 VSFragCoord;


vec4 EyeSpaceFragCoord(sampler2D Depthtex,vec2 texCoord,mat4 projMtx){
	vec3 NDC;
	NDC.xy=(2.0*texCoord-1.0);
	NDC.z=(2.0 * texture2D(Depthtex,texCoord).r-1.0);

	vec4 clipPos;
	clipPos.w=projMtx[3][2]/(NDC.z-(projMtx[2][2]/projMtx[2][3]));
	clipPos.xyz=NDC*clipPos.w ;
	return inverse(projMtx)*clipPos ;

}
void main(){
	
	VSFragCoord=EyeSpaceFragCoord(CameraDepthMap,texcoord,CameraProMtx) ;
	gl_Position=vec4(position,0.0,1.0);
}