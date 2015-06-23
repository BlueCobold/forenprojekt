uniform sampler2D tex;
uniform float time;
void main()
{
    vec4 col = texture2D(tex, gl_TexCoord[0].xy);
	float r = 0.5f + 0.125f*sin(1.5f*time+7.0f*sin(1.0f*time)-gl_FragCoord.x/3.0f)
				   + 0.125f*sin(2.2f*time+9.0f*sin(1.0f*time)-gl_FragCoord.x/5.0f)
				   + 0.125f*sin(2.4f*time+2.0f*sin(1.7f*time)+gl_FragCoord.y/6.5f)
				   + 0.125f*sin(1.9f*time+3.0f*sin(2.7f*time)+gl_FragCoord.y/8.0f);
    gl_FragColor = vec4(col.b * r,
						col.g * r,
						mod(gl_FragCoord.x/20.0f, 2.0f), col.a);
}