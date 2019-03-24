#version 430
layout(location=0) in vec2 Position ;



layout(std140, binding=0) uniform TransformsMtx{
	mat4 ViewMtx;
	mat4 ProjMtx ;
};

out vec2 pos ;
void main(){
	pos=Position;
	gl_Position=ProjMtx*ViewMtx*vec4(Position.x,0.0,Position.y,1.0);

}