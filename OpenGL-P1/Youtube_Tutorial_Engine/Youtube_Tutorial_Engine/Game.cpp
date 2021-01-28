#include "Game.h"

// Private functions
void Game::InitGLFW()
{
	//Init GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

// Private functions
void Game::InitWindow(
	const char* title,
	bool resizable
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT,
		title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_resize_callBack);

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); // IMPORTANT for glew
}

void Game::InitGLEW()
{
	glewExperimental = GL_TRUE;

	// Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::InitOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::InitMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::InitShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::InitTextures()
{
	// Texture 0
	this->textures.push_back(new Texture ("images/cam.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture ("images/cam_specular.png", GL_TEXTURE_2D));

	// Texture 1
	this->textures.push_back(new Texture ("images/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture ("images/container_specular.png", GL_TEXTURE_2D));
}

void Game::InitMaterials()
{
	this->materials.push_back(new Material(glm::vec3(.1f), glm::vec3(1.f), glm::vec3(1.f), 
		0, 1));
}

void Game::InitOBJModels()
{
}

void Game::InitModels()
{
	std::vector<Mesh*> meshes;
	std::vector<Mesh*> meshes2;

	meshes.push_back(
		new Mesh(
			&Pyramid(),
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);

	meshes.push_back(
		new Mesh(
			&Quad(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);

	meshes2.push_back(
		new Mesh(
			&Quad(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(-90.f, 0.f, 0.f),
			glm::vec3(100.f)
		)
	);

	this->models.push_back(new Model(
		glm::vec3(0.f, 1.f, 1.f),
		this->materials[0],
		this->textures[TEX_CAM],
		this->textures[TEX_CAM_SPECULAR],
		meshes
		)
	);

	this->models.push_back(new Model(
		glm::vec3(0.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
		)
	);

	this->models.push_back(new Model(
		glm::vec3(2.f, 0.f, 2.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
		)
	);

	this->models.push_back(new Model(
		glm::vec3(2.f, -5.f, 2.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes2
	)
	);

	this->models.push_back(new Model(
		glm::vec3(4.f, 0.f, 4.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		"OBJFiles/eyeball.obj"
	)
	);

	for (auto*& i : meshes)
		delete i;

	for (auto*& i : meshes2)
		delete i;
}

void Game::InitPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f)));
}

void Game::InitLights()
{
	this->InitPointLights();
}

void Game::InitUniforms()
{
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	for each (PointLight* pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
}

void Game::UpdateUniforms()
{
	// Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPostion(), "cameraPos");

	for each (PointLight * pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	// Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

Game::Game(const char* title,
	const int WINDOW_WIDTH,
	const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR,
	const int GL_VERSION_MINOR,
	bool resizable
) 
	: 
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR), 
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	// Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3 (0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.0f;
	this->curTime = 0.0f;
	this->lastTime = 0.0f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->InitGLFW();
	this->InitWindow(title, resizable);
	this->InitGLEW();
	this->InitOpenGLOptions();
	this->InitMatrices();
	this->InitShaders();
	this->InitTextures();
	this->InitMaterials();
	this->InitOBJModels();
	this->InitModels();
	this->InitLights();
	this->InitUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];
}

// Accessor
int Game::GetWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

// Modifier
void Game::SetWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Functions
void Game::UpdateDT()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::UpdateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	// Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	// Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	// Move light
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		this->pointLights[0]->SetPosition(this->camera.getPostion());
	}
}

void Game::UpdateKeyboardInput()
{
	// Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	// Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.Move(this->dt, FORWARD);
	}

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.Move(this->dt, BACKWARD);
	}

	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.Move(this->dt, LEFT);
	}

	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.Move(this->dt, RIGHT);
	}

	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y += 0.05f;
	}

	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.05f;
	}
}

void Game::UpdateInput()
{
	glfwPollEvents();

	this->UpdateKeyboardInput();
	this->UpdateMouseInput();
	this->camera.UpdateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::Update()
{
	// Update input
	this->UpdateInput();
	this->UpdateDT();

	// Constant rotation
	//this->models[0]->Rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[1]->Rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[2]->Rotate(glm::vec3(0.f, 1.f, 0.f));
}

void Game::Render()
{
	// Update
	//updateInput(window);

	// Draw
	// Clear
	glClearColor(0.f, 0.f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Update uniforms
	this->UpdateUniforms();

	//Draw
	for (auto&i : this->models)
		i->Render(this->shaders[SHADER_CORE_PROGRAM]);

	// End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Static functions
void Game::framebuffer_resize_callBack(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
}