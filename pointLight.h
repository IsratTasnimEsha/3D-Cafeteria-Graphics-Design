//
//  pointLight.h
//  test
//
//  Created by Abrar Hasan on 22/9/23.
//

#ifndef pointLight_h
#define pointLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber;

    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();

        int index = lightNumber - 1; // Adjust to 0-based indexing for the array
        std::string lightBase = "pointLights[" + std::to_string(index) + "]";

        lightingShader.setVec3(lightBase + ".position", position);
        lightingShader.setVec3(lightBase + ".ambient", ambientOn * ambient);
        lightingShader.setVec3(lightBase + ".diffuse", diffuseOn * diffuse);
        lightingShader.setVec3(lightBase + ".specular", specularOn * specular);
        lightingShader.setFloat(lightBase + ".k_c", k_c);
        lightingShader.setFloat(lightBase + ".k_l", k_l);
        lightingShader.setFloat(lightBase + ".k_q", k_q);
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnAmbientOff()
    {
        diffuseOn = 1.0;
        specularOn = 1.0;
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        ambientOn = 0.0;
        specularOn = 0.0;
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        ambientOn = 1.0;
        specularOn = 1.0;
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */
