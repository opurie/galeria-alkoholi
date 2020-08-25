#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=2) in vec4 color; //kolor wierzchołka
layout (location=1) in vec4 normal; //wektor normalny wierzchołka w przestrzeni modelu

out vec4 iC;
out vec4 l;
out vec4 n;
out vec4 v;

void main(void) {
    l = normalize(V * (lp - M * vertex));//znormalizowany wektor do światła w przestrzeni oka
    n = normalize(V * M * normal);//znormalizowany wektor normalny w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //Wektor do obserwatora w przestrzeni oka
    iC = color;

    gl_Position=P*V*M*vertex;
}
