
vec3 calcLight(vec3 normal, vec3 viewDir, vec3 lightDir, Light light, Material material) {
	// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Combine results
    vec3 ambient  = light.ambient  * material.ka;
    vec3 diffuse  = light.diffuse  * diff * material.kd;
    vec3 specular = light.specular * spec * material.ks;

	return (ambient + diffuse + specular);
	//return vec3(diffuse);
}
