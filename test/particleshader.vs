#version 410

layout(location= 0) in vec3 Position ;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 Color ;

out vec4 normal0 ;
out vec4 position0;
out vec3 color0 ;

void main(){
	position0=Model*vec4(Position,1.0);
	normal0=View*Model*vec4(Position,0.0);
	color0=Color ;
	gl_Position=Projection*View*position0 ;
}