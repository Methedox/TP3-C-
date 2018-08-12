#include "Room.h"
#include <algorithm>



Room::Room()
{
}


Room::Room(float width, float height, float depth, glm::vec3 position, bool northDoor, bool southDoor, bool eastDoor, bool westDoor)
{
	min = glm::vec3(position.x - width * 0.5f, position.y - height * 0.5f, position.z - depth * 0.5f);
	max = glm::vec3(position.x + width * 0.5f, position.y + height * 0.5f, position.z + depth * 0.5f);
	Cube* modularCube;
	for (float x = 0; x < width; x++)
	{
		float posX = (position.x - width * 0.5f) + x;

		for (float z = 0; z < depth; z++)
		{
			float posZ = (position.z - depth * 0.5f) + z;
			modularCube = new Cube();
			modularCube->SetPosition(posX, position.y, posZ);
			currentRoom.push_back(modularCube);

			modularCube = new Cube();
			modularCube->SetPosition(posX, position.y + height, posZ);
			currentRoom.push_back(modularCube);

			if (x == 0 || x == width - 1 || ((x > 0 && x < width - 1) && z == 0 || z == depth - 1))
			{
				for (float y = 1; y < height; y++)
				{
					if (northDoor && x == (int)(width * 0.5f) && z == depth - 1 && y < 3)
						continue;
					if (southDoor && x == (int)(width * 0.5f) && z == 0 && y < 3)
						continue;
					if (eastDoor && x == width - 1 && z == (int)(depth * 0.5f) && y < 3)
						continue;
					if (westDoor && x == 0 && z == (int)(depth * 0.5f) && y < 3)
						continue;

					modularCube = new Cube();
					modularCube->SetPosition(posX, position.y + y, posZ);
					currentRoom.push_back(modularCube);
				}
			}
		}
	}
}

void Room::Draw(int glProgram, unsigned int VAO)
{
	for (size_t i = 0; i < currentRoom.size(); i++)
	{
		currentRoom[i]->Draw(glProgram, VAO);
	}
}

bool Room::IsCollidingWith(glm::vec3 playerPosition)
{
	if (playerPosition.x >= min.x && playerPosition.x <= max.x && playerPosition.y >= min.y && playerPosition.y <= max.y && playerPosition.z >= min.z && playerPosition.z <= max.z)
		return true;
	return false;
}

Room::~Room()
{
}
