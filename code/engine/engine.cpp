#include "engine.h"

#include <stdio.h>

#include "..\..\dependencies\glad\src\glad.c"
#include "..\..\dependencies\glfw\include\glfw3.h"

#include "renderer/renderer.cpp"
#include "renderer/orthographic_camera.h"
#include "physics/physics.h"

void FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height);

internal int
SetUpWindow(GLFWwindow **Window, int Width, int Height, char* Name)
{
    if (!glfwInit())
    {
        printf("Can't load glfw!");
        return BAD_RESULT;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *Window = glfwCreateWindow(Width, Height, Name, NULL, NULL);
    if (!*Window)
    {
        printf("Failed to create a window");
        glfwTerminate();
        return BAD_RESULT;
    }
    glfwMakeContextCurrent(*Window);

    return SUCCESS;
}

internal int
LoadGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return BAD_RESULT;
    }
    else
    {
        return SUCCESS;
    }
}

void processInput(GLFWwindow *Window, orthographic_camera *Camera, float Dt)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, true);
    }

    // Camera
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Camera->Position.Y += 1.f * Dt;
    }
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Camera->Position.Y -= 1.f * Dt;
    }
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Camera->Position.X -= 1.f * Dt;
    }
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Camera->Position.X += 1.f * Dt;
    }
}

void UpdateAndRender(GLFWwindow *Window)
{
    float DeltaTime = 0.f;
    float LastFrame = 0.f;

    color Color = { 0.4f, 0.3f, 0.35f };

    float TexCoords[] = { -1.f, -1.f, 1.f,  1.f };
    texture Texture = CreateTexture("test.jpg", GL_NEAREST, GL_REPEAT); // TODO(insolence): change path to relative
    Texture.TexCoords = TexCoords;

    orthographic_camera Camera;
    SetViewProjection(&Camera, -1.f, 1.f, -1.f, 1.f);
    
    // NOTE(insolence): Main rendering loop
    while (!glfwWindowShouldClose(Window))
    {
        float CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        processInput(Window, &Camera, DeltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(Color.R, Color.G, Color.B, 1.f);

        RecalculateViewMatrix(&Camera);

        // NOTE(insolence): Actual drawing
        DrawRectangle(&Camera, {0.0f, 0.0f}, {0.5f, 0.5f}, {0.1f, 0.3f, 0.7f});

        DrawRectangleTextured(&Camera, {-1.f, -1.f}, {0.f, 0.f}, Texture);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
}

int 
Start()
{
    printf("The Engine has started!");

    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    char* WindowName = "Insosure Engine";

    GLFWwindow* Window;
    if (!SetUpWindow(&Window, WINDOW_WIDTH, WINDOW_HEIGHT, WindowName))
    {
        return BAD_RESULT;
    }

    if (!LoadGlad())
    {
        printf("Failed to initialize GLAD!");
        return BAD_RESULT;
    }

    glViewport(0, 0, 1280, 720);

    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    UpdateAndRender(Window);

    glfwTerminate(); // TODO: Pass somewhere else or remove
    return SUCCESS;
}


///// NOTE: Callbacks for OpenGL /////////////// 

void 
FramebufferSizeCallback(GLFWwindow *Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
} 
