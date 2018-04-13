#version 410

layout(points) in;
layout(line_strip, max_vertices = 13) out;




uniform mat4 Model ;
uniform mat4 VP ;

void main(){
	vec4 pos=gl_in[0].;

	gl_Position=VP*Model*(pos+vec4(1.0,0.0,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.6,0.3,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.3,0.6,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.0,1.0,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(-0.3,0.6,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(-0.6,0.3,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(-1.0,0.0,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(-0.6,-0.3,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(-0.3,-0.6,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.0,-1.0,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.3,-0.6,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(0.6,-0.3,0.0,0.0));
	EmitVertex();

	gl_Position=VP*Model*(pos+vec4(1.0,0.0,0.0,0.0));
	EmitVertex();
}