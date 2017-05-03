#version 330

layout(location=0) in Vertex ;
uniform mat4 World ;
uniform mat4 VP ;
void main(){
	gl_Position=VP*World* Vertex ;	
}
