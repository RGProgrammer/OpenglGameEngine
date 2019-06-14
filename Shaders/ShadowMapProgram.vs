#version 430

layout(location=0) in vec3 Vertex ;
uniform mat4 World ;
uniform mat4 View ;
uniform mat4 Projection ;
void main(){
	gl_Position=Projection*View*World* vec4(Vertex,1.0) ;	
}
