#sginclude "://shaders/commonlight.vert.inc"

out vec4 color;

void main()
{
    vec3 viewPosition = eyePosition().xyz;
    vec3 viewDirection = -normalize(viewPosition);
    vec3 vertexColor = phong(viewPosition, viewDirection, eyeNormalVec().xyz, vec3(1.0));

    viewPosition = -viewDirection;

    vec3 lichtVec = -normalize(sunLights[0].viewDirection);

    float dotNormal = dot(vertexNormal,lichtVec);




if(dotNormal < 0.25)
          dotNormal = 0.5;
else if(dotNormal<0.5)
          dotNormal = 0.6;
else if(dotNormal<0.75)
          dotNormal = 0.7;
else if(dotNormal<0.9)
          dotNormal = 0.8;
else
          dotNormal = 0.9;


          // color = vec4(vertexColor + material.emission.rgb, computeMaterialAlpha());
        color=vec4(material.diffuse.xyz * dotNormal, computeMaterialAlpha());


    gl_Position = vPos();
}
