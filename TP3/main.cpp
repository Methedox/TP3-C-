#include <iostream>
#include "glad\glad.h"
#include <string>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW\glfw3native.h>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"
#include "Room.h"
#include <vector>


HGLRC loaderContext;
HDC hDC;

void SetCurrentRoom(Room* room1, Room* room2, Room* room3, Room* room4, bool broom1, bool broom2, bool broom3, bool broom4, GLuint& currentTexture, bool& transition);
void SetCurrentRoom(Room* room1, Room* room2, Room* room3, Room* room4, bool broom1, bool broom2, bool broom3, bool broom4, GLuint& currentTexture);
bool SetRoom1Texture(unsigned int& texture);
bool SetRoom2Texture(unsigned int& texture);
bool SetRoom3Texture(unsigned int& texture);
bool SetRoom4Texture(unsigned int& texture);

std::string readFileStr(std::string path)
{
	std::ifstream fileStream;
	fileStream.open(path);

	std::stringstream strStream;
	strStream << fileStream.rdbuf();

	fileStream.close();

	return strStream.str();
}

int CreateShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	// --------------------------------------------------
	// VERTEX SHADER
	//
	std::string vertexFile = readFileStr(vertexShaderPath);
	const char* vertexShaderSource = vertexFile.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	{
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
	// --------------------------------------------------


	// --------------------------------------------------
	// FRAGMENT SHADER
	//
	std::string fragmentFile = readFileStr(fragmentShaderPath);
	const char* fragmentShaderSource = fragmentFile.c_str();

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	{
		int success;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
	// --------------------------------------------------

	// --------------------------------------------------
	// CREATE SHADER PROGRAM
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	{
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// --------------------------------------------------

	return shaderProgram;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "TP3", NULL, NULL);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	hDC = GetDC(glfwGetWin32Window(window));
	loaderContext = wglCreateContext(hDC);
	HGLRC mainContext = wglGetCurrentContext();
	wglShareLists(loaderContext, mainContext);

	glEnable(GL_DEPTH_TEST);

	int lightingProgram = CreateShaderProgram("lighting.vs", "lighting.fs");
	int textureProgram = CreateShaderProgram("shader.vs", "shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	/*
	unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
	};*/

	unsigned int VAO, VBO;// , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// -----------------------

	//stbi_set_flip_vertically_on_load(true);

	glViewport(0, 0, 800, 600);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


	float startCameraHeight = 1.6f;
	glm::vec3 cameraPos = glm::vec3(0.0f, startCameraHeight, -2.0f);
	glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 maxY = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 maxX = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 rotDir = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec4 moveDir = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 cameraOffset = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

	glm::mat4 cameraRotation = glm::mat4();

	const int ROW = 7;
	const int COL = 7;
	double lastMouseX, lastMouseY;
	lastMouseX = -1;
	lastMouseY = -1;
	float lastTime = 0.0f;
	float tick = 0.0f;
	float speed = 0.07f;
	float runSpeed = 0.105f;
	float currentSpeed = 0.0f;
	float rotspeed = 0.03f;
	float currentRotation = 0.0f;
	float cameraTime = 0.0f;
	float headbobOffset = 0.0f;
	float HBWave = 60.0f;
	float HBTime = 0.0f;
	bool isWalking = false;
	bool isRunning = false;
	float transition = 0;
	bool inTransition = false;

	GLuint currentTexture = 0;
	GLuint textureBuffer = 0;

	//Create Rooms
	Room* room1 = new Room(11.0f, 4.0f, 11.0f, glm::vec3(), true, false, false, false);
	Room* room1toroom2 = new Room(3.0f, 3.0f, 4.0f, glm::vec3(0.0f, 0.0f, 7.5f), true, true, false, false);
	Room* room2 = new Room(11.0f, 4.0f, 11.0f, glm::vec3(0.0f, 0.0f, 15.0f), false, true, true, true);
	Room* room2toroom3 = new Room(4.0f, 3.0f, 3.0f, glm::vec3(-7.5f, 0.0f, 15.0f), false, false, true, true);
	Room* room2toroom4 = new Room(4.0f, 3.0f, 3.0f, glm::vec3(7.5f, 0.0f, 15.0f), false, false, true, true);
	Room* room3 = new Room(11.0f, 4.0f, 11.0f, glm::vec3(-15.0f, 0.0f, 15.0f), false, false, true, false);
	Room* room4 = new Room(11.0f, 4.0f, 11.0f, glm::vec3(15.0f, 0.0f, 15.0f), false, false, false, true);
	SetCurrentRoom(room1, room2, room3, room4, true, false, false, false, currentTexture);

	// Create a vector for all the cube in the map and allocate enough space to hold them
	std::vector<Cube*> mapCube;
	mapCube.reserve(room1->currentRoom.size() + room1toroom2->currentRoom.size() +
		room2->currentRoom.size() + room2toroom3->currentRoom.size() +
		room2toroom4->currentRoom.size() + room3->currentRoom.size() +
		room4->currentRoom.size());

	// Populate the vector with all the cube in the map
	mapCube.insert(mapCube.end(), room1->currentRoom.begin(), room1->currentRoom.end());
	mapCube.insert(mapCube.end(), room1toroom2->currentRoom.begin(), room1toroom2->currentRoom.end());
	mapCube.insert(mapCube.end(), room2->currentRoom.begin(), room2->currentRoom.end());
	mapCube.insert(mapCube.end(), room2toroom3->currentRoom.begin(), room2toroom3->currentRoom.end());
	mapCube.insert(mapCube.end(), room2toroom4->currentRoom.begin(), room2toroom4->currentRoom.end());
	mapCube.insert(mapCube.end(), room3->currentRoom.begin(), room3->currentRoom.end());
	mapCube.insert(mapCube.end(), room4->currentRoom.begin(), room4->currentRoom.end());

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float currentTime = (float)glfwGetTime();
		float delta = currentTime - lastTime;
		lastTime = currentTime;

		if (inTransition)
		{
			if (transition < 1.0f)
				transition += delta;
			else
			{
				transition = 1.0f;
				inTransition = false;
				glDeleteTextures(1, &currentTexture);
				currentTexture = textureBuffer;
				transition = 0.0f;
			}
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);


		isWalking = false;
		isRunning = false;
		moveDir = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		rotDir = glm::vec3(0.0f, 0.0f, 0.0f);

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			isRunning = true;


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			moveDir.z = 1.0f;
			isWalking = true;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			moveDir.x = 1.0f;
			isWalking = true;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			moveDir.z = -1.0f;
			isWalking = true;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			moveDir.x = -1.0f;
			isWalking = true;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotDir.y = 1.0f;
			currentRotation += rotDir.y * rotspeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotDir.y = -1.0f;
			currentRotation += rotDir.y * rotspeed;
		}

		if (isWalking)
		{
			if (isRunning)
			{
				currentSpeed = runSpeed;
				HBWave = 40.0f;
				HBTime = 15.0f;
			}
			else
			{
				currentSpeed = speed;
				HBTime = 10.0f;
				HBWave = 60.0f;
			}
			//Set Camera Headbob
			cameraTime += delta * HBTime;
			headbobOffset = sin(cameraTime);
		}
		else
		{
			headbobOffset = 0.0f;
		}

		int leftMouseBtn = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftMouseBtn == GLFW_PRESS)
		{
			if (lastMouseX != -1 && lastMouseY != -1)
			{
				double deltaMouseX = mouseX - lastMouseX;

				if (deltaMouseX > 0)
					rotDir.y = -1.0f;
				else if (deltaMouseX < 0)
					rotDir.y = 1.0f;
				else
					rotDir.y = 0.0f;

				currentRotation += rotDir.y * rotspeed *1.2f;
			}
			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
		else
		{
			lastMouseX = -1;
			lastMouseY = -1;
		}


		//Set the camera position
		glm::vec3 prevPos = cameraPos;

		cameraPos += (glm::vec3(glm::rotate(cameraRotation, currentRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * moveDir * currentSpeed)) + glm::vec3(0.0f, headbobOffset / HBWave, 0.0f);

		for (size_t i = 0; i < mapCube.size(); i++)
		{
			if (mapCube[i]->IsCollidingWith(cameraPos))
			{
				cameraPos = prevPos;
				break;
			}
		}
		if (room1->IsCollidingWith(cameraPos) && !room1->hasLoaded)
		{
			SetCurrentRoom(room1, room2, room3, room4, true, false, false, false, textureBuffer, inTransition);
		}
		else if (room2->IsCollidingWith(cameraPos) && !room2->hasLoaded)
		{
			SetCurrentRoom(room1, room2, room3, room4, false, true, false, false, textureBuffer, inTransition);
		}
		else if (room3->IsCollidingWith(cameraPos) && !room3->hasLoaded)
		{
			SetCurrentRoom(room1, room2, room3, room4, false, false, true, false, textureBuffer, inTransition);
		}
		else if (room4->IsCollidingWith(cameraPos) && !room4->hasLoaded)
		{
			SetCurrentRoom(room1, room2, room3, room4, false, false, false, true, textureBuffer, inTransition);
		}

		//Set the camera looking point (Always towards the camera)
		targetPos = cameraPos + (glm::vec3(glm::rotate(cameraRotation, currentRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * cameraOffset));


		// -----------------------------------------------
		// ------ INITIATE THE BACKGROUND COLOR
		// -----------------------------------------------
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// -----------------------------------------------
		// ------------ SHADER WITH LIGHTS
		// -----------------------------------------------

		glActiveTexture(GL_TEXTURE0);
		glUseProgram(lightingProgram);
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		glUniform1i(glGetUniformLocation(lightingProgram, "texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glUseProgram(lightingProgram);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		glUniform1i(glGetUniformLocation(lightingProgram, "texture2"), 1);

		glUniform1f(glGetUniformLocation(lightingProgram, "transition"), transition);

		int projectionUniformLocation = glGetUniformLocation(lightingProgram, "projection");
		glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view;
		view = glm::lookAt(cameraPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));

		int viewUniformLocation = glGetUniformLocation(lightingProgram, "view");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		int lightPosUniformLocation = glGetUniformLocation(lightingProgram, "lightPos");
		glUniform3f(lightPosUniformLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		int viewPosUniformLocation = glGetUniformLocation(lightingProgram, "viewPos");
		glUniform3f(viewPosUniformLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		int lightColorUniformLocation = glGetUniformLocation(lightingProgram, "lightColor");
		glUniform3f(lightColorUniformLocation, 0.5f, 0.5f, 0.5f);

		// -----------------------------------------------
		// ---------------- Draw Room
		// -----------------------------------------------

		room1->Draw(lightingProgram, VAO);
		room1toroom2->Draw(lightingProgram, VAO);
		room2->Draw(lightingProgram, VAO);
		room2toroom3->Draw(lightingProgram, VAO);
		room2toroom4->Draw(lightingProgram, VAO);
		room3->Draw(lightingProgram, VAO);
		room4->Draw(lightingProgram, VAO);

		// -----------------------------------------------
		// ---------- SHADER WITHOUT LIGHTS
		// -----------------------------------------------

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		//glUseProgram(textureProgram);
		//glUniform1i(glGetUniformLocation(textureProgram, "texture1"), 1);
		////glUniform1i(glGetUniformLocation(textureProgram, "texture2"), 1);

		//int transitionUniformLocation = glGetUniformLocation(textureProgram, "transition");
		//glUniform1f(transitionUniformLocation, 1.0f);


		//int texProjectionUniformLocation = glGetUniformLocation(textureProgram, "projection");
		//glUniformMatrix4fv(texProjectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

		////glm::mat4 view;
		////view = glm::lookAt(cameraPos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f));

		//int texViewUniformLocation = glGetUniformLocation(textureProgram, "view");
		//glUniformMatrix4fv(texViewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		glfwSwapBuffers(window);
	}
}


bool SetRoom1Texture(unsigned int &texture)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nbChannel;
	unsigned char *data = stbi_load("rock0.jpg", &width, &height, &nbChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "cannot load image" << std::endl;
	}
	stbi_image_free(data);
	return true;
}

bool SetRoom2Texture(unsigned int& texture)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nbChannel;
	unsigned char *data = stbi_load("rock1.jpg", &width, &height, &nbChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "cannot load image" << std::endl;
	}
	stbi_image_free(data);
	return true;
}

bool SetRoom3Texture(unsigned int& texture)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nbChannel;
	unsigned char *data = stbi_load("rock2.jpg", &width, &height, &nbChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "cannot load image" << std::endl;
	}
	stbi_image_free(data);
	return true;
}

bool SetRoom4Texture(unsigned int& texture)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nbChannel;
	unsigned char *data = stbi_load("rock3.jpg", &width, &height, &nbChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "cannot load image" << std::endl;
	}
	stbi_image_free(data);
	return true;
}

void SetCurrentRoom(Room* room1, Room* room2, Room* room3, Room* room4, bool broom1, bool broom2, bool broom3, bool broom4, GLuint& currentTexture)
{
	bool dummy = false;
	SetCurrentRoom(room1, room2, room3, room4, broom1, broom2, broom3, broom4, currentTexture, dummy);
}

void SetCurrentRoom(Room* room1, Room* room2, Room* room3, Room* room4, bool broom1, bool broom2, bool broom3, bool broom4, GLuint& currentTexture, bool& transition)
{
	if (broom1)
	{
		room1->hasLoaded = true;
		room2->hasLoaded = false;
		room3->hasLoaded = false;
		room4->hasLoaded = false;
		std::thread load_texture_1([&] {
			wglMakeCurrent(hDC, loaderContext);
			transition = SetRoom1Texture(currentTexture);
			wglMakeCurrent(NULL, NULL);
		});
		load_texture_1.detach();
		if (load_texture_1.joinable())
		{
			load_texture_1.join();
		}
	}
	else if (broom2)
	{
		room1->hasLoaded = false;
		room2->hasLoaded = true;
		room3->hasLoaded = false;
		room4->hasLoaded = false;
		std::thread load_texture_1([&] {
			wglMakeCurrent(hDC, loaderContext);
			transition = SetRoom2Texture(currentTexture);
			wglMakeCurrent(NULL, NULL);
		});
		load_texture_1.detach();
		if (load_texture_1.joinable())
		{
			transition = true;
			load_texture_1.join();
		}
	}
	else if (broom3)
	{
		room1->hasLoaded = false;
		room2->hasLoaded = false;
		room3->hasLoaded = true;
		room4->hasLoaded = false;
		std::thread load_texture_1([&] {
			wglMakeCurrent(hDC, loaderContext);
			transition = SetRoom3Texture(currentTexture);
			wglMakeCurrent(NULL, NULL);
		});
		load_texture_1.detach();
		if (load_texture_1.joinable())
		{
			transition = true;
			load_texture_1.join();
		}
	}
	else if (broom4)
	{
		room1->hasLoaded = false;
		room2->hasLoaded = false;
		room3->hasLoaded = false;
		room4->hasLoaded = true;
		std::thread load_texture_1([&] {
			wglMakeCurrent(hDC, loaderContext);
			transition = SetRoom4Texture(currentTexture);
			wglMakeCurrent(NULL, NULL);
		});
		load_texture_1.detach();
		if (load_texture_1.joinable())
		{
			transition = true;
			load_texture_1.join();
		}
	}
}