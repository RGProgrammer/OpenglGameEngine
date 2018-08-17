#version 410



uniform sampler2D Diffusemap ;
uniform sampler2D Specularmap ;
uniform sampler2D Normalmap ;
uniform samplerCube ReflectMap ;
uniform int 	hasReflectMap ;

in vec4 PositionColor ;
in  mat3 TBN ;
in vec2 texcoord0;
in mat4 ViewMtx0;


void main(){
	vec3 normalcolor0 ;

	normalcolor0 = texture2D(Normalmap, texcoord0).rgb;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	vec4 basecolor=texture2D(Diffusemap,texcoord0);
	gl_FragData[0]=basecolor	; // basecolor with transprent material rendered 
	if(basecolor.a==1.0)
		gl_FragData[1]=basecolor	;				//basecolor without transprent material
	gl_FragData[2]=texture2D(Specularmap,texcoord0);//
	gl_FragData[3]=ViewMtx0*vec4(normalcolor0,0.0);
	gl_FragData[4]=vec4(texture2D(Diffusemap,texcoord0).a,0.0,0.0,1.0);
	gl_FragData[5]=PositionColor;
	gl_FragData[6]= vec4(vec3(texture2D(Diffusemap,texcoord0).a),1.0); 

}