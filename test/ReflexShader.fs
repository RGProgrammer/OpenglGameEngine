#version 410
#extension GL_NV_shadow_samplers_cube : enable
uniform samplerCube Probe ;
in vec3 Reflected ;
in vec4 Normal ;
in vec4 PositionColor ;
void main()
{
	
	//gl_FragData[0]= vec4(textureCube(Probe,Reflected).rgb,1.0); //diffuse
	gl_FragData[0]=vec4(0.0,0.0,0.0,1.0);
	gl_FragData[1]= vec4(0.0); //specular
	gl_FragData[2]= vec4(Normal.xyz,0.0); //normal
	gl_FragData[3]= vec4(0.0); //material ID
	gl_FragData[4]= vec4(PositionColor.xyz,1.0); //PositionColor
}