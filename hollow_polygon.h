#ifndef hollow_polygon_h
#define hollow_polygon_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class HollowPolygon {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    int segment;
    float innerRadius;   // Bottom face inner radius
    float outerRadius;   // Bottom face outer radius
    float topInnerRadius; // Top face inner radius
    float topOuterRadius; // Top face outer radius

    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;

    unsigned int diffuseMap;
    unsigned int specularMap;

    float shininess;

    HollowPolygon(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax, int seg, float innerR, float outerR, float topInnerR, float topOuterR)
        : diffuseMap(dMap), specularMap(sMap), shininess(shiny), TXmin(textureXmin), TYmin(textureYmin), TXmax(textureXmax), TYmax(textureYmax),
        segment(seg), innerRadius(innerR), outerRadius(outerR), topInnerRadius(topInnerR), topOuterRadius(topOuterR) {
        setUpPolygonVertexDataAndConfigureVertexAttribute();
    }

    ~HollowPolygon() {
        glDeleteVertexArrays(1, &polygonVAO);
        glDeleteBuffers(1, &polygonVBO);
        glDeleteBuffers(1, &polygonEBO);
    }

    void drawPolygon(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        shader.setMat4("model", model);

        glBindVertexArray(polygonVAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int polygonVAO, polygonVBO, polygonEBO;
    int indexCount;

    void setUpPolygonVertexDataAndConfigureVertexAttribute() {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        // Generate vertices for the inner and outer rings (bottom and top)
        for (int ring = 0; ring < 2; ++ring) {
            float z = (ring == 0) ? 0.0f : 1.0f; // Bottom and top rings
            float currentInnerRadius = (ring == 0) ? innerRadius : topInnerRadius;
            float currentOuterRadius = (ring == 0) ? outerRadius : topOuterRadius;

            for (int i = 0; i < segment; ++i) {
                float angle = static_cast<float>(i) * (2.0f * glm::pi<float>() / segment);

                // Outer ring
                float xOuter = currentOuterRadius * cos(angle);
                float yOuter = currentOuterRadius * sin(angle);
                float nxOuter = cos(angle); // Normal for side faces (x)
                float nyOuter = sin(angle); // Normal for side faces (y)
                vertices.insert(vertices.end(), {
                    xOuter, yOuter, z,
                    nxOuter, nyOuter, 0.0f, // Normals point outward for side faces
                    (xOuter + currentOuterRadius) / (2.0f * currentOuterRadius),
                    (yOuter + currentOuterRadius) / (2.0f * currentOuterRadius)
                    });

                // Inner ring
                float xInner = currentInnerRadius * cos(angle);
                float yInner = currentInnerRadius * sin(angle);
                float nxInner = -cos(angle); // Inward normals for inner vertices
                float nyInner = -sin(angle); // Inward normals for inner vertices
                vertices.insert(vertices.end(), {
                    xInner, yInner, z,
                    nxInner, nyInner, 0.0f, // Normals point inward for side faces
                    (xInner + currentOuterRadius) / (2.0f * currentOuterRadius),
                    (yInner + currentOuterRadius) / (2.0f * currentOuterRadius)
                    });
            }
        }

        // Generate indices for the walls
        for (unsigned int i = 0; i < segment; ++i) {
            unsigned int next = (i + 1) % segment;

            // Bottom face (outer to inner)
            indices.insert(indices.end(), { i * 2, i * 2 + 1, next * 2 });
            indices.insert(indices.end(), { next * 2, i * 2 + 1, next * 2 + 1 });

            // Top face (outer to inner)
            indices.insert(indices.end(), { segment * 2 + i * 2, segment * 2 + next * 2, segment * 2 + i * 2 + 1 });
            indices.insert(indices.end(), { segment * 2 + next * 2, segment * 2 + next * 2 + 1, segment * 2 + i * 2 + 1 });

            // Wall
            indices.insert(indices.end(), { i * 2, segment * 2 + i * 2, next * 2 });
            indices.insert(indices.end(), { next * 2, segment * 2 + i * 2, segment * 2 + next * 2 });

            indices.insert(indices.end(), { i * 2 + 1, next * 2 + 1, segment * 2 + i * 2 + 1 });
            indices.insert(indices.end(), { next * 2 + 1, segment * 2 + next * 2 + 1, segment * 2 + i * 2 + 1 });
        }

        indexCount = indices.size();

        // OpenGL buffers
        glGenVertexArrays(1, &polygonVAO);
        glGenBuffers(1, &polygonVBO);
        glGenBuffers(1, &polygonEBO);

        glBindVertexArray(polygonVAO);

        glBindBuffer(GL_ARRAY_BUFFER, polygonVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

#endif
