#ifndef BOTTLE_H
#define BOTTLE_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shaderprogram.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>


#define SCIANY 10
using namespace std;

class bottle {
private:
	float R=1;
	

public:
	glm::vec4* vertices = new glm::vec4[21* SCIANY];
	glm::vec4* colors = new glm::vec4[21 * SCIANY];
	glm::vec4* normals = new glm::vec4[21 * SCIANY];
	glm::vec4* vertexNormals = new glm::vec4[21 * SCIANY];

	bottle();
	void create_bottle();
	void draw();
};
#endif