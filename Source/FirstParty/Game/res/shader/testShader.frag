void main()
{
    gl_FragColor = vec4(0.5f*mod(100*gl_TexCoord[0].x, 1.0f), 0.5f*mod(100*gl_TexCoord[0].y, 1.0f), mod(gl_FragCoord.x/20.0f, 2.0f), 1.f);
}