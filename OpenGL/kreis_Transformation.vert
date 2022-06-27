uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
mat4 modelViewMatrix = viewMatrix * modelMatrix;


uniform float Time;

in vec3 vertexPosition;

in vec2 textureCoords;

out vec2 texCoords;


void main(void)
{
    texCoords = textureCoords;

vec3 mittelPunkt = vec3(0,0,0);
float radius = 2f;


vec3 kreisPunkt  = normalize(vertexPosition - mittelPunkt) * radius ;

vec3 punktZuKreispunkt = (vertexPosition - kreisPunkt)* Time ; //* Time


if(length(vertexPosition)==0)
gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition,1);
else
 gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition - punktZuKreispunkt,1);
 
}
