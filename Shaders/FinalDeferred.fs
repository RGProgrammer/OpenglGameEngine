#version 330 

varying vec2 texcoord0 ;

uniform sampler2D Specular ;
uniform sampler2D Normal ;
uniform sampler2D Diffuse ;
uniform sampler2D Depth ;

void main(){
	gl_FragColor=vec4(texture2D(Diffuse,texcoord0).rgb,1.0);
}