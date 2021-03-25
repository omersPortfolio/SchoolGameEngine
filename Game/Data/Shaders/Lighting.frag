uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec3 u_CameraPosition;

varying vec2 v_UVCoord;
varying vec3 v_Normal;
varying vec3 v_SurfacePos;

void main()
{
    //gl_FragColor = texture2D( u_Texture, v_UVCoord ) * u_Color;

    vec3 lightPos = vec3(0,0,0);
    vec3 lightColor = vec3(0,0,1);
    float lightRange = 10.0;
    vec3 materialColor = vec3(1,1,1);

    vec3 litColor = vec3(0,0,0);
    litColor += CalculateLight( vec3( 0,0,2), vec3(0,1,0), lightRange, u_CameraPosition, materialColor, v_SurfacePos, v_Normal );
    litColor += CalculateLight( vec3( 2,0,0), vec3(1,0,0), lightRange, u_CameraPosition, materialColor, v_SurfacePos, v_Normal );
    litColor += CalculateLight( vec3(-2,0,0), vec3(0,0,1), lightRange, u_CameraPosition, materialColor, v_SurfacePos, v_Normal );
    
    gl_FragColor.rgb = litColor;
    gl_FragColor.a = 1;
}
