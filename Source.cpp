#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderinit.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
void applyTexture(unsigned int& texture);
unsigned int texture;

void drawLampObject(Shader& lightCubeShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model);

unsigned int  cubeVAO;
unsigned int lightCubeVAO;

// light position
glm::vec3 lightPos(0.0f, 0.0f, -1.0f);

void applyTexture(unsigned int& texture);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void init(void);
void render();

void tranformations(Shader& ourShader);

// variables
const unsigned int screen_width = 1024;
const unsigned int screen_height = 768;
const GLuint NumVertices = 36;
GLuint VBO, VBO2, VBO3, VBO4, VBO5, VBO6, VBOP, VBOS, VBOL;
GLuint VAO1, VAO2, VAO3, VAO4, VAO5, VAO6, VAOP, VAOS, VAOL;
GLuint EBO;
bool goLeft = false;
float lightX = -0.5f;
bool spacePressed = false;
float sphereY = -3.1f;
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window;
    window = glfwCreateWindow(screen_width, screen_height, "OpenGLCameraAuto", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader program
    Shader ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like
    Shader lightingShader("lighting.vs", "lighting.fs");
    Shader lightCubeShader("lightsource.vs", "lightsource.fs");
    Shader Texture("Texture.vs", "Texture.fs");

    init();

    tranformations(ourShader);

    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f); // view matrix initialization
    // setting the radius variable 
    float radius = 10.0f;

    //applyTexture(texture);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        // render the background
        render();
        // Shader instance use
        ourShader.use();
        // create a circular movement
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //// Camera Position
        //glm::vec3 cameraPos = glm::vec3(camX, 0.0f, camZ);
        ////Camera Direction
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        //// Camera Right
        //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        //// Camera Up
        //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        //view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
        glm::mat4 view = glm::mat4(1.0f);
        ourShader.setMat4("view", view);


       

        // send the data to the shaders
        lightingShader.use();
        //lightingShader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
       
        // transformations
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        //draw the lamp object
       // drawLampObject(lightCubeShader, projection, view, model);

        float xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO1);
        for (unsigned int i = 0; i < 14; i++)
        {
          
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 3.4f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));
            ourShader.use();
            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //row 2
        xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO2);
        for (unsigned int i = 0; i < 14; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 2.9f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));

            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //row 3
        xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO3);
        for (unsigned int i = 0; i < 14; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 2.4f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));

            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //row 4
        xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO4);
        for (unsigned int i = 0; i < 14; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 1.9f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));

            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //row 5
        xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO5);
        for (unsigned int i = 0; i < 14; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 1.4f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));

            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //row 6
        xAxixTranslate = -4.00f;
        // render boxes
        glBindVertexArray(VAO3);
        for (unsigned int i = 0; i < 14; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.2 + i / 1.5f, 0.9f, -5.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.3f));

            float angle = 20.0f * i;
            // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //Platform
        glBindVertexArray(VAOP);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 modelP = glm::mat4(1.0f);
        modelP = glm::translate(modelP, glm::vec3(0, -3.4f, -5.0f));
        modelP = glm::scale(modelP, glm::vec3(0.9f, 0.25f, 0.3f));


        ourShader.setMat4("model", modelP);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Sphere
        if (spacePressed) {
            sphereY += 0.05;
        }
        glBindVertexArray(VAOS);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(0, sphereY, -5.0f));
        model1 = glm::scale(model1, glm::vec3(0.2f, 0.2f, 0.2f));

        // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader.setMat4("model", model1);

        glDrawArrays(GL_TRIANGLES, 0, 36);

      
        //Light
       
        glBindVertexArray(VAOL);

        lightPos.x = lightX;
        //lightPos.y = sin(glfwGetTime() / 2.0f) * 0.5f;
        
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model2 = glm::mat4(1.0f);
       
        if (!goLeft) {
            model2 = glm::translate(model, lightPos);
            lightX += 0.01f;
            if (lightPos.x >= 1) {
                goLeft = true;
            }
        }
        else {
            model2 = glm::translate(model, lightPos);
            lightX -= 0.01f;
            if (lightPos.x <= -1) {
                goLeft = false;
            }
        }
       
        model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
         
        ourShader.setMat4("model", model2);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glfw: swap buffers
        glfwSwapBuffers(window);
        // gldw: poll for events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void applyTexture(unsigned int& texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/mybox.png", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void drawLampObject(Shader& lightCubeShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model)
{
    // also draw the lamp object
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // we are scaling donw the cube to create the lamp
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void tranformations(Shader& ourShader)
{
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    ourShader.use();
    ourShader.setMat4("projection", projection);
}

void render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void init(void)
{   //row 1
    float vertices1[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //row 2
    float vertices2[] = {
 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

 -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

 -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //row 3
    float vertices3[] = {
 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

 -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

 -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,

 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //row 4
    float vertices4[] = {
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindVertexArray(VAO4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //row 5
    float vertices5[] = {
 -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.2f,
  0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.2f,
  0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,

 -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.2f,
  0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,

 -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,

  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.2f,
  0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,

 -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.2f,
 -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.2f,

 -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.2f,
 -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.2f,
    };

    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindVertexArray(VAO5);

    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices5), vertices5, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //row 6
    float vertices6[] = {
 -0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f, -0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f, -0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f, -0.5f,   0.2f, 1.5f, 0.5f,

 -0.5f, -0.5f,  0.5f,   0.2f, 1.5f, 0.5f,
  0.5f, -0.5f,  0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
  0.5f,  0.5f,  0.5f, 0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f,  0.5f,   0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f,  0.5f, 0.2f, 1.5f, 0.5f,

 -0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f, -0.5f, 0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,

  0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
  0.5f,  0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f, -0.5f, 0.2f, 1.5f, 0.5f,
  0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f,  0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f,  0.5f,   0.2f, 1.5f, 0.5f,

 -0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
  0.5f, -0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f, -0.5f, -0.5f,  0.2f, 1.5f, 0.5f,

 -0.5f,  0.5f, -0.5f,  0.2f, 1.5f, 0.5f,
  0.5f,  0.5f, -0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f,  0.5f,   0.2f, 1.5f, 0.5f,
  0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f,  0.5f,  0.2f, 1.5f, 0.5f,
 -0.5f,  0.5f, -0.5f, 0.2f, 1.5f, 0.5f,
    };

    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glBindVertexArray(VAO6);

    glBindBuffer(GL_ARRAY_BUFFER, VBO6);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices6), vertices6, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Platform
    float verticesP[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAOP);
    glGenBuffers(1, &VBOP);
    glBindVertexArray(VAOP);

    glBindBuffer(GL_ARRAY_BUFFER, VBOP);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesP), verticesP, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Sphere
    float verticesS[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &VAOS);
    glGenBuffers(1, &VBOS);
    glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesS), verticesS, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //light
    float verticesL[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAOL);
    glGenBuffers(1, &VBOL);
    glBindVertexArray(VAOL);

    glBindBuffer(GL_ARRAY_BUFFER, VBOL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesL), verticesL, GL_STATIC_DRAW);

    // position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// glfw: user input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        spacePressed = true;
    }
}

// glfw: viewport to window adjustment
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
