#version 430

in vec2 pos ;
void main(){


	gl_FragData[0]=	vec4(1.0,1.0,1.0,1.0); // Diffuse
	if(pos.x==0.0 && pos.y !=0.0){
		gl_FragData[0]=	vec4(0.0,0.0,1.0,1.0); // Diffuse
	}else if(pos.y==0.0 && pos.x !=0.0){
		gl_FragData[0]=	vec4(1.0,0.0,0.0,1.0); // Diffuse
	}
	
	gl_FragData[1]= vec4(0.5,0.5,0.5,1.0); // Specular 
	gl_FragData[2]= vec4(0.0,0.0,0.0,1.0); // world normal
	gl_FragData[3]= vec4(1.0,0.0,0.0,1.0); // Material ID
	gl_FragData[4]= vec4(0.0,0.0,0.0,1.0); // World Position
	gl_FragData[5]= vec4(0.0,0.0,0.0,1.0); // Transparency

}