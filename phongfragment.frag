#version 410 core
out vec4 fragColor;

in vec3 normalTransposed;
in vec3 fragmentPosition;
in vec2 UV;

uniform  sampler2D textureSampler;

uniform float ambientStrength = 0.3;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightColor = vec3(0.7, 0.1, 0.1);
uniform vec3 objectColor = vec3(0.8, 0.7, 0.1);


uniform float lightStrength = 0.9;
uniform float specularStrength = 0.5;
uniform int specularExponent = 32;

void main(){
    //ambient
    vec3 ambient = ambientStrength * lightColor;
    //diffuse
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection),0.0);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrength;
    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0) * texture(textureSampler, UV);

//    vec3 result = ambient * color;
//    fragColor = color;

}
