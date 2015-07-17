uniform sampler2D source;
uniform sampler2D displaceTex;
void main()
{
    // lookup the pixel in the texture
    vec4 disp = texture2D(displaceTex, gl_TexCoord[0].xy);
	float thresh = smoothstep(0.25, 0.35, disp.a);
	vec4 pixel = texture2D(source, gl_TexCoord[0].xy + thresh*0.060*(disp.xy-vec2(0.5, 0.5)));
	float alpha = 1-0.7*thresh*(1-disp.a);//disp.a * thresh;
	float colorPart = 0.4;
	gl_FragColor = (colorPart*thresh)*vec4(0.4,0.6,1,1)
				 + (1-colorPart*thresh)*vec4((1-0.5*thresh)*pixel.r,
											 (1-0.4*thresh)*pixel.g,
											 pixel.b,
											 alpha);// * vec4(1-disp.r,1,1,1));
}
