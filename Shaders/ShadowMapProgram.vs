#version 330

layout(location=0) in vec3 Vertex ;
uniform mat4 World ;
uniform mat4 VP ;
void main(){
	gl_Position=VP*World* vec4(Vertex,1.0) ;	
}
