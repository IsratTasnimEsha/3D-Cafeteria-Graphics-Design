#ifndef polygon_h
#define polygon_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Polygon {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    int segment;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;

    int c = -1;
    int l = 0;

    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    Polygon(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax, float seg)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;
        this->segment = seg;

        setUpPolygonVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Polygon()
    {
        glDeleteVertexArrays(1, &polygonVAO);
        glDeleteVertexArrays(1, &lightPolygonVAO);
        glDeleteVertexArrays(1, &lightTexPolygonVAO);
        glDeleteBuffers(1, &polygonVBO);
        glDeleteBuffers(1, &polygonEBO);
    }

    void drawPolygonWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexPolygonVAO);
        glDrawElements(GL_TRIANGLES, 3000, GL_UNSIGNED_INT, 0);
    }

    void drawLightPolygon(Shader& lightShader, glm::mat4 model, glm::vec3 lightColor)
    {
        lightShader.use();

        lightShader.setVec3("lightColor", lightColor);

        lightShader.setMat4("model", model);

        glBindVertexArray(lightPolygonVAO);
        glDrawElements(GL_TRIANGLES, 3000, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int polygonVAO;
    unsigned int lightPolygonVAO;
    unsigned int lightTexPolygonVAO;
    unsigned int polygonVBO;
    unsigned int polygonEBO;

    void setUpPolygonVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        const int MAX_INDICES = 1000;
        float polygon_vertices[MAX_INDICES] = {};
        int count1 = 0;

        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = -1;
        polygon_vertices[count1++] = 0.5;
        polygon_vertices[count1++] = 0.5;
        c += 1;

        for (int i = 0; i < 360; i += 360 / segment) {
            float angle = static_cast<float>(i) * (3.1416 / 180.0f);
            float x = cos(angle);
            float y = sin(angle);

            polygon_vertices[count1++] = x;
            polygon_vertices[count1++] = y;
            polygon_vertices[count1++] = 0;
            polygon_vertices[count1++] = 0;
            polygon_vertices[count1++] = 0;
            polygon_vertices[count1++] = -1;
            polygon_vertices[count1++] = TXmin + (TXmax - TXmin) * (x + 1) / 2;
            polygon_vertices[count1++] = TYmin + (TYmax - TYmin) * (y + 1) / 2;

            c += 1;
        }

        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 1;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 0;
        polygon_vertices[count1++] = 1;
        polygon_vertices[count1++] = 0.5;
        polygon_vertices[count1++] = 0.5;
        c += 1;

        for (int i = 0; i < 360; i += 360 / segment) {
            float angle = static_cast<float>(i) * (3.1416 / 180.0f);
            float x = cos(angle);
            float y = sin(angle);

            polygon_vertices[count1++] = x;
            polygon_vertices[count1++] = y;
            polygon_vertices[count1++] = 1;
            polygon_vertices[count1++] = 0;
            polygon_vertices[count1++] = 0;
            polygon_vertices[count1++] = 1;
            polygon_vertices[count1++] = TXmin + (TXmax - TXmin) * (x + 1) / 2;
            polygon_vertices[count1++] = TYmin + (TYmax - TYmin) * (y + 1) / 2;

            c += 1;
        }

        for (int i = 0; i < 360; i += 360 / segment) {
            float angle = static_cast<float>(i) * (3.1416 / 180.0f);
            float x = cos(angle);
            float y = sin(angle);

            if (l % 2 == 0) {
                polygon_vertices[count1++] = x;
                polygon_vertices[count1++] = y;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = TXmin;
                polygon_vertices[count1++] = TYmin;
            }
            else {
                polygon_vertices[count1++] = x;
                polygon_vertices[count1++] = y;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = TXmax;
                polygon_vertices[count1++] = TYmin;
            }

            if (l % 2 == 0) {
                polygon_vertices[count1++] = x;
                polygon_vertices[count1++] = y;
                polygon_vertices[count1++] = 1;
                polygon_vertices[count1++] = 1;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = TXmin;
                polygon_vertices[count1++] = TYmax;
            }
            else {
                polygon_vertices[count1++] = x;
                polygon_vertices[count1++] = y;
                polygon_vertices[count1++] = 1;
                polygon_vertices[count1++] = 1;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = 0;
                polygon_vertices[count1++] = TXmax;
                polygon_vertices[count1++] = TYmax;
            }
            l++;
        }

        unsigned int polygon_indices[MAX_INDICES] = {};
        int count2 = 0;

        for (int i = 1; i < c / 2; i++) {
            polygon_indices[count2++] = 0;
            polygon_indices[count2++] = i;
            polygon_indices[count2++] = i + 1;
        }
        polygon_indices[count2++] = 0;
        polygon_indices[count2++] = c / 2;
        polygon_indices[count2++] = 1;

        for (int i = c / 2 + 2; i < c; i++) {
            polygon_indices[count2++] = c / 2 + 1;
            polygon_indices[count2++] = i;
            polygon_indices[count2++] = i + 1;
        }
        polygon_indices[count2++] = c / 2 + 1;
        polygon_indices[count2++] = c;
        polygon_indices[count2++] = c / 2 + 2;

        for (int i = c + 1; i < c + c - 2; i++) {
            polygon_indices[count2++] = i;
            polygon_indices[count2++] = i + 1;
            polygon_indices[count2++] = i + 2;
        }

        polygon_indices[count2++] = c + c - 2;
        polygon_indices[count2++] = c + c - 1;
        polygon_indices[count2++] = c + 1;

        polygon_indices[count2++] = c + c - 1;
        polygon_indices[count2++] = c + 1;
        polygon_indices[count2++] = c + 2;

        glGenVertexArrays(1, &polygonVAO);
        glGenVertexArrays(1, &lightPolygonVAO);
        glGenVertexArrays(1, &lightTexPolygonVAO);
        glGenBuffers(1, &polygonVBO);
        glGenBuffers(1, &polygonEBO);


        glBindVertexArray(lightTexPolygonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(polygon_vertices), polygon_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(polygon_indices), polygon_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightPolygonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(polygonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


#endif /* cube_h */