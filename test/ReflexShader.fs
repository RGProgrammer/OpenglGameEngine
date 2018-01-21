#version 410
#extension GL_NV_shadow_samplers_cube : enable
uniform samplerCube Probe ;
in vec3 Reflected ;
in vec4 Normal ;
in vec4 PositionColor ;
void main()
{
	
	gl_FragData[0]= textureCube(Probe,Reflected); //diffuse
	gl_FragData[1]= vec4(0.0); //specular
	gl_FragData[2]= Normal; //normal
	gl_FragData[3]= vec4(0.0); //material ID
	gl_FragData[4]= PositionColor; //PositionColor
}