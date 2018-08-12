#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad\glad.h"
#include <GLFW/glfw3.h>
#include <time.h>
#include <iostream>

class Cube
{
public:
	Cube();
	~Cube();
	void SetPosition(float x, float y, float z);
	void Draw(int glProgram, unsigned int VAO);
	bool IsCollidingWith(glm::vec3 playerPosition);
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 w;
	glm::vec3 min;
	glm::vec3 max;
private:
	float dotUP1 = 0;
	float dotUP2 = 0;
	float dotVP1 = 0;
	float dotVP4 = 0;
	float dotWP1 = 0;
	float dotWP5 = 0;
	glm::vec3 p1 = glm::vec3();
	glm::vec3 p2 = glm::vec3();
	glm::vec3 p4 = glm::vec3();
	glm::vec3 p5 = glm::vec3();
};

