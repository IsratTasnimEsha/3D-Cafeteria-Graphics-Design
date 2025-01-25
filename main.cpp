//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "sphere.h"
#include "Cone2.h"
#include "cube.h"
#include "polygon.h"
#include "hollow_polygon.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

glm::mat4 RotationMatricesX(float theta);
glm::mat4 RotationMatricesY(float theta);
glm::mat4 RotationMatricesZ(float theta);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 900;

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = false;
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;
unsigned int bezierCylinderVAO;
unsigned int bezierLightsVAO;

float ang = 20.0f;
float ballonSpeed = 0.0f;

vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = -90.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 0.0;
float translate_X = 13.0;
float translate_Y = -9.0;
float translate_Z = -47; // -27.0;
float scale_X = 3.0;
float scale_Y = .5;
float scale_Z = 1.0;

float r_fan = 0.0;
float r_sphere = 0.0;

bool tvEnabled = false;
int tv_count = 0;
bool tvOn = false;
bool tvOff = true;

bool lightEnabled = false;
bool rotationEnabled = false;
bool backgroundEnabled = false;

bool fanEnabled = false;
bool fanOn = false;

bool curtainEnabled = false;
float t_curtain = 0.0;
bool curtainOpen = false;
bool curtainClose = true;
bool curtainStill = true;
float curtain_speed = 0.5f;

bool doorEnabled = false;
float t_sliding_door = 0.0;
bool doorOpen = false;
bool doorClose = true;
bool doorStill = true;
float sliding_door_speed = 0.1f;

bool liftEnabled = false;
float t_lift = 0.0;
bool liftOpen = false;
bool liftClose = false;
bool liftStill = true;
float lift_door_speed = 0.05f;

bool liftMoveEnabled = false;
float t_lift_move = 0.0;
bool liftMoveOn = false;
bool liftMoveOff = true;
bool liftMoveStill = true;
float lift_move_speed = 0.1f;

bool directionalLightOn = true;
bool spotLightOn = true;

int noOfLights = 50;
// camera

Camera camera(glm::vec3(-2.0f, 0.5f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 2.0, eyeZ = -10.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(7.5, 16.5,  -1.0f),
    glm::vec3(7.5, 16.5,  2.0f),
    glm::vec3(0.0f, -1.2f, -1.0f),
    glm::vec3(0.0f, 5.0f, 0.0f),
    glm::vec3(0.0f, 5.0f, 2.0f),
    glm::vec3(2.0f, 5.0f, 0.0f),
    glm::vec3(2.0f, 5.5f, 2.0f),
    glm::vec3(1.0f, 5.0f, -9.0f),

    glm::vec3(5.0f + 5 * 1.5, 2.5f, -0.5f),
    glm::vec3(8.0f, 2.6f, -8.5f),
    glm::vec3(0.9f, 2.3f, -3.0f),
    glm::vec3(0.0f, 0.0f, -6.0f),
    glm::vec3(6.5f + 2 * 3.5, 2.6f, -0.5f),
    glm::vec3(5.0f,  10.0f,  5.0f),
    glm::vec3(2.5f, 0.0f, 4.0f),
    glm::vec3(2.5f, 8.0f, 3.0f),

    glm::vec3(0.0f + 4 * 3, 0.0f, 0.5f),
    glm::vec3(0.0f, 4.8f, 8.5f),
    glm::vec3(3.0f + 5 * 4, 5.0f, 3.0f),
    glm::vec3(1.0f, 2.0f, 1.5),
    glm::vec3(5.0, 0.0f, 1.0),
    glm::vec3(-8.0f + 2*5.5f, 0.0f, 2.5f),
    glm::vec3(5.0f, 20.0f, 3.0f),
    glm::vec3(5.0f, 20.0f, 4.0f),

    glm::vec3(5.0f,  0.0f,  -2.0f),
    glm::vec3(5.0f,  0.0f,  -1.0f),
    glm::vec3(5.0f, 0.f, -1.0f),
    glm::vec3(5.0f, 0.0f, 0.0f),
    glm::vec3(5.0f,  0.0f,  1.0f),
    glm::vec3(5.0f,  0.0f,  2.0f),
    glm::vec3(5.0f, 0.0f, 3.0f),
    glm::vec3(5.0f, 0.0f, 4.0f),
              
    glm::vec3(5.0f,  10.0f,  -2.0f),
    glm::vec3(5.0f,  10.0f,  -1.0f),
    glm::vec3(5.0f, 10.f, -1.0f),
    glm::vec3(5.0f, 10.0f, 0.0f),
    glm::vec3(5.0f,  10.0f,  1.0f),
    glm::vec3(5.0f,  10.0f,  2.0f),
    glm::vec3(5.0f, 10.0f, 3.0f),
    glm::vec3(5.0f, 10.0f, 4.0f),
              
    glm::vec3(5.0f,  20.0f,  -5.0f),
    glm::vec3(5.0f,  20.0f,  0.0f),
    glm::vec3(5.0f, 20.f, -1.0f),
    glm::vec3(5.0f, 20.0f, 0.0f),
    glm::vec3(5.0f,  20.0f,  1.0f),
    glm::vec3(5.0f,  20.0f,  2.0f),
    glm::vec3(5.0f, 20.0f, 3.0f),
    glm::vec3(5.0f, 20.0f, 4.0f),

    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 8.0f, 0.0f),
};

bool pointLightOn[50] = { true, true, true, true, true, true, true, true,
                         true, true, true, true, true, true, true, true,
                         true, true, true, true, true, true, true, true, 
                        true, true, true, true, true, true, true, true,
                         true, true, true, true, true, true, true, true,
                         true, true, true, true, true, true, true, true, 
                        true, true};

//glm::vec3(-0.5, 1, -0.5)


std::vector<PointLight> pointLights;

// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

void setUpLighting(Shader& lightingShader) {
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);

    // Assuming pointLights is a vector of PointLight objects
    for (size_t i = 0; i < pointLights.size(); ++i) {
        pointLights[i].setUpPointLight(lightingShader);
    }

    lightingShader.setVec3("diectionalLight.directiaon", 0.0f, 4.8f, 6.5f + 2 * 3);
    lightingShader.setVec3("diectionalLight.ambient", 0.8, 0.8, 0.8);
    lightingShader.setVec3("diectionalLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("diectionalLight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setBool("dlighton", directionalLightOn);

    lightingShader.setVec3("spotlight.position", -3.0f, 8.0f, -40.0f);
    lightingShader.setVec3("spotlight.direction", 0, -1, 0);
    lightingShader.setVec3("spotlight.ambient", 1.0, 1.0, 0.0);
    lightingShader.setVec3("spotlight.diffuse", 1.0f, 1.0f, 0.0f);
    lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 0.0f);
    lightingShader.setFloat("spotlight.k_c", 0.02f);
    lightingShader.setFloat("spotlight.k_l", 0.02f);
    lightingShader.setFloat("spotlight.k_q", 0.02f);
    lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));
    lightingShader.setBool("spotlighton", spotLightOn);
}
    

void drawChairWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    Cube& cube_wall,
    Cube& cube_floor,
    const glm::vec3& viewPos) {

    // Start with the identity matrix for the whole chair
    glm::mat4 chairTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    chairTransformMatrix = glm::rotate(chairTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Keep the original structure for the chair as it was
    glm::mat4 translateMatrix, rotateMatrix, scaleMatrix, model;

    // Vertical cylinders (legs)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.95f, 0.0f, 5.7f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 1.5f, 0.15f));
    model = globalTranslationMatrix * chairTransformMatrix * scaleMatrix;
    cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.8f, 0.0f, 5.7f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 1.5f, 0.15f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.95f, 0.0f, 6.3f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 1.5f, 0.15f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.8f, 0.0f, 6.3f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.15f, 1.5f, 0.15f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Seat (cube)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 1.5f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 0.2f, 1.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (cube)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 1.6f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrests (cubes)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f, 1.6f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest vertical sections
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Seat bottom part
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.7f, 2.0f, 5.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 0.8f, 0.1f));
    model = globalTranslationMatrix*chairTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
}

void drawTableWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    Cube& cube_floor) {

    // Start with the identity matrix for the whole table
    glm::mat4 tableTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    tableTransformMatrix = glm::rotate(tableTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Keep the original structure for the table as it was
    glm::mat4 translateMatrix, scaleMatrix, model;

    // Table top (rectangular surface)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.7f, 2.0f, 5.7f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(2.0f, 0.1f, 1.5f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Table leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.7f, 0.0f, 6.25f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.0f, 0.2f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Table leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.4f, 0.0f, 5.9f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0f, 0.1f, 1.0f));
    model = globalTranslationMatrix*tableTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
}


void drawSofaWithTransformations(const glm::mat4& identityMatrix,
    const glm::mat4& globalTranslationMatrix,
    const glm::vec3& translation,
    const glm::vec3& rotation,
    Shader& lightingShaderWithTexture,
    Shader& ourShader,
    Cube& cube_floor,
    Cube& cube_box) {

    // Start with the identity matrix for the whole sofa
    glm::mat4 sofaTransformMatrix = glm::translate(identityMatrix, translation);

    // Apply rotation (on all axes)
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    sofaTransformMatrix = glm::rotate(sofaTransformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translateMatrix, scaleMatrix, model;

    // Sofa parts (like seat cushions, backrest, and armrests)
    // Seat 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Seat 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -0.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -.01f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, -.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrest 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, 2.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Armrest 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -0.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 0.2f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (large part)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -.01f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Backrest (small part)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(14.0f, -.01f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.2f, 2.1f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Bottom Seat Cushion 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(4.0f, 0.2f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Bottom Seat Cushion 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 6.8f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(4.0f, 0.2f, 0.2f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Side Cushion 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.2f, 1.2f, 5.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.8f, 0.6f, 2.0f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Side Cushion 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(10.2f, 1.8f, 6.5f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.8f, 1.6f, 0.6f));
    model = globalTranslationMatrix*sofaTransformMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);
}

void drawLift(glm::mat4 globalTranslationMatrix, Shader& lightingShaderWithTexture, Shader& ourShader, Cube& cube_floor, Cube& cube_wall, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix;
    glm::mat4 rotateMatrix;
    glm::mat4 scaleMatrix;
    glm::mat4 model;

    // Global transformations
    globalTranslationMatrix = glm::translate(globalTranslationMatrix, translation);

    // Apply scaling
    scaleMatrix = glm::scale(identityMatrix, scale);
    globalTranslationMatrix = globalTranslationMatrix * scaleMatrix;

    // Apply rotation
    rotateMatrix = glm::rotate(identityMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    globalTranslationMatrix = globalTranslationMatrix * rotateMatrix;

    // Right Wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.2f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 3.7f, 0.3f));
    model = globalTranslationMatrix * scaleMatrix;
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Left Wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.2f, -0.3f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 3.7f, 0.3f));
    model = globalTranslationMatrix * scaleMatrix;
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.0f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 4.0f));
    model = globalTranslationMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Ceiling
    translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 3.7f, -4.0f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.2f, 4.0f));
    model = globalTranslationMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

    // Near Wall

    if (liftStill) {
        t_lift = 0.0f;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.2f, -3.75f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.0f, 3.5f, 3.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    else if (!liftStill && liftOpen) {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.2f, -3.75f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.0f, 3.5f, 3.5f + t_lift));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

        if (t_lift < -3.3) {
            liftStill = false;
            liftOpen = false;
            liftClose = true;
            t_lift = 0.0;
        }
        else {
            t_lift -= lift_door_speed;
            liftOpen = true;
            liftClose = false;
        }
    }

    else if (!liftStill && liftClose) {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0f, 0.2f, -3.75f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.0f, 3.5f, 0.2f + t_lift));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

        if (t_lift > 3.3f) {
            liftStill = true;
            liftOpen = false;
            liftClose = false;
        }
        else {
            t_lift += lift_door_speed;
            liftOpen = false;
            liftClose = true;
        }
    }

    // Far Wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.2f, -3.75f));
    scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.0f, 3.5f, 3.5f));
    model = globalTranslationMatrix * scaleMatrix;
    cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void drawFan(
    const glm::mat4& globalTranslationMatrix,
    glm::vec3 basePosition,
    glm::vec3 baseScale,
    float rotationAngle,
    bool fanOn,
    Shader& shader,
    Shader& ourShader,
    float& ang,
    Polygon& cylinder_mirror,
    Cube& cube_tile
) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 model, translateMatrix, rotateMatrix, scaleMatrix;

    // Draw fan base
    translateMatrix = glm::translate(identityMatrix, basePosition);
    rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(rotateMatrix, glm::vec3(baseScale.x, baseScale.y, baseScale.z * -0.4f));
    model = globalTranslationMatrix * scaleMatrix;
    cylinder_mirror.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, basePosition + glm::vec3(0.0f, -0.2f, 0.0f));
    rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(rotateMatrix, glm::vec3(baseScale.x, baseScale.y, baseScale.z * -0.2f));
    model = globalTranslationMatrix * scaleMatrix;
    cylinder_mirror.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

    // Fan blades
    for (int i = 0; i < 3; ++i) {
        float angleOffset = i * 60.0f; // Rotate blades by 60 degrees each
        for (int j = 0; j < 2; ++j) {
            float bladeOffset = j == 0 ? 0.75f : -0.75f;

            translateMatrix = glm::translate(identityMatrix, basePosition + glm::vec3(0.0f, 0.2f, 0.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(angleOffset + ang), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(baseScale.x * 6.0f * (j == 0 ? 1.0f : -1.0f), baseScale.y * -0.2f, baseScale.z * bladeOffset));
            model = globalTranslationMatrix * scaleMatrix;
            cube_tile.drawCubeWithTexture(shader, model);
        }
    }
}

int main()
{
    for (int i = 0; i < noOfLights; ++i) {
        pointLights.emplace_back(
            pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z, // position
            1.0f, 0.0f, 0.0f, // ambient
            1.0f, 0.0f, 0.0f, // diffuse
            1.0f, 0.0f, 0.0f, // specular
            0.015f, // k_c
            0.015f, // k_l
            0.015f, // k_q
            i + 1 // light number (1-based indexing)
        );
    }

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        //Uper Plane
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };

    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    vector<float> cntrlPointsCylinder
    {
    0.1150, 1.2650, 5.1000,
    -0.0050, 1.2750, 5.1000,
    -0.1250, 1.2650, 5.1000,
    -0.2550, 1.2400, 5.1000,
    -0.3600, 1.2100, 5.1000,
    -0.4800, 1.1750, 5.1000,
    -0.5650, 1.1400, 5.1000,
    -0.5900, 1.0750, 5.1000,
    -0.5650, 1.0350, 5.1000,
    -0.5250, 0.9950, 5.1000,
    -0.4600, 0.9650, 5.1000,
    -0.3750, 0.9300, 5.1000,
    -0.3100, 0.9100, 5.1000,
    -0.2550, 0.8950, 5.1000,
    -0.1650, 0.8700, 5.1000,
    -0.0950, 0.8600, 5.1000,
    -0.0500, 0.8600, 5.1000,
    0.0050, 0.8600, 5.1000,
    0.0550, 0.8600, 5.1000,
    0.0950, 0.8500, 5.1000,
    };

    unsigned int bezierCylinderVAO;
    coordinates.clear();
    normals.clear();
    indices.clear();
    vertices.clear();
    bezierCylinderVAO = hollowBezier(cntrlPointsCylinder.data(), ((unsigned int)cntrlPointsCylinder.size() / 3) - 1);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath;
    unsigned int diffMap;
    string specularMapPath = "container2_specular.png";
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    diffuseMapPath = "wall.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_wall = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "curtain.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_curtain = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "floor_.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_floor = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "box.jpeg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_box = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "bottol.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bottol_box = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "cone.jpeg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    // Create a Cone2 object
    Cone2 cone_chair = Cone2(
        1.0f,                   // Radius
        2.0f,                   // Height
        36,                     // Sector count (smoothness of the cone base)
        glm::vec3(0.2f, 0.2f, 0.2f), // Ambient color
        glm::vec3(1.0f, 0.5f, 0.3f), // Diffuse color
        glm::vec3(1.0f, 1.0f, 1.0f), // Specular color
        32.0f,                 // Shininess
        diffMap,            // Diffuse texture ID
        specMap,           // Specular texture ID
        0.0f, 0.0f,            // Texture coordinates (Xmin, Ymin)
        1.0f, 1.0f             // Texture coordinates (Xmax, Ymax)
    );

    diffuseMapPath = "mirror.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_mirror(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "besin_tile.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    HollowPolygon cylinder_besin(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 6, 1.9f, 2.2f, 1.0f, 1.3f);

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    HollowPolygon cylinder_kitchen_besin(diffMap, specMap, 32.0f, 0.0f, 0.0f, 4.0f, 1.0f, 4, 1.9f, 2.2f, 1.8f, 2.1f);

    diffuseMapPath = "besin.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_besin = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "design1.jpeg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_design1(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "design2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_design2(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "design3.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_design3(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "design4.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_design4(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "design5.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon cylinder_design5(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 30);

    diffuseMapPath = "hexagon.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon hexagon_design1(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 6);

    diffuseMapPath = "hexagon2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon hexagon_design2(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 6);

    diffuseMapPath = "hexagon3.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon hexagon_design3(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 6);

    diffuseMapPath = "sofa.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_sofa = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "chair.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_table = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "table.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_chair = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "kitchen_box.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_kitchen_box = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "stove.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_stove = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 2.0f);

    diffuseMapPath = "oven.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_oven = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "white.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_white = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "tile.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_tile = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 4.0, 2.0f);

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_tile2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0, 2.0f);

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon polygon_star1(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 3);

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Polygon polygon_star2(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 4);

    diffuseMapPath = "grass.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_grass = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 30.0f, 30.0f);

    diffuseMapPath = "tv.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_tv = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "tv2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_tv2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "theater_floor.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube_theater_floor = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //ourShader.use();
    //lightingShader.use();

    glClearColor(1.0f, 0.8f, 0.6f, 1.0f); // Morning color

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects


        setUpLighting(lightingShader);


        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, globalTranslationMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        globalTranslationMatrix = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", globalTranslationMatrix);

        // be sure to activate shader when setting uniforms/drawing objects
        setUpLighting(lightingShaderWithTexture);
        lightingShaderWithTexture.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);


        // bezier curve
 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.5, 16.5, 0 +ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0, 8, 3.0));
        model = globalTranslationMatrix * scaleMatrix;
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.0f, 0.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(bezierCylinderVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.0, 16.5, 2 + ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0, 9.5, 3.0));
        model = globalTranslationMatrix * scaleMatrix;
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 1.0f, 0.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 1.0f, 0.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 0.0f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(bezierCylinderVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.5, 16.5, 4 +ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0, 8, 3.0));
        model = globalTranslationMatrix * scaleMatrix;
        lightingShader.use();
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.5f, 0.0f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(bezierCylinderVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(27.0, 20.5, -10 + ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0, 1.5, 1.0));
        model = globalTranslationMatrix * scaleMatrix;
        sphere.drawSphere(ourShader, model, glm::vec3(0.0f, 0.5f, 1.0f));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(30.0, 20.5, -10 + ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0, 1.5, 1.0));
        model = globalTranslationMatrix * scaleMatrix;
        sphere.drawSphere(ourShader, model, glm::vec3(1.0f, 0.0f, 1.0f));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(27.0, 20.5, -13.5 + ballonSpeed));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.0, 1.5, 1.0));
        model = globalTranslationMatrix * scaleMatrix;
        sphere.drawSphere(ourShader, model, glm::vec3(1.0f, 0.5f, 0.5f));

        ballonSpeed += 0.05;

        drawFan(globalTranslationMatrix, glm::vec3(12.0, 17.0, 10.0), glm::vec3(1.0, 1.0, 1.0), 0.0, true, lightingShaderWithTexture, ourShader, ang, cylinder_mirror, cube_table);
        drawFan(globalTranslationMatrix, glm::vec3(16.5, 17.0, 21.0), glm::vec3(1.0, 1.0, 1.0), 0.0, true, lightingShaderWithTexture, ourShader, ang, cylinder_mirror, cube_table);

        if (fanOn) {
            ang += 20;
            ang = (float)((int)ang % 360); // Keep angle within 360 degrees
        }

        // grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f, -1.2f, -30.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(100.0f, 0.7f, 100.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_grass.drawCubeWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Cafeteria Boundary ************************************************************************

        // Drink Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.0f, 7.5f, 0.5f));
        model = globalTranslationMatrix * scaleMatrix;
        //cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Design Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 30.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, 7.5f, 0.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Besin Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.5f, 7.5f, 30.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, -0.5f, 30.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Ceiling
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 7.5f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, 0.5f, 30.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Kitchen Boundary ************************************************************************

        // Right Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(18.0f, 7.5f, 0.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Far Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.5f, 7.5f, 9.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_tile2.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Near Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.5f, 7.5f, 9.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_tile2.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, -0.5f, 9.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Ceiling
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 7.5f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, 0.5f, 9.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Kitchen Box ************************************************************************

        for (int i = 0; i < 12; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 1.5, 2.5f, -8.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 0.1f, 2.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_white.drawCubeWithTexture(lightingShaderWithTexture, model);

            if (i == 2 || i == 9) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 1.5, 0.0f, -8.5f));
                scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 2.5f, 2.0f));
                model = globalTranslationMatrix * scaleMatrix;
                cube_oven.drawCubeWithTexture(lightingShaderWithTexture, model);
            }
            else if (i == 3 || i == 10);

            else {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 1.5, 0.0f, -8.5f));
                scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 2.5f, 2.0f));
                model = globalTranslationMatrix * scaleMatrix;
                cube_kitchen_box.drawCubeWithTexture(lightingShaderWithTexture, model);
            }
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.0f, 2.6f, -8.5f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, -2.6f, 2.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_white.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.1f, 0.0f, -9.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 7.5f, 6.5f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_tile2.drawCubeWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Kitchen Besin ************************************************************************

        for (int i = 0; i < 1; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 2.3f, -3.0f));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.45f, 1.5f, 0.3f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_besin.drawPolygon(lightingShaderWithTexture, model);
        }
        for (int i = 0; i < 2; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -6.0f + i * 3));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 2.0f, 3.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_besin.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Stove ************************************************************************

        for (int i = 0; i < 2; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f + i * 3.5, 2.6f, -8.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 0.1f, 2.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_stove.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Lift ************************************************************************

        // Ceiling
        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.0f, 12.0f, -12.3f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(5.0f, 0.2f, 3.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.0f, 0.0f, -12.3f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(5.0f, -0.5f, 3.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (liftMoveStill && liftMoveOff) {
            t_lift_move = 0.0f;

            glm::vec3 translation(22.5f, 0.0f, 11.0f);
            glm::vec3 rotation(0.0f, 90.0f, 0.0f); // Rotation in degrees
            glm::vec3 scale(1.0f, 1.0f, 1.0f);     // Uniform scaling

            drawLift(globalTranslationMatrix, lightingShaderWithTexture, ourShader, cube_floor, cube_wall, translation, rotation, scale);
        }

         if (!liftMoveStill && liftMoveOn) {
            glm::vec3 translation(22.5f, 0.0f + t_lift_move, 11.0f);
            glm::vec3 rotation(0.0f, 90.0f, 0.0f); // Rotation in degrees
            glm::vec3 scale(1.0f, 1.0f, 1.0f);     // Uniform scaling

            drawLift(globalTranslationMatrix, lightingShaderWithTexture, ourShader, cube_floor, cube_wall, translation, rotation, scale);

            if (t_lift_move > 8.0f) {
                liftMoveStill = true;
            }
            else {
                t_lift_move += lift_move_speed;
                liftMoveOn = true;
                liftMoveOff = false;
            }
        }

        else if (liftMoveStill && liftMoveOn) {
            t_lift_move = 0.0f;

            glm::vec3 translation(22.5f, 8.0f, 11.0f);
            glm::vec3 rotation(0.0f, 90.0f, 0.0f); // Rotation in degrees
            glm::vec3 scale(1.0f, 1.0f, 1.0f);     // Uniform scaling

            drawLift(globalTranslationMatrix, lightingShaderWithTexture, ourShader, cube_floor, cube_wall, translation, rotation, scale);
        }

        else if (!liftMoveStill && liftMoveOff) {
             glm::vec3 translation(22.5f, 8.0f - t_lift_move, 11.0f);
             glm::vec3 rotation(0.0f, 90.0f, 0.0f); // Rotation in degrees
             glm::vec3 scale(1.0f, 1.0f, 1.0f);     // Uniform scaling

             drawLift(globalTranslationMatrix, lightingShaderWithTexture, ourShader, cube_floor, cube_wall, translation, rotation, scale);

            if (t_lift_move > 8.0) {
                liftMoveStill = true;
            }
            else {
                t_lift_move += lift_move_speed;
                liftMoveOn = false;
                liftMoveOff = true;
            }
        }

        glDisable(GL_BLEND);

        // ************************************************************************ Box ************************************************************************

        for (int i = 0; i < 6; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 3, 0.0f, 0.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.0f, 2.5f, 2.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_box.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Cone Chair ************************************************************************

        for (int i = 0; i < 6; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f + i * 2.7f, 0.0f, 4.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cone_chair.drawConeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f + i * 2.7f, 1.7f, 4.5f));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cone_chair.drawConeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Besin ************************************************************************

        glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 5.0f);

        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 4.8f, 6.5f + i * 3));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.75f, 1.75f, 0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_mirror.drawPolygonWithTexture(lightingShaderWithTexture, model);
        }
        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 2.3f, 6.5f + i * 3));
            glm::mat4 rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.45f, 0.45f, 0.3f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_besin.drawPolygon(lightingShaderWithTexture, model);
        }
        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 5.0f + i * 3));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(1.5f, 2.0f, 3.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_besin.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // ************************************************************************ Design ************************************************************************


        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f + 5 * i, 5.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.65f, 0.65f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design1.drawPolygonWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f + 5 * i, 6.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.55f, 0.55f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design2.drawPolygonWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(5.8f + 5 * i, 5.2f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.45f, 0.45f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design3.drawPolygonWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(5.3f + 5 * i, 4.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.35f, 0.35f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design4.drawPolygonWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f + 5 * i, 3.8f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.25f, 0.25f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design5.drawPolygonWithTexture(lightingShaderWithTexture, model);
        }

        

        // ************************************************************************ Hexagon ************************************************************************
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 4.0f, 25.0f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.8f, 1.8f, 0.1f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design1.drawPolygonWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 2.6f, 22.25f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.5f, 1.5f, 0.1f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design2.drawPolygonWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 5.15f, 22.4f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(1.2f, 1.2f, 0.1f));
        model = globalTranslationMatrix * scaleMatrix;
        hexagon_design3.drawPolygonWithTexture(lightingShaderWithTexture, model);

        // ************************************************************************ Chair ************************************************************************

        //1st set
        
        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(1.0f, 0.0f, 13.0 + i*4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, 90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, lightingShaderWithTexture, cube_chair, cube_chair, viewPos);
        }

        //table
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(1.5f, 0.0f, 3.1f+i*4.4f);  // Translation for the table
            glm::vec3 rotation(0.0f, 0.0f, 0.0f);  // Rotation for the table (pitch, yaw, roll in degrees)
            drawTableWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cube_table);
        }

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(18.0f, 0.0f, 6.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f,-90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, lightingShaderWithTexture, cube_chair, cube_chair, viewPos);
        }


        //2nd set

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(8.0f, 0.0f, 13.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, 90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, lightingShaderWithTexture, cube_chair, cube_chair, viewPos);
        }

        //table
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(8.5f, 0.0f, 3.1f + i * 4.4f);  // Translation for the table
            glm::vec3 rotation(0.0f, 0.0f, 0.0f);  // Rotation for the table (pitch, yaw, roll in degrees)
            drawTableWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, ourShader, cube_table);
        }

        //chair
        for (int i = 0; i < 4; i++) {
            glm::vec3 translation(25.0, 0.0f, 6.0 + i * 4.4f);  // Translation for the chair
            glm::vec3 rotation(0.0f, -90.0f, 0.0f);  // Rotation for the chair (pitch, yaw, roll in degrees)
            drawChairWithTransformations(identityMatrix, globalTranslationMatrix, translation, rotation, lightingShaderWithTexture, lightingShaderWithTexture, cube_chair, cube_chair, viewPos);
        }

        //sofa
        for (int i = 0; i < 3; i++) {
            glm::vec3 sofaTranslation(-8.0f + i*5.5f, 0.0f, 22.5f);  // Translation for the sofa
            glm::vec3 sofaRotation(0.0f, 0.0f, 0.0f);  // Rotation for the sofa (pitch, yaw, roll in degrees)
            drawSofaWithTransformations(identityMatrix, globalTranslationMatrix, sofaTranslation, sofaRotation, lightingShaderWithTexture, ourShader, cube_floor, cube_sofa);
        }

        for (int j = 0; j < 3; j++) {
            // theater sofa
            for (int i = 0; i < 3; i++) {
                glm::vec3 sofaTranslation(1.5f + 3.5f * j, 8.0f + 0.5 * j, 16.0f + i * 4.5);  // Translation for the sofa
                glm::vec3 sofaRotation(0.0f, 90.0f, 0.0f);  // Rotation for the sofa (pitch, yaw, roll in degrees)
                drawSofaWithTransformations(identityMatrix, globalTranslationMatrix, sofaTranslation, sofaRotation, lightingShaderWithTexture, ourShader, cube_floor, cube_sofa);
            }

            for (int i = 0; i < 2; i++) {
                glm::vec3 sofaTranslation(1.5f + 3.5 * j, 8.0f + 0.5 * j, 34.0f + i * 4.5);  // Translation for the sofa
                glm::vec3 sofaRotation(0.0f, 90.0f, 0.0f);  // Rotation for the sofa (pitch, yaw, roll in degrees)
                drawSofaWithTransformations(identityMatrix, globalTranslationMatrix, sofaTranslation, sofaRotation, lightingShaderWithTexture, ourShader, cube_floor, cube_sofa);
            }
        }

        // ************************************************************************************************************************************************

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // ************************************************************************ Star Light ************************************************************************

        for (int j = 0; j < 6; j++) {
            for (int i = 0; i < 4; i++) {
                int ang;
                if (i % 2 == 0) ang = 0;
                else ang = 45;

                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + 3 * j, 6.8f - i * 0.7, 2.0f));
                rotateMatrix = glm::rotate(translateMatrix, glm::radians(0.0f + ang), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.25f, 0.25, 0.10f));
                model = globalTranslationMatrix * scaleMatrix;
                polygon_star1.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + 3 * j, 6.8f - i * 0.7, 2.0f));
                rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f + ang), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.25f, 0.25, 0.10f));
                model = globalTranslationMatrix * scaleMatrix;
                polygon_star1.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
            }

            for (int i = 0; i < 3; i++) {
                int ang;
                if (i % 2 == 0) ang = 0;
                else ang = 45;

                translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f + 3 * j, 6.7f - i * 0.9, 2.0f));
                rotateMatrix = glm::rotate(translateMatrix, glm::radians(0.0f + ang), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.30f, 0.30, 0.10f));
                model = globalTranslationMatrix * scaleMatrix;
                polygon_star2.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 0.0f, 0.0f));

                translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f + 3 * j, 6.7f - i * 0.9, 2.0f));
                rotateMatrix = glm::rotate(translateMatrix, glm::radians(45.0f + ang), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.30f, 0.30, 0.10f));
                model = globalTranslationMatrix * scaleMatrix;
                polygon_star2.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }

        // ************************************************************************ Light ************************************************************************

        // ************************************************************************ Wall Light ************************************************************************

        for (int i = 0; i < 4; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f + 5*i, 4.8f, 30.0f));
            //rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.4f, -0.4f, -0.1f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design5.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        // ************************************************************************ Room Corner Light ************************************************************************

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 29.75f - i*38.25f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 15.0f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            if (i == 1) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(22.25f, 0.0f, 29.75f - i * 38.25f));
            }
            else {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 29.75f - i * 38.25f));
            }
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.5f, 15.0f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        for (int i = 0; i < 2; i++) {
            glBindVertexArray(lightCubeVAO);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 7.5f, 29.75f - i * 38.25f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(46.0f, -0.5f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
        }

        // ************************************************************************ Table Light ************************************************************************

        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 6; i++) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + i * 4.0f, 5.0f, 8.0f + 8.0f * j));
                rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.425f, 0.425, -1.7f));
                model = globalTranslationMatrix * scaleMatrix;
                cylinder_design5.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }
        for (int i = 0; i < 6; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + i * 4.0f, 5.0f, -5.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.425f, 0.425, -1.7f));
            model = globalTranslationMatrix * scaleMatrix;
            cylinder_design5.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 0.0f));
        }

        // ************************************************************************ Light ************************************************************************

        float outerRadius = 0.50f;   // Outer radius of the torus
        float innerRadius = 0.25f;   // Inner radius of the torus
        int numOuterSegments = 72;  // Number of segments around the outer circle
        int numInnerSegments = 36;  // Number of segments for the "ring"

        // Outer loop for the circular path
        for (int i = 0; i < numOuterSegments; ++i) {
            float outerAngle = glm::radians(i * (360.0f / numOuterSegments)); // Outer angle in radians
            float centerX = outerRadius * cos(outerAngle); // x-coordinate of the current ring center
            float centerZ = outerRadius * sin(outerAngle); // z-coordinate of the current ring center

            // Inner loop for positioning the VAOs in a smaller circle at each outer point
            for (int j = 0; j < numInnerSegments; ++j) {
                float innerAngle = glm::radians(j * (360.0f / numInnerSegments)); // Inner angle in radians
                float x = centerX + innerRadius * cos(innerAngle) * cos(outerAngle);
                float y = 14 + innerRadius * sin(innerAngle); // Move up/down for the inner ring
                float z = 4 + centerZ + innerRadius * cos(innerAngle) * sin(outerAngle);

                // Translate and scale for each VAO
                translateMatrix = glm::translate(identityMatrix, glm::vec3(x + 2, y + 2.89, z));
                scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.05f, 0.05f, 0.05f)); // Adjust scale for smoothness
                model = globalTranslationMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                cylinder_design5.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }

        // Outer loop for the circular path
        for (int i = 0; i < numOuterSegments; ++i) {
            float outerAngle = glm::radians(i * (360.0f / numOuterSegments)); // Outer angle in radians
            float centerX = outerRadius * cos(outerAngle); // x-coordinate of the current ring center
            float centerZ = outerRadius * sin(outerAngle); // z-coordinate of the current ring center

            // Inner loop for positioning the VAOs in a smaller circle at each outer point
            for (int j = 0; j < numInnerSegments; ++j) {
                float innerAngle = glm::radians(j * (360.0f / numInnerSegments)); // Inner angle in radians
                float x = centerX + innerRadius * cos(innerAngle) * cos(outerAngle);
                float y = 14 + innerRadius * sin(innerAngle); // Move up/down for the inner ring
                float z = 27 + centerZ + innerRadius * cos(innerAngle) * sin(outerAngle);

                // Translate and scale for each VAO
                translateMatrix = glm::translate(identityMatrix, glm::vec3(x + 2, y + 2.89, z));
                scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.05f, 0.05f, 0.05f)); // Adjust scale for smoothness
                model = globalTranslationMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                cylinder_design5.drawLightPolygon(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }

        // ************************************************************************ Theater Boundary ************************************************************************

        for (int i = 0; i < 20; i++) {
            if (i < 16 || i == 19) {
                // Right Wall
                translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + i * 1.15, 8.0f, 0.0f));
                scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.575f, 10.5f, 0.5f));
                model = globalTranslationMatrix * scaleMatrix;
                cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);
            }

            //Left Wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f+i*1.15, 8.0f, 30.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.575f, 10.5f, 0.5f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        //Back Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 8.0f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.5f, 9.5f, 30.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_wall.drawCubeWithTexture(lightingShaderWithTexture, model);



        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 9.5f, 7.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 7.0f, 16.0f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_tv.drawLightCube(ourShader, model, glm::vec3(0.0f, 0.0f, 0.0f));

        if (tvOn) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 10.0f, 7.5f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 6.0f, 15.0f));
            model = globalTranslationMatrix * scaleMatrix;
            if (tv_count < 10) {
                cube_tv.drawCubeWithTexture(lightingShaderWithTexture, model);
            }
            else {
                cube_tv2.drawCubeWithTexture(lightingShaderWithTexture, model);
            }

            tv_count++;
            if (tv_count == 20) {
                tv_count = 0;
            }
        }

        // ************************************************************************ Curtain ************************************************************************


        if (curtainStill && curtainClose) {
            t_curtain = 0.0f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, 15.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, -15.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        else if (!curtainStill && curtainOpen) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, 15.0f - t_curtain));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, -15.0f + t_curtain));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            if (t_curtain >= 10.0f) {
                curtainStill = true;
            }
            else {
                t_curtain += curtain_speed;
                curtainOpen = true;
                curtainClose = false;
            }
        }

        else if (curtainStill && curtainOpen) {
            t_curtain = 0.0f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, 5.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, -5.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        else if (!curtainStill && curtainClose) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, 5.0f + t_curtain));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 8.0f, 30.0f));
            rotateMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.1f, 9.5f, -5.0f - t_curtain));
            model = globalTranslationMatrix * scaleMatrix;
            cube_curtain.drawCubeWithTexture(lightingShaderWithTexture, model);

            if (t_curtain >= 10.0f) {
                curtainStill = true;
            }
            else {
                t_curtain += curtain_speed;
                curtainOpen = false;
                curtainClose = true;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0f + t_sliding_door, 7.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0f + t_sliding_door, 9.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        for (int i = 0; i < 2; i++) {
            // Theater floar - 1
            translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0f + 3.5f*i, 8.0f, 0.5));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(3.5f, 0.5f + 0.5*i, 29.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            cube_theater_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        for (int i = 0; i < 4; i++) {
            // Theater floar - 2
            translateMatrix = glm::translate(identityMatrix, glm::vec3(16.0f + 0.7f * i, 8.0f, 0.5));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(0.7, 1.0f - 0.25 * i, 29.5f));
            model = globalTranslationMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            cube_theater_floor.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        // Ceiling
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 17.5f, 0.0f));
        scaleMatrix = glm::scale(translateMatrix, glm::vec3(23.5f, 0.5f, 30.5f));
        model = globalTranslationMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        cube_floor.drawCubeWithTexture(lightingShaderWithTexture, model);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Right Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(22.5f, 0.0f, -9.0f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(3.0f, 12.0f, 0.3f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_floor.drawCubeWithTexture(ourShader, model);

        // Left Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.2f, 0.0f, -9.0f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(3.0f, 12.0f, 0.3f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_floor.drawCubeWithTexture(ourShader, model);

        // Back Wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.7f, 0.0f, -12.0f));
        rotateMatrix = glm::rotate(translateMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(rotateMatrix, glm::vec3(0.3f, 12.0f, 5.2f));
        model = globalTranslationMatrix * scaleMatrix;
        cube_floor.drawCubeWithTexture(ourShader, model);


        glDisable(GL_BLEND);

        // ************************************************************************ Sliding Door ************************************************************************

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (doorStill && doorClose) {
            t_sliding_door = 0.0f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 7.5f, -30.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            // Front Wall
            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 9.5f, -30.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        else if (!doorStill && doorOpen) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 7.5f, -30.0f + t_sliding_door));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 9.5f, -30.0f + t_sliding_door));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            if (t_sliding_door > 23.0f) {
                doorStill = true;
            }
            else {
                t_sliding_door += sliding_door_speed;
                doorOpen = true;
                doorClose = false;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-t_sliding_door, 7.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-t_sliding_door, 9.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        else if (doorStill && doorOpen) {
            t_sliding_door = 0.0f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 7.5f, -7.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0, 7.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));


            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 9.5f, -7.0f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0, 9.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        else if (!doorStill && doorClose) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 7.5f, -7.0f - t_sliding_door));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.0));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-0.2f, 9.5f, -7.0f - t_sliding_door));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            if (t_sliding_door > 23.0f) {
                doorStill = true;
            }
            else {
                t_sliding_door += sliding_door_speed;
                doorOpen = false;
                doorClose = true;
            }

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 0.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0f + t_sliding_door, 7.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));

            translateMatrix = glm::translate(identityMatrix, glm::vec3(23.0f, 8.0f, 30.55f));
            scaleMatrix = glm::scale(translateMatrix, glm::vec3(-23.0f + t_sliding_door, 9.5f, 0.2f));
            model = globalTranslationMatrix * scaleMatrix;
            cube_wall.drawLightCube(ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        glDisable(GL_BLEND);

        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 5000, GL_UNSIGNED_INT, 0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        backgroundEnabled = true;
    }

    if (backgroundEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glClearColor(1.0f, 0.8f, 0.6f, 1.0f); // Morning color
        backgroundEnabled = false;
    }
    if (backgroundEnabled && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Noon color
        backgroundEnabled = false;
    }
    if (backgroundEnabled && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glClearColor(1.0f, 0.75f, 0.5f, 1.0f); // Afternoon color
        backgroundEnabled = false;
    }
    if (backgroundEnabled && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        glClearColor(0.9f, 0.4f, 0.3f, 1.0f); // Evening color
        backgroundEnabled = false;
    }
    if (backgroundEnabled && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // Night color
        backgroundEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) translate_Y -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) translate_Y += 0.1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) translate_X -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) translate_X += 0.1;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) translate_Z += 0.1;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) translate_Z -= 0.1;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        rotationEnabled = true;
    }

    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;

        rotationEnabled = false;
    }
    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;

        rotationEnabled = false;
    }
    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;

        rotationEnabled = false;
    }

    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        rotateAxis_X += 90;
        rotateAxis_Y = 0.0;
        rotateAngle_Z = 0.0;

        rotationEnabled = false;
    }

    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        rotateAxis_X = 0.0;
        rotateAxis_Y += 90; 
        rotateAngle_Z = 0.0;

        rotationEnabled = false;
    }

    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAngle_Z += 90;

        rotationEnabled = false;
    }

    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        rotateAngle_X -= 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;

        rotationEnabled = false;
    }
    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        rotateAngle_Y -= 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;

        rotationEnabled = false;
    }
    if (rotationEnabled && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        rotateAngle_Z -= 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;

        rotationEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        lightEnabled = true;
    }

    if (lightEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOn();
        }

        directionalLightOn = true;
        spotLightOn = true;

        lightEnabled = false;
    }
    if (lightEnabled && glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOff();
        }
        directionalLightOn = false;
        spotLightOn = false;

        lightEnabled = false;
    }
    if (lightEnabled && glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOn();
        }
        directionalLightOn = false;
        spotLightOn = false;

        lightEnabled = false;
    }
    if (lightEnabled && glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOff();
        }
        directionalLightOn = true;
        spotLightOn = true;

        lightEnabled = false;
    }
    
    if (lightEnabled && glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOff();
        }
        directionalLightOn = true;
        spotLightOn = false;

        lightEnabled = false;
    }
    if (lightEnabled && glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOn();
        }
        directionalLightOn = false;
        spotLightOn = true;

        lightEnabled = false;
    }

    if (lightEnabled && glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOff();
        }
        directionalLightOn = false;
        spotLightOn = true;

        lightEnabled = false;
    }

    if (lightEnabled && glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        for (size_t i = 0; i < pointLights.size(); ++i) {
            pointLights[i].turnOn();
        }
        directionalLightOn = true;
        spotLightOn = false;

        lightEnabled = false;
    }


    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        tvEnabled = true;
    }
    if (tvEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        tvOn = true;
        tvOff = false;

        tvEnabled = false;
    }
    if (tvEnabled && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        tvOn = false;
        tvOff = true;

        tvEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        curtainEnabled = true;
    }
    if (curtainEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (curtainStill == true && curtainClose == true) {
            curtainOpen = true;
            curtainClose = false;
            curtainStill = false;
        }

        curtainEnabled = false;
    }
    if (curtainEnabled && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        if (curtainStill == true && curtainOpen == true) {
            curtainOpen = false;
            curtainClose = true;
            curtainStill = false;
        }
        curtainEnabled = false;
    }
   
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        doorEnabled = true;
    }
    if (doorEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (doorStill == true && doorClose == true) {
            doorOpen = true;
            doorClose = false;
            doorStill = false;
        }

        doorEnabled = false;
    }
    if (doorEnabled && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        if (doorStill == true && doorOpen == true) {
            doorOpen = false;
            doorClose = true;
            doorStill = false;
        }
        doorEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        liftMoveEnabled = true;
    }
    if (liftMoveEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (liftMoveStill == true && liftMoveOff == true) {
            liftMoveOn = true;
            liftMoveOff = false;
            liftMoveStill = false;
        }
        liftMoveEnabled = false;
    }
    if (liftMoveEnabled && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        if (liftMoveStill == true && liftMoveOn == true) {
            liftMoveOn = false;
            liftMoveOff = true;
            liftMoveStill = false;
        }
        liftMoveEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fanEnabled = true;
    }

    if (fanEnabled && glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        fanOn = true;
        fanEnabled = false;
    }
    if (fanEnabled && glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        fanOn = false;
        fanEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        liftOpen = true;
        liftClose = false;
        liftStill = false;
    }


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

glm::mat4 RotationMatricesX(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around X-axis
    return glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosTheta, -sinTheta, 0.0f,
        0.0f, sinTheta, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

glm::mat4 RotationMatricesY(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around Y-axis
    return glm::mat4(
        cosTheta, 0.0f, sinTheta, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sinTheta, 0.0f, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

glm::mat4 RotationMatricesZ(float theta) {
    // Calculate cosine and sine of the angle
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // Rotation matrix around Z-axis
    return glm::mat4(
        cosTheta, -sinTheta, 0.0f, 0.0f,
        sinTheta, cosTheta, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}