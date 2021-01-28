#pragma once

#include "lib.h"
#include "Camera.h"

// Enumerations
enum shader_enums { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_CAM = 0, TEX_CAM_SPECULAR, TEX_CONTAINER, TEX_CONTAINER_SPECULAR };
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

class Game
{
private:
	// Variables

	// Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	// Opengl context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	// Delta time
	float dt;
	float curTime;
	float lastTime;

	// Mouse input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	// Camera
	Camera camera;

	// Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	// Shaders
	std::vector<Shader*> shaders;

	// Textures
	std::vector<Texture*> textures;

	// Materials
	std::vector<Material*> materials;

	// Models
	std::vector<Model*> models;

	// Lights
	std::vector<PointLight*> pointLights;

	// Private functions
	void InitGLFW();

	void InitWindow(
		const char* title, 
		bool resizable
	);

	void InitGLEW(); // After context creation!!
	void InitOpenGLOptions();
	void InitMatrices();
	void InitShaders();
	void InitTextures();
	void InitMaterials();
	void InitOBJModels();
	void InitModels();
	void InitPointLights();
	void InitLights();
	void InitUniforms();

	void UpdateUniforms();

	// Static variables

public:
	Game(const char* title,
		const int WINDOW_WIDTH,
		const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR,
		const int GL_VERSION_MINOR,
		bool resizable);
	virtual ~Game();

	// Accessors
	int GetWindowShouldClose();

	// Modifiers
	void SetWindowShouldClose();

	// Functions
	void UpdateDT();
	void UpdateMouseInput();
	void UpdateKeyboardInput();
	void UpdateInput();
	void Update();
	void Render();

	// Static functions
	static void framebuffer_resize_callBack(GLFWwindow* window, int fbw, int fbh);
};

