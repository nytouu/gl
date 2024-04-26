#include "libs/glad.c"
#include "model.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>
#include <vector>

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f));

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseMoveCallback(GLFWwindow *window, double xposIn, double yposIn);

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

    float deltaTime;
    float lastFrame;

    std::vector<Model> models;

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
    }

    void update()
    {
        deltaTime = 0.0f; // Time between current frame and last frame
        lastFrame = 0.0f; // Time of last frame

        Model ourModel("./assets/backpack/backpack.obj");

        while (!glfwWindowShouldClose(window))
        {
            processInput();

            float time = glfwGetTime();
            deltaTime = time - lastFrame;
            lastFrame = time;

            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setMat4("view", camera.view);
            shader.setMat4("projection", camera.projection);
            camera.updateView();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
            shader.setMat4("model", model);
            ourModel.Draw(shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup()
    {
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

void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
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
