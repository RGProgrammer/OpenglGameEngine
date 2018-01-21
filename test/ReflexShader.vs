#version 410

layout(location=0) in vec3 vertex ;
layout(location=0) in vec3 normals ;

uniform mat4 World ;
uniform mat4 View ;
uniform mat4 Proj ;
uniform vec3 CameraPos ;

out vec3 Reflected ;
out vec4 Normal ;
out vec4 PositionColor ;
void main()
{
	
	PositionColor=World*vec4(vertex,1.0);
	Normal=World*vec4(normals,0.0);
	vec3 ViewRay =PositionColor.xyz-CameraPos ;
	Reflected=reflect(ViewRay,Normal.xyz);
	gl_Position=Proj*View*World*vec4(vertex,1.0);
}
