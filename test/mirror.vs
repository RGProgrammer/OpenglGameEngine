#version 410
layout(location=0) in vec3 Position ;
layout(location=1) in vec3 Normal ;
layout(location=2) in vec2 TexCoord ;

uniform mat4 World ;
uniform mat4 View ;
uniform mat4 Proj ;



out vec4 PositionColor ;
out vec4 Normal0 ;
out vec2 texcoord0;
void main(){
	texcoord0=TexCoord ;
	PositionColor=World*vec4(Position,1.0);
	Normal0= View*World*vec4(Normal,0.0);
	gl_Position=Proj*View*World*vec4(Position,1.0);


}