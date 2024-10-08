#include "lab_m1/Project/Lab5.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Lab5T2::Lab5T2(): myModelMatrix(glm::mat4(1)) {}

Lab5T2::~Lab5T2() {}

void Lab5T2::Init() {
    renderCameraTarget = false;

    camera = new implemented::CameraT2();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix =
        glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Lab5T2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab5T2::Update(float deltaTimeSeconds) {
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // TODO(student): Draw more objects with different model matrices.
    // Attention! The `RenderMesh()` function overrides the usual
    // `RenderMesh()` that we've been using up until now. This new
    // function uses the view matrix from the camera that you just
    // implemented, and the local projection matrix.

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2.f, 2.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(1.f, 1.f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, .5f, .3f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2.f, 2.5f, 3.f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(180.f), glm::vec3(0.8f, 0, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(.8f, 1.f, .9f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}

void Lab5T2::FrameEnd() {
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Lab5T2::RenderMesh(Mesh* mesh, Shader* shader,
    const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->program) return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    this->myModelMatrix = modelMatrix;
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE,
        glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE,
        glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE,
        glm::value_ptr(modelMatrix));

    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Lab5T2::OnInputUpdate(float deltaTime, int mods) {
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
         //   camera->TranslateForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
           // camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
           // camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
          //  camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
          //  camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
           // camera->TranslateUpward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_F)) {
            fov += deltaTime * cameraSpeed;
            projectionMatrix =
                glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_G)) {
            fov -= deltaTime * cameraSpeed;
            projectionMatrix =
                glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_UP)) {
            top += deltaTime * cameraSpeed;
            bottom -= deltaTime * cameraSpeed;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_DOWN)) {
            top -= deltaTime * cameraSpeed;
            bottom += deltaTime * cameraSpeed;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            left += deltaTime * cameraSpeed;
            right -= deltaTime * cameraSpeed;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_LEFT)) {
            left -= deltaTime * cameraSpeed;
            right += deltaTime * cameraSpeed;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.
}

void Lab5T2::OnKeyPress(int key, int mods) {
    // Add key press event
    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O) {
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (key == GLFW_KEY_P) {
        projectionMatrix =
            glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    }
}

void Lab5T2::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Lab5T2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}

void Lab5T2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Lab5T2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Lab5T2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Lab5T2::OnWindowResize(int width, int height) {}
