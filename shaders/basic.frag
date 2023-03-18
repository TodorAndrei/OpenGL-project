#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;
//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform bool fog;

vec4 fPosEye;

float fogDensity = 0.03f;

//lighting 2
uniform vec3 lightDir2;
uniform vec3 lightColor2;

vec3 ambient2;
vec3 diffuse2;
vec3 specular2;

//components
vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;

void computeDirLight()
{
    //compute eye space coordinates
    fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = ambientStrength * lightColor;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular = specularStrength * specCoeff * lightColor;
}

void computeDirLight2()
{
    //compute eye space coordinates
    fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir2, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient2 = ambientStrength * lightColor2;

    //compute diffuse light
    diffuse2 = max(dot(normalEye, lightDirN), 0.0f) * lightColor2;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    specular2 = specularStrength * specCoeff * lightColor2;
}

float computeFog()
{
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

	return clamp(fogFactor, 0.0f, 1.0f);
}

void main() 
{
    computeDirLight();
    computeDirLight2();

    //compute final vertex color
    vec3 color = min(((ambient + ambient2) + (diffuse + diffuse2)) * texture(diffuseTexture, fTexCoords).rgb + (specular + specular2)* texture(specularTexture, fTexCoords).rgb, 1.0f);

    vec4 fogColor = vec4(0.5f,0.5f,0.5f,1.0f);
	float fogFactor = computeFog();

    if(fog == true)
		fColor = fogColor * (1- fogFactor) + vec4(color * fogFactor,1);
	else
		fColor = vec4(color, 1.0f);
}
