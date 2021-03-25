vec3 CalculateLight(vec3 lightPos, vec3 lightColor, float lightRange, vec3 cameraPos, vec3 materialColor, vec3 surfacePos, vec3 normal)
{
    vec3 dirToLight = lightPos - surfacePos;
    float distanceFromLight = length( dirToLight );
    vec3 normalizedDirToLight = dirToLight / distanceFromLight;

    // Ambient.
    float ambientPerc = 0.05;

    // Diffuse.
    float diffusePerc = max( 0.0, dot( normalizedDirToLight, normal ) );

    // Specular.
    vec3 normalizedDirToCamera = normalize( cameraPos - surfacePos );
    vec3 halfVector = normalize( normalizedDirToLight + normalizedDirToCamera );
    float specularPerc = max( 0.0, dot( halfVector, normal ) );
    specularPerc = pow( specularPerc, 50.0 );

    // Attenuation.
    float rangePerc = distanceFromLight / lightRange;
    float attenuation = max( 0.0, 1.0 - rangePerc );

    // Light components.
    vec3 ambient = materialColor * lightColor * ambientPerc;
    vec3 diffuse = materialColor * lightColor * diffusePerc * attenuation;
    vec3 specular = lightColor * specularPerc * attenuation;

    // Combined.
    return ambient + diffuse + specular;
}
