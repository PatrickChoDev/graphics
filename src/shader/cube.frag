#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 color;
uniform vec3 viewPos;
uniform float materialType; // 0=wall, 1=pellet, 2=floor

void main() {
    // Light configuration
    vec3 lightPos1 = vec3(10.0, 10.0, 10.0);
    vec3 lightPos2 = vec3(5.0, 8.0, 15.0);
    vec3 lightColor = vec3(0.9, 0.95, 1.0); // Slightly cool white

    // Much lower ambient for dramatic lighting
    float ambientStrength = 1;
    vec3 ambient = ambientStrength * vec3(0.1, 0.15, 0.25); // Very dark blue ambient

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Light 1 - Main dramatic light
    vec3 lightDir1 = normalize(lightPos1 - FragPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    float distance1 = length(lightPos1 - FragPos);
    float attenuation1 = 1.0 / (1.0 + 0.09 * distance1 + 0.032 * distance1 * distance1);
    vec3 diffuse1 = diff1 * lightColor * attenuation1;

    // Specular for glass-like reflection
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 64.0);
    vec3 specular1 = spec1 * lightColor * attenuation1;

    // Light 2 - Subtle fill light
    vec3 lightDir2 = normalize(lightPos2 - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    float distance2 = length(lightPos2 - FragPos);
    float attenuation2 = 1.0 / (1.0 + 0.09 * distance2 + 0.032 * distance2 * distance2);
    vec3 diffuse2 = diff2 * lightColor * attenuation2 * 0.3;

    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32.0);
    vec3 specular2 = spec2 * lightColor * attenuation2 * 0.3;

    // Material-specific properties
    vec3 finalColor = color;
    float glassiness = 0.0;
    float alpha = 1.0;

    if (materialType < 0.5) { // Walls - solid glass-like
                              glassiness = 0.8;
                              finalColor = mix(color, vec3(0.6, 0.8, 1.0), 0.3); // Cool glass tint
                              alpha = 1.0; // Walls remain opaque
    } else if (materialType < 1.5) { // Pellets - translucent glass-like
                                     glassiness = 0.9;
                                     finalColor = mix(color, vec3(1.0, 0.95, 0.7), 0.2); // Subtle warm tint
                                     alpha = 0.6; // Semi-transparent pellets
    } else { // Floor - matte
             glassiness = 0.1;
             finalColor = mix(color, vec3(0.2, 0.3, 0.6), 0.4); // Dark blue floor
             alpha = 1.0;
    }

    // Combine lighting
    vec3 lighting = ambient + diffuse1 + diffuse2;
    vec3 specular = (specular1 + specular2) * glassiness;

    // Fresnel effect for glass-like materials
    float fresnel = 0.0;
    if (glassiness > 0.3) {
        float fresnelPower = 1.0 - max(dot(norm, viewDir), 0.0);
        fresnel = pow(fresnelPower, 2.0) * glassiness * 0.5;

        // For translucent materials, fresnel affects alpha too
        if (materialType >= 0.5 && materialType < 1.5) {
            alpha = mix(alpha, min(alpha + 0.3, 1.0), fresnelPower);
        }
    }

    vec3 result = lighting * finalColor + specular + vec3(fresnel);

    // Reduce gamma correction for brighter colors
    result = pow(result, vec3(1.0/2.0));

    // Remove color grading to preserve original colors

    FragColor = vec4(result, alpha);
}