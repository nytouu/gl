#include "glad.c"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

using namespace std;

const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;

const float SCREEN_RATIO = 16.0f / 9.0f;

float vertices[] = {
    0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
};

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
        loadVertices();
        loadTexture();
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

    void init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);

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

        glViewport(0, 0, WIDTH, HEIGHT);

        glfwSetWindowAspectRatio(window, 16, 9);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
        unsigned char *imageData = stbi_load("./niko.png", &width, &height, &nrChannels, 0);

        if (imageData)
        {
            // generate opengl texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to load texture" << endl;
        }

        stbi_image_free(imageData);

        // add texcoords to vertex format
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void update()
    {
        while (!glfwWindowShouldClose(window))
        {
            processInput();

            float timeValue = glfwGetTime();

            float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
            float redValue = (sin(timeValue) / 1.0f) + 0.8f;
            float blueValue = (sin(timeValue) / 3.0f) + 0.1f;

            glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
            shader.setVec3("random", redValue, greenValue, blueValue);

            /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
};

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
