#pragma once

#include <vector>
#include "components/simple_scene.h"
#include "lab_m1/Project/myObject2D.h"
#include "lab_m1/Project/transform2D.h"
#include "lab_m1/Project/Lab_Camera.h"


namespace m1
{
    class Project : public gfxc::SimpleScene
    {
     public:
        Project();
        ~Project();

        void Init() override;
        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
        void CreatePoints(std::vector<glm::vec3> vect, std::vector<VertexFormat> &vertices, std::vector<unsigned int> &indices);
        void CreateScene(float deltaTimeSeconds);
        void decideFunc(int i, glm::vec3& K1, glm::vec3& K2, glm::vec3& K3);
        void initCameraPlayer();
        void initMiniMap();
        bool canMove(glm::vec3 position, glm::vec3 dist, int offset);
        bool carDetection(glm::vec3 position);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        //void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3& color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::vec3 carAng;
        float rotAng;
        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;
        float fov;
        Mesh* triangle;
        std::vector<glm::vec3> vect;
        float windowRatio;
        float scale;
        std::vector<glm::vec3> pointsTrian;
        std::vector<glm::vec3> pointsTree;
        std::vector<glm::vec3> finalPoints;
        std::vector<std::pair<glm::vec3, glm::vec3>> obsPath;
        std::vector<std::pair<int, glm::vec3>> obsPos;
        glm::vec4 pos;
        std::vector<glm::vec3> carPosition;
        std::vector<glm::vec3> randColor;
        int cars;
        implemented::CameraT2* camera;
        implemented::CameraT2* minimap_camera;
        implemented::CameraT2* main_camera;
        implemented::CameraT2 aux_cam;
        glm::mat4 projectionMatrix;
    };
}   // namespace m1
