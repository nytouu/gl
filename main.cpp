#include "libs/glad.c"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"

using namespace std;

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;


Camera camera(
    glm::vec3(0.0f, 0.0f, 3.0f),
    glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f)
);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xposIn, double yposIn);

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                             glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

class Application
{
  public:
    void run()
    {
        init();
        update();
        cleanup();
    }

  private:
    Shader shader;
    GLFWwindow *window;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    unsigned int texture;

    float deltaTime;
    float lastFrame;

    void init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(WIDTH, HEIGHT, "Cubes", NULL, NULL);

        if (window == NULL)
        {
            cout << "Failed to create GLFW window" << endl;
            glfwTerminate();

            return;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            cout << "Failed to initialize GLAD" << endl;
            return;
        }

        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, WIDTH, HEIGHT);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetWindowAspectRatio(window, 8, 6);

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetCursorPosCallback(window, mouseMoveCallback);

        loadVertices();
        loadTexture();
    }

    void loadVertices()
    {
        shader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

        // vertex buffer
        glGenBuffers(1, &VBO);

        // create vertex attribute object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // send to gpu
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // define element buffer object
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        shader.use();
    }

    void loadTexture()
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image data
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *imageData = stbi_load("./assets/niko.png", &width, &height, &nrChannels, 0);

        if (imageData)
        {
            // generate opengl texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to load texture" << endl;
        }

        stbi_image_free(imageData);

        // add texcoords to vertex format
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void update()
    {
        deltaTime = 0.0f;	// Time between current frame and last frame
        lastFrame = 0.0f; // Time of last frame

        while (!glfwWindowShouldClose(window))
        {
            processInput();

            float time = glfwGetTime();
            deltaTime = time - lastFrame;
            lastFrame = time;

            float greenValue = (sin(time) / 2.0f) + 0.5f;
            float redValue = (sin(time) / 1.0f) + 0.8f;
            float blueValue = (sin(time) / 3.0f) + 0.1f;

            glClearColor(0.4f, 0.1f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            camera.updateView();

            shader.setMat4("view", camera.view);
            shader.setMat4("projection", camera.projection);

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, time, glm::vec3(0.0, 0.0, 1.0));

            shader.setMat4("transform", trans);
            shader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
            shader.setVec3("random", redValue, greenValue, blueValue);

            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);

            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        glfwTerminate();
    }

    void processInput()
    {
        // quit program
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // handle movement
        float speed = camera.speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position += speed * camera.front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position -= speed * camera.front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * speed;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.position += speed * camera.up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.position -= speed * camera.up;
    }
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    camera.moveFromMouse(xpos, ypos);
}

int main(int argc, char *argv[])
{
    Application app;

    try
    {
        app.run();
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
