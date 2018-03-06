#version 410
uniform sampler2D Capture ;
in vec4 PositionColor ;
in vec4 Normal0 ;
in vec2 texcoord0;

void main(){
	
	gl_FragData[0]=vec4(0.0,0.0,0.0,1.0);//texture2D(Capture,texcoord0);
	gl_FragData[1]=vec4(0.5,0.5,0.5,1.0);
	gl_FragData[2]=Normal0 ;
	gl_FragData[3]=vec4(0.0);
	gl_FragData[4]=PositionColor; 
}