#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad\glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "Cube.h"

class Room
{
public:
	Room();
	//position is the middle of the floor and y always at floor level
	Room(float width, float height, float depth, glm::vec3 position, bool northDoor, bool southDoor, bool eastDoor, bool westDoor);
	void Draw(int glProgram, unsigned int VAO);
	bool IsCollidingWith(glm::vec3 playerPosition);
	~Room();

	std::vector<Cube*> currentRoom;
	glm::vec3 min = glm::vec3();
	glm::vec3 max = glm::vec3();
	bool hasLoaded = false;
};

