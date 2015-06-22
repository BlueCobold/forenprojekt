uniform sampler2D tex;
void main()
{
    vec4 col = texture2D(tex, gl_TexCoord[0].xy);
    gl_FragColor = vec4(col.r, col.g * mod(gl_FragCoord.x/20.0f, 2.0f), mod(gl_FragCoord.x/20.0f, 2.0f), col.a);
}