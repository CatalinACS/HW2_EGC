#include "lab_m1/Project/Project.h"
#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Project::Project()
{
	windowRatio = window->props.aspectRatio;
	rotAng = 0;
	scale = 20.f;
}


Project::~Project()
{
}


void Project::Init()
{

	{
		Mesh* mesh = new Mesh("plane50");
		mesh->LoadMesh(
			PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
			"plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		vect.push_back(glm::vec3(6, 0, 16));
		vect.push_back(glm::vec3(7, 0, 16));
		vect.push_back(glm::vec3(8, 0, 15.5f));
		vect.push_back(glm::vec3(9, 0, 15));
		vect.push_back(glm::vec3(10, 0, 14.75f));
		vect.push_back(glm::vec3(11, 0, 15));
		vect.push_back(glm::vec3(12, 0, 15.5f));
		vect.push_back(glm::vec3(13, 0, 16.01f));
		vect.push_back(glm::vec3(14, 0, 16.01f));
		vect.push_back(glm::vec3(15, 0, 15.51f));
		vect.push_back(glm::vec3(16, 0, 15.01f));
		vect.push_back(glm::vec3(17, 0, 14.f));
		vect.push_back(glm::vec3(16.5, 0, 13.f));
		vect.push_back(glm::vec3(16.25, 0, 12.f));
		vect.push_back(glm::vec3(16, 0, 11.f));
		vect.push_back(glm::vec3(15, 0, 10.5f));
		vect.push_back(glm::vec3(14, 0, 10));
		vect.push_back(glm::vec3(13, 0, 10));
		vect.push_back(glm::vec3(12, 0, 10.5f));
		vect.push_back(glm::vec3(11, 0, 11.01f));
		vect.push_back(glm::vec3(10, 0, 11.25f));
		vect.push_back(glm::vec3(9, 0, 11.f));
		vect.push_back(glm::vec3(8, 0, 10.5f));
		vect.push_back(glm::vec3(7, 0, 10.01f));
		vect.push_back(glm::vec3(6, 0, 10.01f));
		vect.push_back(glm::vec3(5, 0, 10.51f));
		vect.push_back(glm::vec3(4, 0, 11.01f));
		vect.push_back(glm::vec3(3, 0, 12));
		vect.push_back(glm::vec3(3.25, 0, 13));
		vect.push_back(glm::vec3(3.55, 0, 14));
		vect.push_back(glm::vec3(4, 0, 15.02f));
		vect.push_back(glm::vec3(5, 0, 15.53f));
	}

	{
		vector<VertexFormat> vertices;
		vector<unsigned int> indices;
		CreatePoints(vect, vertices, indices);
		CreateMesh("road", vertices, indices);
		meshes["road"]->SetDrawMode(GL_TRIANGLE_STRIP);
		glm::vec4 red;
		glm::vec4 blue;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-8, 0.425, -8));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		for (int i = 0; i < vertices.size(); i++)
		{
			if (i % 2 == 0)
			{
				red = glm::vec4(meshes["road"]->vertices[i].position, 1);
				red = modelMatrix * red;
				pointsTrian.push_back(glm::vec3(red.x, red.y, red.z));
			}
			else
				if (i % 2 == 1)
				{
					blue = glm::vec4(meshes["road"]->vertices[i].position, 1);
					blue = modelMatrix * blue;
					pointsTrian.push_back(glm::vec3(blue.x, blue.y, blue.z));
				}
		}

		for (int i = 0; i < pointsTree.size(); i++)
		{
			if (i % 2 == 0)
			{
				red = glm::vec4(pointsTree[i], 1);
				red = modelMatrix * red;
				finalPoints.push_back(glm::vec3(red.x, red.y, red.z));
			}
			else
				if (i % 2 == 1)
				{
					blue = glm::vec4(pointsTree[i], 1);
					blue = modelMatrix * blue;
					finalPoints.push_back(glm::vec3(blue.x, blue.y, blue.z));
				}
		}
	}

	camera = new implemented::CameraT2();
	minimap_camera = new implemented::CameraT2();
	camera->Set(glm::vec3(-5.85, 5.53, 0.015), glm::vec3(0, 3.5, 0), glm::vec3(0, 1, 0));
	initCameraPlayer();
	minimap_camera->Set(glm::vec3(-2, 5.53, 0), glm::vec3(0, -10.5, 0), glm::vec3(0, 1, 0));
	initMiniMap();

	fov = RADIANS(60);
	left = -56.f;
	right = 56.f;
	bottom = -27.f;
	top = 27.f;
	zNear = 0.01f;
	zFar = 200.f;

	// Create a simple cube
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
			VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
			VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
			VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
		};
		vector<unsigned int> indices =
		{
			0, 1, 2,        1, 3, 2,
			2, 3, 7,        2, 7, 6,
			1, 7, 3,        1, 5, 7,
			6, 7, 4,        7, 5, 4,
			0, 4, 1,        1, 4, 5,
			2, 6, 4,        0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);

		CreateMesh("car", vertices, indices);
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist4(-3,3);
	std::uniform_int_distribution<int> dist5(0, vect.size() / 2 - 3);
	std::uniform_int_distribution<int> dist6(vect.size() / 2 + 2, vect.size() - 1);
	std::uniform_int_distribution<int> dist7(0, 0);
	std::vector<std::uniform_int_distribution<int>> distVec;
	distVec.push_back(dist5);
	distVec.push_back(dist6);
	std::uniform_int_distribution<int> dist8(3, 7);
	std::uniform_real_distribution<float> dist9(0.f, 1.f);
	cars = dist8(rng);
	carPosition = std::vector<glm::vec3>(cars);
	randColor = std::vector<glm::vec3>(cars);
	int random = 0;
	glm::vec3 P0, P1;
	for (int i = 0; i < cars; i++)
	{
		randColor[i] = glm::vec3(dist9(rng), dist9(rng), dist9(rng));
		int random = (distVec[dist7(rng)])(rng);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-8, 0.425, -8));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		pos = glm::vec4(vect[random], 1);
		pos = modelMatrix * pos;

		P0 = glm::vec3(pos.x, pos.y, pos.z);

		if (random == 0)
		{
			pos = glm::vec4(vect[vect.size() - 1], 1);
			pos = modelMatrix * pos;
			P1 = glm::vec3(pos.x, pos.y, pos.z);
		}
		else
		{
			pos = glm::vec4(vect[(double)random - 1], 1);
			pos = modelMatrix * pos;
			P1 = glm::vec3(pos.x, pos.y, pos.z);
		}
		obsPos.push_back(std::make_pair(random, P1 - glm::vec3(0, 0, dist4(rng) + 0.5)));
		obsPath.push_back(std::make_pair(P0, P1));
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("LabShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Project", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Project", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}


Mesh* Project::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
	unsigned int VAO = 0;
	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// Set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// Set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// Set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Project::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void takeValue(int i, glm::vec3& P1, glm::vec3& P2, std::vector<glm::vec3> vect, glm::vec3& P0 = glm::vec3(0, 0, 0))
{
	switch (i)
	{
	case 0:
	{
		P0 = vect[vect.size() - 1];
		P1 = vect[0];
		P2 = vect[1];
	}
	break;
	default:
	{
		if (i == vect.size() - 1)
		{
			P0 = vect[(double)i - 1];
			P1 = vect[i];
			P2 = vect[0];
		}
		else
		{
			P0 = vect[(double)i - 1];
			P1 = vect[i];
			P2 = vect[(double)i + 1];
		}
	}
	}
}

void Project::CreatePoints(vector<glm::vec3> vect, vector<VertexFormat>& vertices, vector<unsigned int>& indices)
{
	glm::vec3 D, UP = glm::vec3(0, 1, 0), P, R, A;
	glm::vec3 P1, P2, P0;
	int discret = 360;
	int discretTree = 3;
	float dX, dZ;
	for (int i = 0; i < vect.size(); i++)
	{
		takeValue(i, P1, P2, vect);
		dX = (P2.x - P1.x) / discret;
		dZ = (P2.z - P1.z) / discret;
		for (int j = 0; j < discret; j++)
		{
			D = P2 - P1;
			P = glm::normalize(glm::cross(D, UP));
			R = P1 + glm::vec3(0.25 * P.x, 0.25 * P.y, 0.25 * P.z);
			A = P1 - glm::vec3(0.25 * P.x, 0.25 * P.y, 0.25 * P.z);
			vertices.push_back(VertexFormat(R));
			vertices.push_back(VertexFormat(A));
			P1.x += dX;
			P1.z += dZ;
		}
		takeValue(i, P1, P2, vect, P0);
		D = P2 - P0;
		P = glm::normalize(glm::cross(D, UP));
		R = P1 + glm::vec3(0.43 * P.x, 0.43 * P.y, 0.43 * P.z);
		A = P1 - glm::vec3(0.43 * P.x, 0.43 * P.y, 0.43 * P.z);
		pointsTree.push_back(R);
		pointsTree.push_back(A);
		dX = (P2.x - P1.x) / (discretTree - 1);
		dZ = (P2.z - P1.z) / (discretTree - 1);
		P1.x += dX;
		P1.z += dZ;

		for (int j = 1; j < discretTree - 1; j++)
		{
			D = P2 - P1;
			P = glm::normalize(glm::cross(D, UP));
			R = P1 + glm::vec3(0.43 * P.x, 0.43 * P.y, 0.43 * P.z);
			A = P1 - glm::vec3(0.43 * P.x, 0.43 * P.y, 0.43 * P.z);
			pointsTree.push_back(R);
			pointsTree.push_back(A);
			P1.x += dX;
			P1.z += dZ;
		}

	}
	for (int i = 0; i < vertices.size(); i++)
		indices.push_back(i);
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);
	indices.push_back(0);
	indices.push_back(1);
}

void Project::CreateScene(float deltaTimeSeconds)
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-8, 0.425, -8));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		RenderSimpleMesh(meshes["road"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		for (auto iter : finalPoints)
		{
			float y = camera->GetTargetPosition().y;
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(iter.x, y - 1.1, iter.z));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 2.5, 0.5f));
				RenderSimpleMesh(meshes["cube"], shaders["LabShader"], modelMatrix, glm::vec3(0.32f, 0.2f, 0.06f));
			}

			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(iter.x, y - 1.1, iter.z) + glm::vec3(0, 4, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5, 1.5, 1.5));
				RenderSimpleMesh(meshes["cube"], shaders["LabShader"], modelMatrix, glm::vec3(0.05f, 0.68f, 0.14f));
			}
		}

		for (int i = 0; i < cars; i++)
		{
			glm::vec3 P0, P1;

			P0 = obsPath[i].first;
			P1 = obsPath[i].second;

			float speedCar = 3.455;

			glm::vec3 D = P0 - P1;
			glm::vec3 P = obsPos[i].second;
			carPosition[i] = P;

			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(obsPos[i].second.x, 0.78f, obsPos[i].second.z));
				modelMatrix = glm::rotate(modelMatrix, atan(D.x / D.z) + RADIANS(180), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 1.5f));
				RenderSimpleMesh(meshes["cube"], shaders["LabShader"], modelMatrix, randColor[i]);
			}

			if (D.z < 0)
			{
				obsPos[i].second.x += (double) speedCar * deltaTimeSeconds * sin(atan(D.x / D.z) - M_PI);
				obsPos[i].second.z += (double) speedCar * deltaTimeSeconds * cos(atan(D.x / D.z) - M_PI);

			}
			else

			{
				obsPos[i].second.x += (double) speedCar * deltaTimeSeconds * sin(atan(D.x / D.z));
				obsPos[i].second.z += (double) speedCar * deltaTimeSeconds * cos(atan(D.x / D.z));
			}

			if (abs(obsPos[i].second.x - obsPath[i].first.x) <= 0.1f) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-8, 0.425, -8));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
				obsPath[i].second = obsPath[i].first;
				if (obsPos[i].first == vect.size() - 1)
				{
					obsPos[i].first = 0;
					pos = glm::vec4(vect[obsPos[i].first], 1);
					pos = modelMatrix * pos;
					obsPath[i].first = glm::vec3(pos.x, pos.y, pos.z);
				}
				else
				{
					obsPos[i].first++;
					pos = glm::vec4(vect[obsPos[i].first], 1);
					pos = modelMatrix * pos;
					obsPath[i].first = glm::vec3(pos.x, pos.y, pos.z);

				}

			}

		}

	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		pos = glm::vec4(meshes["car"]->vertices[0].position, 1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition() - glm::vec3(0, 0.45, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, rotAng, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 1.5f));
		pos = modelMatrix * pos;
		RenderSimpleMesh(meshes["car"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.28, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
		RenderSimpleMesh(meshes["plane50"], shaders["LabShader"], modelMatrix, glm::vec3(0.48f, 0.91f, 0.43f));
	}
}

void Project::Update(float deltaTimeSeconds)
{
	main_camera = camera;
	projectionMatrix =
		glm::perspective(fov, windowRatio, zNear, zFar);
	CreateScene(deltaTimeSeconds);
	if (window->GetResolution().x > 0 && window->GetResolution().y > 0)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(window->GetResolution().x - 384, -75, 600 * window->GetResolution().x / (window->GetResolution().x + window->GetResolution().y),
			950 * window->GetResolution().y / (window->GetResolution().x + window->GetResolution().y));
		minimap_camera->position = camera->GetTargetPosition();
		main_camera = minimap_camera;
		projectionMatrix = glm::ortho(left, right, bottom, top, zNear - 50, zFar);
		CreateScene(deltaTimeSeconds);
	}

}

bool PointTriangle(glm::vec3 p, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	float s = (p0.x - p2.x) * (p.z - p2.z) - (p0.z - p2.z) * (p.x - p2.x);
	float t = (p1.x - p0.x) * (p.z - p0.z) - (p1.z - p0.z) * (p.x - p0.x);

	if ((s < 0) != (t < 0) && s != 0 && t != 0)
		return false;

	float d = (p2.x - p1.x) * (p.z - p1.z) - (p2.z - p1.z) * (p.x - p1.x);
	return d == 0 || (d < 0) == (s + t <= 0);
}


void Project::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	//"Model"

	GLint modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	//"View"

	GLint modelView = glGetUniformLocation(shader->GetProgramID(), "View");
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(main_camera->GetViewMatrix()));

	// "Player"

	GLint locCar = glGetUniformLocation(shader->program, "carPos");
	glUniform3fv(locCar, 1, glm::value_ptr(camera->GetTargetPosition() - glm::vec3(0, 1.1, 0)));

	// "Color"

	GLint locObj = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObj, 1, glm::value_ptr(color));

	//"Projection"

	GLint projectionLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Project::decideFunc(int i, glm::vec3& K1, glm::vec3& K2, glm::vec3& K3)
{
	if (i == pointsTrian.size() - 1)
	{
		K1 = pointsTrian[i];
		K2 = pointsTrian[0];
		K3 = pointsTrian[1];
	}
	else
		if (i == pointsTrian.size() - 2)
		{
			K1 = pointsTrian[i];
			K2 = pointsTrian[(double)i + 1];
			K3 = pointsTrian[0];
		}
		else
		{
			K1 = pointsTrian[i];
			K2 = pointsTrian[(double)i + 1];
			K3 = pointsTrian[(double)i + 2];
		}
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

bool Project::canMove(glm::vec3 position, glm::vec3 dist, int offset)
{
	for (int i = 0; i < pointsTrian.size(); i++)
	{
		glm::vec3 K1, K2, K3;
		decideFunc(i, K1, K2, K3);
		if (PointTriangle(position + dist, glm::vec3(K1.x + offset, K1.y, K1.z + offset),
			glm::vec3(K2.x + offset, K2.y, K2.z + offset), glm::vec3(K3.x + offset, K3.y, K3.z + offset)))
		{
			return true;
		}
	}
	return false;
}

bool Project::carDetection(glm::vec3 position)
{
	for (int i = 0; i < cars; i++)
	{
		float dist = sqrt(glm::dot(position - carPosition[i], position - carPosition[i]));
		if (dist <= 1.25f)
			return true;
	}
	return false;
}


void Project::OnInputUpdate(float deltaTime, int mods)
{
	int speed = 15;
	if (window->KeyHold(GLFW_KEY_W)) {
		int offset = 5;
		glm::vec3 position = glm::vec3(camera->GetTargetPosition().x + offset, camera->GetTargetPosition().y, camera->GetTargetPosition().z + offset);
		glm::vec3 dist = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
		if (canMove(position - glm::vec3(0, 1.1, 0), dist, offset) &&
			!carDetection(glm::vec3(position.x - offset, position.y, position.z - offset) - glm::vec3(0, 1.1, 0)))
		{
			camera->MoveForward(speed * deltaTime);

		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		int offset = -5;
		glm::vec3 position = glm::vec3(camera->GetTargetPosition().x + offset, camera->GetTargetPosition().y, camera->GetTargetPosition().z + offset);
		glm::vec3 dist = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
		if (canMove(position - glm::vec3(0, 1.1, 0), -dist, offset) &&
			!carDetection(glm::vec3(position.x - offset, position.y, position.z - offset) - glm::vec3(0, 1.1, 0)))
		{
			camera->MoveForward(-speed * deltaTime);

		}
	}

	if (window->KeyHold(GLFW_KEY_D) && window->KeyHold(GLFW_KEY_S)) {
		if (!carDetection(camera->GetTargetPosition() - glm::vec3(0, 1.1, 0)))
		{
			rotAng += (speed / 10 + 0.5) * deltaTime;
			camera->RotateThirdPerson_OY((speed / 10 + 0.5) * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_D) && !window->KeyHold(GLFW_KEY_S)) {
		if (!carDetection(camera->GetTargetPosition() - glm::vec3(0, 1.1, 0)))
		{
			rotAng -= (speed / 10 + 0.5) * deltaTime;
			camera->RotateThirdPerson_OY(-(speed / 10 + 0.5) * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_A) && window->KeyHold(GLFW_KEY_S)) {
		if (!carDetection(camera->GetTargetPosition() - glm::vec3(0, 1.1, 0)))
		{
			rotAng -= (speed / 10 + 0.5) * deltaTime;
			camera->RotateThirdPerson_OY(-(speed / 10 + 0.5) * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_A) && !window->KeyHold(GLFW_KEY_S)) {
		if (!carDetection(camera->GetTargetPosition() - glm::vec3(0, 1.1, 0)))
		{
			rotAng +=( speed / 10 + 0.5) * deltaTime;
			camera->RotateThirdPerson_OY((speed / 10 + 0.5) * deltaTime);
		}
	}
}


void Project::OnKeyPress(int key, int mods)
{
	
}


void Project::OnKeyRelease(int key, int mods)
{
	
}


void Project::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	

}


void Project::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

}


void Project::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Project::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Project::OnWindowResize(int width, int height)
{

}

void Project::initCameraPlayer()
{
	rotAng -= 1.5;
	camera->RotateThirdPerson_OY(-1.5);
	camera->MoveForward(450);
	rotAng -= 3.15;
	camera->RotateThirdPerson_OY(-3.15);
	camera->MoveForward(50);
	rotAng -= 1.15;
	camera->RotateThirdPerson_OY(-1.15);
	rotAng -= 0.95;
	camera->RotateThirdPerson_OY(-0.95);
	camera->MoveForward(375);
	rotAng -= 4;
	camera->RotateThirdPerson_OY(-4);
	camera->MoveForward(385);
	rotAng += 1;
	camera->RotateThirdPerson_OY(1);
}


void Project::initMiniMap()
{
	minimap_camera->RotateThirdPerson_OY(-1.5);
	minimap_camera->MoveForward(450);
	minimap_camera->RotateThirdPerson_OY(-3.15);
	minimap_camera->MoveForward(50);
	minimap_camera->RotateThirdPerson_OY(-1.15);
	minimap_camera->RotateThirdPerson_OY(-0.95);
	minimap_camera->MoveForward(375);
	minimap_camera->RotateThirdPerson_OY(-4);
	minimap_camera->MoveForward(385);
	minimap_camera->RotateThirdPerson_OY(1);
}
