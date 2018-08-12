#include "Cube.h"



Cube::Cube()
{
	srand(time(NULL));
}


Cube::~Cube()
{
}



void Cube::Draw(int glProgram, unsigned int VAO)
{
	int modelLoc = glGetUniformLocation(glProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//std::cout << position.x << " - " << position.y << std::endl;
}

bool Cube::IsCollidingWith(glm::vec3 playerPosition)
{
	//float dotUPLAYER = glm::dot(u, playerPosition);
	//float dotVPLAYER = glm::dot(v, playerPosition);
	//float dotWPLAYER = glm::dot(w, playerPosition);

	//if (dotUPLAYER < dotUP1 && dotUPLAYER > dotUP2 && dotVPLAYER < dotVP1 && dotVPLAYER > dotVP4 && dotWPLAYER < dotWP1 && dotWPLAYER > dotWP5)
	//	return true;
	//return false;

	if (playerPosition.x >= min.x && playerPosition.x <= max.x && playerPosition.y >= min.y && playerPosition.y <= max.y && playerPosition.z >= min.z && playerPosition.z <= max.z)
		return true;
	return false;
}

void Cube::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	model = glm::mat4();
	//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::translate(model, position);
	min = glm::vec3(x - 0.6f, y - 0.6f, z - 0.6f);
	max = glm::vec3(x + 0.6f, y + 0.6f, z + 0.6f);
	//p1 = glm::vec3(x - 0.5f, y - 0.5f, z - 0.5f);
	//p2 = glm::vec3(x - 0.5f, y - 0.5f, z + 0.5f);
	//p4 = glm::vec3(x + 0.5f, y - 0.5f, z + 0.5f);
	//p5 = glm::vec3(x - 0.5f, y + 0.5f, z - 0.5f);
	//u = p1 - p2;
	//v = p1 - p4;
	//w = p1 - p5;
	//dotUP1 = glm::dot(u, p1);
	//dotUP2 = glm::dot(u, p2);
	//dotVP1 = glm::dot(v, p1);
	//dotVP4 = glm::dot(v, p4);
	//dotWP1 = glm::dot(w, p1);
	//dotWP5 = glm::dot(w, p5);
}
