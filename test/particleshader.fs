#version 410 

in vec4 normal0 ;
in vec4 position0;
in vec3 color0 ;


void main (){


	gl_FragData[0]=vec4(color0,1.0);;

	gl_FragData[1]=vec4(0.2,0.2,0.2,1.0);

	gl_FragData[2]=normal0 ;
	
	gl_FragData[3]=vec4(0.0);

	gl_FragData[4]=position0; // Position buffer
}