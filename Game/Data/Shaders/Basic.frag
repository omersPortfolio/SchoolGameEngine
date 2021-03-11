uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_HasTexture;
varying vec2 v_UVCoord;

void main()
{
    if(u_HasTexture == 1)
    {
        gl_FragColor = texture2D( u_Texture, v_UVCoord );
    }
    else
    {
        gl_FragColor = u_Color;
    }
}
