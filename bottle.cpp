#include "bottle.h"

bottle::bottle( ) {
	
	create_bottle();
}


void bottle::create_bottle() {
	float x, y, z, degree = 0.0;
	float h[4] = { 0.0, 4 * R, 5 * R,6.5 * R };
	glm::vec4 vert[4][SCIANY+1];
	double r = R;
	// wstêpne wyznaczanie wierzcho³ków
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < SCIANY+1; j++) {
			x = cos(degree) * r;
			z = sin(degree) * r;
			vert[i][j] = glm::vec4(x, h[i], z, 1);
			degree += ((360.0 / SCIANY) * (3.14159 / 180));
		}
		degree = 0.0;
		if (i == 1) r *= 1.4 / 3.0;
	}
	// wyznaczanie wierzcho³ków do rysowania poprzez GL_TRIANGLES
	int k = 0;
	for (int i = 0; i < SCIANY; i++) {
		vertices[k++] = vert[0][i];
		vertices[k++] = vert[0][i+1];
		vertices[k++] = glm::vec4(0, 0, 0, 1);
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < SCIANY; j++) {
			if (i != 3) {
				vertices[k++] = vert[i][j];
				vertices[k++] = vert[i][j+1];
				vertices[k++] = vert[i+1][j];
			}
			if (i != 0) {
				vertices[k++] = vert[i][j];
				vertices[k++] = vert[i][j + 1];
				vertices[k++] = vert[i - 1][j + 1];
			}
		}
	}
	//kolory
	for (int i = 0; i < 21 * SCIANY; i++) {
		if (i % 1 == 0) {
			colors[i++] = glm::vec4(0, 0, 1, 1);
			colors[i++] = glm::vec4(0, 0, 1, 1);
			colors[i] = glm::vec4(0, 0, 1, 1);
		}
		else {
			colors[i++] = glm::vec4(0.5, 1, 0.5, 1);
			colors[i++] = glm::vec4(0.5, 1, 0.5, 1);
			colors[i] = glm::vec4(0.5, 1, 0.5, 1);
		}
	}

	//normale œcian
	k = 0;
	for (int i = 0; i < 21 * SCIANY; i++) {
		float A, B, C, x1, x2, x3, y1, y2, y3, z1, z2, z3, len;
		glm::vec4 n;
		x1 = vertices[i].x; y1 = vertices[i].y; z1 = vertices[i++].z;
		x2 = vertices[i].x; y2 = vertices[i].y; z2 = vertices[i++].z;
		x3 = vertices[i].x; y3 = vertices[i].y; z3 = vertices[i].z;

		A = (y2 - 2 * y1) * (z3 - 2 * z1) - (z2 - 2 * z1) * (y3 - 2 * y1);
		B = (x3 - 2 * x1) * (z2 - 2 * z1) - (z3 - 2 * z1) * (x2 - 2 * x1);
		C = (x2 - 2 * x1) * (y3 - 2 * y1) - (y2 - 2 * y1) * (x3 - 2 * x1);
		len = sqrt(A*A + B*B + C*C);
		
		n = glm::vec4(A / len, B / len, C / len, 1);
		normals[k++] = n;
		normals[k++] = n;
		normals[k++] = n;
	}
}

void bottle::draw() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, vertices);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, normals);
	glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, 21*SCIANY);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

