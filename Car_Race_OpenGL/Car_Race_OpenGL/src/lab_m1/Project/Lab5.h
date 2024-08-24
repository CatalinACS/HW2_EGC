#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Project/Lab_Camera.h"


namespace m1
{
    class Lab5T2 : public gfxc::SimpleScene
    {
    public:
        Lab5T2();
        ~Lab5T2();
        float fov = 3.75f / 4;
        float left = 0.8f, right = 7.f, bottom = 0.8f, top = 5, zNear = 0.1, zFar = 300;

        void Init() override;
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        implemented::CameraT2* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        glm::vec3 lightPosition;
        glm::mat4& myModelMatrix;
    };
}   // namespace m1