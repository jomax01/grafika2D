//Autor: Nedeljko Tesanovic
//Opis: Primjer upotrebe tekstura

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread> 
#include <chrono> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define CRES 30

enum MicrowaveState { ON, OFF_CLOSED, OFF_OPENED, BROKEN, FINISHED };

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

void shiftLeftTime(string& time, char newDigit);
void printMicrowaveTime(string microwaveTime, unsigned textures[]);
void extractMinutesSeconds(string& time, float& minutes, float& seconds);
bool updateTime(string& time, float& minutes, float& seconds, float timeElapsed);
bool validateTime(string& time);
void printDugmici(unsigned textures[]);
void  creatCircleCoord(float lampica[], float r, float center_x, float center_y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void checkTextureClick(float mouseX, float mouseY);
void printErrorMessage(unsigned textures[]);

void startMicrowave();
void resetMicrovawe();
void stopMicrowave();

string microwaveTime;
MicrowaveState state;
float minutes;
float seconds;


int main(void)
{

	if (!glfwInit())
	{
		std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	unsigned int wWidth = 900;
	unsigned int wHeight = 900;
	const char wTitle[] = "[Generic Title]";
	window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Prozor nije napravljen! :(\n";
		glfwTerminate();
		return 2;
	}

	glfwMakeContextCurrent(window);


	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW nije mogao da se ucita! :'(\n";
		return 3;
	}

	unsigned int textureShader = createShader("texture.vert", "texture.frag");
	unsigned int uniformTextureShader = createShader("texture.vert", "uniformTexture.frag");
	unsigned int basicShader = createShader("basic.vert", "basic.frag");
	unsigned int uniformColorShader = createShader("basic.vert", "uniformColor.frag");
	//unsigned int smokeShader = createShader("uniformPosition.vert", "uniformColor.frag");
	unsigned int smokeShader = createShader("uniformSmoke.vert", "uniformColor.frag");


#include <GL/glew.h>
#include <GLFW/glfw3.h>

	//mikrotalasna spolja
	float mtSpoljaVertices[] = {
		-0.7, -0.5,       0.9, 0.9, 0.9, 1.0,
		0.7, -0.5,       0.9, 0.9, 0.9, 1.0,
		-0.7, 0.5,       0.9, 0.9, 0.9, 1.0,
		0.7, 0.5,       0.9, 0.9, 0.9, 1.0,
	};

	//mikrotalasna unutra
	float mtUnutraVertices[] = {
		-0.6, -0.3,
		0.1, -0.3,
		-0.6, 0.3,
		0.1, 0.3,
	};

	//mikrotalasna vrata
	float mtVrataClosedVertices[] = {
		-0.6, -0.3,       0.0, 0.7, 1.0, 0.5,
		0.1, -0.3,       0.0, 0.7, 1.0, 0.5,
		-0.6, 0.3,       0.0, 0.7, 1.0, 0.5,
		0.1, 0.3,       0.0, 0.7, 1.0, 0.5,
	};

	//mikrotalasna vrata
	float mtVrataOpenedVertices[] = {
		-0.95, -0.45,       0.0, 0.7, 1.0, 0.5,
		-0.6, -0.3,       0.0, 0.7, 1.0, 0.5,
		-0.95, 0.45,       0.0, 0.7, 1.0, 0.5,
		-0.6, 0.3,       0.0, 0.7, 1.0, 0.5,
	};

	//tanjir
	float tanjirVertices[] = {
		-0.5, -0.2,             0.0, 0.0,
		0.0, -0.2,              1.0, 0.0,
		-0.5, 0.1,              0.0, 1.0,
		0.0, 0.1,				1.0, 1.0,

	};


	//dugmici
	float dugmeSirina = 0.1;
	float dugmici[] = {
		//0
		0.3, -0.3,                                0.0, 0.0,
		0.3 + dugmeSirina, -0.3,                    1.0, 0.0,
		0.3, -0.3 + dugmeSirina,                    0.0, 1.0,
		0.3 + dugmeSirina, -0.3 + dugmeSirina,        1.0, 1.0,

		//1
		0.2, 0.0,                                0.0, 0.0,
		0.2 + dugmeSirina, 0.0,                    1.0, 0.0,
		0.2, 0.0 + dugmeSirina,                    0.0, 1.0,
		0.2 + dugmeSirina, 0.0 + dugmeSirina,        1.0, 1.0,

		//2
		0.3, 0.0,                                0.0, 0.0,
		0.3 + dugmeSirina, 0.0,                    1.0, 0.0,
		0.3, 0.0 + dugmeSirina,                    0.0, 1.0,
		0.3 + dugmeSirina, 0.0 + dugmeSirina,        1.0, 1.0,

		//3
		0.4, 0.0,                                0.0, 0.0,
		0.4 + dugmeSirina, 0.0,                    1.0, 0.0,
		0.4, 0.0 + dugmeSirina,                    0.0, 1.0,
		0.4 + dugmeSirina, 0.0 + dugmeSirina,        1.0, 1.0,

		//4
		0.2, -0.1,                                0.0, 0.0,
		0.2 + dugmeSirina, -0.1,                    1.0, 0.0,
		0.2, -0.1 + dugmeSirina,                    0.0, 1.0,
		0.2 + dugmeSirina, -0.1 + dugmeSirina,        1.0, 1.0,

		//5
		0.3, -0.1,                                0.0, 0.0,
		0.3 + dugmeSirina, -0.1,                    1.0, 0.0,
		0.3, -0.1 + dugmeSirina,                    0.0, 1.0,
		0.3 + dugmeSirina, -0.1 + dugmeSirina,        1.0, 1.0,

		//6
		0.4, -0.1,                                0.0, 0.0,
		0.4 + dugmeSirina, -0.1,                    1.0, 0.0,
		0.4, -0.1 + dugmeSirina,                    0.0, 1.0,
		0.4 + dugmeSirina, -0.1 + dugmeSirina,        1.0, 1.0,

		//7
		0.2, -0.2,                                0.0, 0.0,
		0.2 + dugmeSirina, -0.2,                    1.0, 0.0,
		0.2, -0.2 + dugmeSirina,                    0.0, 1.0,
		0.2 + dugmeSirina, -0.2 + dugmeSirina,        1.0, 1.0,

		//8
		0.3, -0.2,                                0.0, 0.0,
		0.3 + dugmeSirina, -0.2,                    1.0, 0.0,
		0.3, -0.2 + dugmeSirina,                    0.0, 1.0,
		0.3 + dugmeSirina, -0.2 + dugmeSirina,        1.0, 1.0,

		//9
		0.4, -0.2,                                0.0, 0.0,
		0.4 + dugmeSirina, -0.2,                    1.0, 0.0,
		0.4, -0.2 + dugmeSirina,                    0.0, 1.0,
		0.4 + dugmeSirina, -0.2 + dugmeSirina,        1.0, 1.0,


	};

	//start stop rst
	float dugmeStart[] = {
		0.2, 0.1,                                0.0, 0.0,
		0.2 + dugmeSirina, 0.1,                    1.0, 0.0,
		0.2, 0.1 + dugmeSirina,                    0.0, 1.0,
		0.2 + dugmeSirina, 0.1 + dugmeSirina,        1.0, 1.0,
	};

	float dugmeStop[] = {
		0.3, 0.1,                                0.0, 0.0,
		0.3 + dugmeSirina, 0.1,                    1.0, 0.0,
		0.3, 0.1 + dugmeSirina,                    0.0, 1.0,
		0.3 + dugmeSirina, 0.1 + dugmeSirina,        1.0, 1.0,
	};

	float dugmeReset[] = {
		0.4, 0.1,                                0.0, 0.0,
		0.4 + dugmeSirina, 0.1,                    1.0, 0.0,
		0.4, 0.1 + dugmeSirina,                    0.0, 1.0,
		0.4 + dugmeSirina, 0.1 + dugmeSirina,        1.0, 1.0,
	};

	//tajmer
	float tajmerSirina = 0.06;
	float tajmerVisina = 0.1;

	float tajmerVertices[] = {
		//pozicija 1
		0.2, 0.3,                                0.0, 0.0,
		0.2 + tajmerSirina, 0.3,                    1.0, 0.0,
		0.2, 0.3 + tajmerVisina,                    0.0, 1.0,
		0.2 + tajmerSirina, 0.3 + tajmerVisina,        1.0, 1.0,

		//pozicija 2
		0.2 + tajmerSirina, 0.3,                      0.0, 0.0,
		0.2 + 2 * tajmerSirina, 0.3,                    1.0, 0.0,
		0.2 + tajmerSirina, 0.3 + tajmerVisina,         0.0, 1.0,
		0.2 + 2 * tajmerSirina, 0.3 + tajmerVisina,       1.0, 1.0,

		//razmak
		0.2 + 2 * tajmerSirina, 0.3,                    0.0, 0.0,
		0.2 + 3 * tajmerSirina, 0.3,                    1.0, 0.0,
		0.2 + 2 * tajmerSirina, 0.3 + tajmerVisina,       0.0, 1.0,
		0.2 + 3 * tajmerSirina, 0.3 + tajmerVisina,       1.0, 1.0,

		//pozicija 3
		0.2 + 3 * tajmerSirina, 0.3,                    0.0, 0.0,
		0.2 + 4 * tajmerSirina, 0.3,                    1.0, 0.0,
		0.2 + 3 * tajmerSirina, 0.3 + tajmerVisina,       0.0, 1.0,
		0.2 + 4 * tajmerSirina, 0.3 + tajmerVisina,       1.0, 1.0,

		//pozicija 4
		0.2 + 4 * tajmerSirina, 0.3,                    0.0, 0.0,
		0.2 + 5 * tajmerSirina, 0.3,                    1.0, 0.0,
		0.2 + 4 * tajmerSirina, 0.3 + tajmerVisina,       0.0, 1.0,
		0.2 + 5 * tajmerSirina, 0.3 + tajmerVisina,       1.0, 1.0,
	};

	float slajder[] =
	{
	   0.505, -0.8,   1.0, 1.0, 0.0, 1.0,
	   0.515, -0.8,   1.0, 1.0, 0.0, 1.0,
	};
	unsigned int strideSlajder = (2 + 4) * sizeof(float);

	//lampice
	float lampica1[CRES * 2 + 4];
	float lampica2[CRES * 2 + 4];
	float r = 0.05;
	creatCircleCoord(lampica1, r, -0.25, 0.25);
	creatCircleCoord(lampica2, r, 0.6, 0.35);

	// The stride is the same for all rectangles
	unsigned int strideTexture = (2 + 2) * sizeof(float); //x y s t
	unsigned int stride = (2 + 4) * sizeof(float); //x y r g b a

	//dim
	float smoke1[CRES * 2 + 4];
	float smoke2[CRES * 2 + 4];
	float smoke3[CRES * 2 + 4];
	float smoke4[CRES * 2 + 4];
	float smoke5[CRES * 2 + 4];
	float rSmoke1 = 0.065;
	float rSmoke2 = 0.065;
	float rSmoke3 = 0.07;
	float rSmoke4 = 0.075;
	float rSmoke5 = 0.06;

	creatCircleCoord(smoke5, rSmoke5, 0.0, 0.65);
	creatCircleCoord(smoke1, rSmoke1, -0.1, 0.55);
	creatCircleCoord(smoke2, rSmoke2, 0.1, 0.57);
	creatCircleCoord(smoke3, rSmoke3, -0.15, 0.75);
	creatCircleCoord(smoke4, rSmoke4, 0.12, 0.8);


	float kvar[] = {
			-1.0, -1.0,
			-1.0, 1.0,
			1.0, -1.0,
			1.0, 1.0,
	};

	float ime[] = {
	0.3,  -1.0,   0.0, 0.0,
	1.0, -1.0,    1.0, 0.0,
	0.3, -0.9,    0.0, 1.0,
	1.0, -0.9,    1.0, 1.0,
	};
	unsigned VAO[20]; //0 - star, 1 - rectangle
	glGenVertexArrays(20, VAO);
	unsigned VBO[20];
	glGenBuffers(20, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mtSpoljaVertices), mtSpoljaVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mtUnutraVertices), mtUnutraVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mtVrataOpenedVertices), mtVrataOpenedVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tajmerVertices), tajmerVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mtVrataClosedVertices), mtVrataClosedVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tanjirVertices), tanjirVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dugmici), dugmici, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dugmeStart), dugmeStart, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dugmeStop), dugmeStop, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dugmeReset), dugmeReset, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lampica1), lampica1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lampica2), lampica2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smoke1), smoke1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smoke2), smoke2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smoke3), smoke3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smoke4), smoke4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(smoke5), smoke5, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[17]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[17]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(kvar), kvar, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[18]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[18]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ime), ime, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, strideTexture, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Tekstura
	unsigned textures[10];	//brojevi

	for (int i = 0; i < 10; ++i) {
		string filePath = "res/num" + to_string(i) + ".PNG";

		textures[i] = loadImageToTexture(filePath.c_str());

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0); // Clear binding
	}

	unsigned colonTexture = loadImageToTexture("res/colon.PNG");
	glBindTexture(GL_TEXTURE_2D, colonTexture); //Podesavamo teksturu
	glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
	glBindTexture(GL_TEXTURE_2D, 0);      // da se ocisti

	unsigned tanjirTexture = loadImageToTexture("res/plate2.PNG");
	glBindTexture(GL_TEXTURE_2D, tanjirTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned startTexture = loadImageToTexture("res/start.PNG");
	glBindTexture(GL_TEXTURE_2D, startTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned stopTexture = loadImageToTexture("res/stop.PNG");
	glBindTexture(GL_TEXTURE_2D, stopTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned resetTexture = loadImageToTexture("res/reset.PNG");
	glBindTexture(GL_TEXTURE_2D, resetTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned eTexture = loadImageToTexture("res/e.PNG");
	glBindTexture(GL_TEXTURE_2D, eTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned rTexture = loadImageToTexture("res/r.PNG");
	glBindTexture(GL_TEXTURE_2D, rTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned oTexture = loadImageToTexture("res/o.PNG");
	glBindTexture(GL_TEXTURE_2D, oTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned errorTextures[5] = { eTexture, rTexture, rTexture, oTexture, rTexture };

	unsigned imeTexture = loadImageToTexture("res/ime.PNG");
	glBindTexture(GL_TEXTURE_2D, imeTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	microwaveTime = "  :  ";
	state = OFF_CLOSED;
	float timeHelper = 0.0;
	minutes = 0;
	seconds = 0;

	glUseProgram(textureShader);
	unsigned uTexLoc = glGetUniformLocation(textureShader, "uTex");
	glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
	glUseProgram(0);
	//Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
	//Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje

	const int targetFPS = 60;
	const double frameTime = 1.0 / targetFPS;

	// Vreme početka petlje
	double previousTime = glfwGetTime();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int uniformColorLoc = glGetUniformLocation(uniformColorShader, "uColor");

	unsigned int smokeColorLoc = glGetUniformLocation(smokeShader, "uColor");
	unsigned int smokePositionLoc = glGetUniformLocation(smokeShader, "uPos");
	unsigned int smokeCenterLoc = glGetUniformLocation(smokeShader, "center");




	float alpha;
	float smokeMovement;
	bool providnaVrata = true;

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();

		// Izračunaj proteklo vreme
		double elapsedTime = currentTime - previousTime;

		if (elapsedTime < frameTime) {
			std::this_thread::sleep_for(
				std::chrono::duration<double>(frameTime - elapsedTime));
			continue;
		}

		previousTime = currentTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			providnaVrata = true;

		}
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			providnaVrata = false;
		}
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			state = BROKEN;
			alpha = 0.5f;
			smokeMovement = 0.0f;
		}

		if (providnaVrata) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else {
			glDisable(GL_BLEND);

		}

		if (state == OFF_CLOSED) {

			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			{
				state = OFF_OPENED;
			}
		}
		else if (state == ON) {
			
			if (!updateTime(microwaveTime, minutes, seconds, elapsedTime)) {
				state = FINISHED;
			}

			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			{
				state = OFF_OPENED;
			}
		}

		else if (state == OFF_OPENED) {
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			{
				state = OFF_CLOSED;
			}
		}

		else if (state == BROKEN) {
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			{
				state = OFF_CLOSED;
				resetMicrovawe();
			}
		}

		else if (state == FINISHED) {
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			{
				state = OFF_OPENED;
			}
		}

		glUseProgram(textureShader);
		glBindVertexArray(VAO[5]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tanjirTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);


		
		//************************************************************        DRAWING         ************************************************************

		glClearColor(0.0, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(basicShader);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(mtSpoljaVertices) / stride);



		glUseProgram(uniformColorShader);
		if (state == ON) {
			glBindVertexArray(VAO[1]);
			glUniform4f(uniformColorLoc, 0.9, 0.9, 0, 1);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(mtUnutraVertices) / (2 * sizeof(float)));


			glBindVertexArray(VAO[10]);
			glUniform4f(uniformColorLoc, 1, 1, 0, 1);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(lampica1) / (2 * sizeof(float)));

		}
		else {
			glBindVertexArray(VAO[1]);
			glUniform4f(uniformColorLoc, 0.1, 0.1, 0.1, 1);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(mtUnutraVertices) / (2 * sizeof(float)));


			glBindVertexArray(VAO[10]);
			glUniform4f(uniformColorLoc, 0, 0, 0, 1);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(lampica1) / (2 * sizeof(float)));

		}

		//tanjir
		glUseProgram(textureShader);
		glBindVertexArray(VAO[5]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tanjirTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(basicShader);
		if (state == OFF_OPENED) {
			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(mtVrataOpenedVertices) / stride);
		}
		else {
			glBindVertexArray(VAO[4]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(mtVrataClosedVertices) / stride);
		}

		//tajmer
		if (state != BROKEN) {
			glUseProgram(textureShader);
			glBindVertexArray(VAO[3]);
			printMicrowaveTime(microwaveTime, textures);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colonTexture);
			glDrawArrays(GL_TRIANGLE_STRIP, 2 * 4, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//dugmici
		glUseProgram(textureShader);
		glBindVertexArray(VAO[6]);
		printDugmici(textures);

		glUseProgram(textureShader);
		glBindVertexArray(VAO[7]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, startTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(VAO[8]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, stopTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(VAO[9]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, resetTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(VAO[11]);
		glUseProgram(uniformColorShader);
		if (state == ON) {
			glUniform4f(uniformColorLoc, abs(sin(glfwGetTime())), 0.5 * abs(sin(glfwGetTime())), 0, 1);
		}
		else if (state == FINISHED) {
			glUniform4f(uniformColorLoc, 0, 1, 0, 1);
		}
		else {
			glUniform4f(uniformColorLoc, 0, 0, 0, 1);
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(lampica2) / (2 * sizeof(float)));

		glUseProgram(textureShader);
		glBindVertexArray(VAO[18]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, imeTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (state == BROKEN) {
			glUseProgram(uniformTextureShader);
			glUniform1f(glGetUniformLocation(uniformTextureShader, "time"), glfwGetTime());
			glBindVertexArray(VAO[3]);
			printErrorMessage(errorTextures);

			glUseProgram(smokeShader);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			alpha -= 0.005f;
			smokeMovement += 0.005f;
			if (alpha < 0.0f) alpha = 0.0f;
			glUniform4f(smokeColorLoc, 0.5f, 0.5f, 0.5f, alpha);

			glUniform2f(smokePositionLoc, -smokeMovement, smokeMovement);

			glBindVertexArray(VAO[12]);
			glUniform2f(smokeCenterLoc, smoke1[0], smoke1[1]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(smoke1) / (2 * sizeof(float)));

			glBindVertexArray(VAO[14]);
			glUniform2f(smokeCenterLoc, smoke3[0], smoke3[1]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(smoke3) / (2 * sizeof(float)));

			glUniform2f(smokePositionLoc, smokeMovement, smokeMovement);
			glBindVertexArray(VAO[13]);
			glUniform2f(smokeCenterLoc, smoke2[0], smoke2[1]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(smoke2) / (2 * sizeof(float)));

			glBindVertexArray(VAO[15]);
			glUniform2f(smokeCenterLoc, smoke4[0], smoke4[1]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(smoke4) / (2 * sizeof(float)));

			glUniform2f(smokePositionLoc, 0, smokeMovement);
			glBindVertexArray(VAO[16]);
			glUniform2f(smokeCenterLoc, smoke5[0], smoke5[1]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(smoke5) / (2 * sizeof(float)));

			//zatamljenje
			glUseProgram(uniformColorShader);
			glBindVertexArray(VAO[17]);
			glUniform4f(uniformColorLoc, 0, 0, 0, 0.55 - alpha);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(kvar) / (2 * sizeof(float)));
		}

		



		///////////////////////////////////////////////////////////////////////////////////
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int i = 0; i < 10; i++) {
		glDeleteTextures(1, &textures[i]);
	}

	glDeleteBuffers(1, &VBO[20]);
	glDeleteVertexArrays(1, &VAO[20]);
	glDeleteProgram(textureShader);

	glfwTerminate();
	return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
	std::string content = "";
	std::ifstream file(source);
	std::stringstream ss;
	if (file.is_open())
	{
		ss << file.rdbuf();
		file.close();
		std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
	}
	else {
		ss << "";
		std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
	}
	std::string temp = ss.str();
	const char* sourceCode = temp.c_str();

	int shader = glCreateShader(type);

	int success;
	char infoLog[512];
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (type == GL_VERTEX_SHADER)
			printf("VERTEX");
		else if (type == GL_FRAGMENT_SHADER)
			printf("FRAGMENT");
		printf(" sejder ima gresku! Greska: \n");
		printf(infoLog);
	}
	return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{

	unsigned int program;
	unsigned int vertexShader;
	unsigned int fragmentShader;

	program = glCreateProgram();

	vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
	fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
		std::cout << infoLog << std::endl;
	}

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	return program;
}
static unsigned loadImageToTexture(const char* filePath) {
	int TextureWidth;
	int TextureHeight;
	int TextureChannels;
	unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
	if (ImageData != NULL)
	{
		//Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
		stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

		// Provjerava koji je format boja ucitane slike
		GLint InternalFormat = -1;
		switch (TextureChannels) {
		case 1: InternalFormat = GL_RED; break;
		case 2: InternalFormat = GL_RG; break;
		case 3: InternalFormat = GL_RGB; break;
		case 4: InternalFormat = GL_RGBA; break;
		default: InternalFormat = GL_RGB; break;
		}

		unsigned int Texture;
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		// oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
		stbi_image_free(ImageData);
		return Texture;
	}
	else
	{
		std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
		stbi_image_free(ImageData);
		return 0;
	}
}


void shiftLeftTime(string& time, char newDigit) {

	time[0] = time[1];
	time[1] = time[3];
	time[3] = time[4];
	time[4] = newDigit;


}


bool updateTime(string& time, float& minutes, float& seconds, float timeElapsed) {

	seconds -= timeElapsed;

	if (seconds < 0) {
		seconds = 59.9;
		minutes--;
	}

	if (minutes < 0) {
		return false;
	}
	int minutesI = minutes;
	int secondsI = seconds;


	stringstream updatedTime;
	updatedTime << (minutesI < 10 ? "0" : "") << minutesI << ":"
		<< (secondsI < 10 ? "0" : "") << secondsI;

	time = updatedTime.str();
	return true;
}
void extractMinutesSeconds(string& time, float& minutes, float& seconds) {

	int  minutesI = 0, secondsI = 0;
	char colon;
	stringstream timeStream(time);
	if (time[0] == ' ' && time[1] == ' ') {
		timeStream >> colon >> secondsI;
	}
	else {
		timeStream >> minutesI >> colon >> secondsI;
	}

	minutes = minutesI;
	seconds = secondsI + 1;

}
bool validateTime(string& time) {

	int  minutesI = 0, secondsI = 0;
	char colon;
	stringstream timeStream(time);
	if (time[0] == ' ' && time[1] == ' ') {
		timeStream >> colon >> secondsI;
	}
	else {
		timeStream >> minutesI >> colon >> secondsI;
	}

	if (minutesI == 0 && secondsI == 0) {
		return false;
	}

	if (secondsI >= 60) {
		return false;
	}
	return true;

}

void printMicrowaveTime(string microwaveTime, unsigned textures[]) {
	for (int i = 0; i < microwaveTime.size(); i++) {
		if (i == 2) {
			continue;
		}
		if (microwaveTime[i] == ' ') {
			continue;
		}
		int num = microwaveTime[i] - '0';
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[num]);
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}

void printDugmici(unsigned textures[]) {
	for (int i = 0; i < 10; i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void printErrorMessage(unsigned textures[]) {
	for (int i = 0; i < 5; i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void  creatCircleCoord(float lampica[], float r, float center_x, float center_y) {
	lampica[0] = center_x; //Centar X0
	lampica[1] = center_y; //Centar Y0
	int i;
	for (i = 0; i <= CRES; i++)
	{

		lampica[2 + 2 * i] = lampica[0] + r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
		lampica[2 + 2 * i + 1] = lampica[1] + r * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
	}
}

void  creatSmokeCoord(float circle[], float r, float center_x, float center_y) {
	circle[0] = center_x; //Centar X0
	circle[1] = center_y; //Centar Y0
	int i;
	for (i = 0; i <= CRES; i++)
	{

		circle[2 + 2 * i] = circle[0] + r * cos((3.141592 / 180) * (i * 360 / CRES)); //Xi
		circle[2 + 2 * i + 1] = circle[1] + r * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float normalizedX = (xpos / width) * 2.0f - 1.0f; // Map to [-1, 1]
		float normalizedY = 1.0f - (ypos / height) * 2.0f; // Invert Y and map to [-1, 1]

		printf("Normalized Coordinates: X: %f, Y: %f\n", normalizedX, normalizedY);

		checkTextureClick(normalizedX, normalizedY);
	}
}


void checkTextureClick(float mouseX, float mouseY) {
	float buttonWidth = 0.1f;
	float buttonHeight = 0.1f;

	float buttonPositions[][2] = {
		{0.3f, -0.3f},
		{0.2f,  0.0f},
		{0.3f,  0.0f},
		{0.4f,  0.0f},
		{0.2f, -0.1f},
		{0.3f, -0.1f},
		{0.4f, -0.1f},
		{0.2f, -0.2f},
		{0.3f, -0.2f},
		{0.4f, -0.2f},
	};

	for (int i = 0; i < 10; i++) {
		float xMin = buttonPositions[i][0];
		float xMax = xMin + buttonWidth;
		float yMin = buttonPositions[i][1];
		float yMax = yMin + buttonHeight;

		if (mouseX >= xMin && mouseX <= xMax && mouseY >= yMin && mouseY <= yMax) {
			printf("Button %d clicked!\n", i);
			shiftLeftTime(microwaveTime, '0' + i);
			return;

		}
	}

	// Start, Stop, Reset buttons positions
	float specialButtons[][2] = {
		{0.2f, 0.1f}, // Start
		{0.3f, 0.1f}, // Stop
		{0.4f, 0.1f}, // Reset
	};

	for (int i = 0; i < 3; i++) {
		float xMin = specialButtons[i][0];
		float xMax = xMin + buttonWidth;
		float yMin = specialButtons[i][1];
		float yMax = yMin + buttonHeight;

		if (mouseX >= xMin && mouseX <= xMax && mouseY >= yMin && mouseY <= yMax) {
			switch (i) {
			case 0:
				startMicrowave();
				break;
			case 1:
				stopMicrowave();
				break;
			case 2:
				resetMicrovawe();
				break;
			}
			return; // Exit after handling a special button click
		}
	}
}

void startMicrowave() {
	if ((state == OFF_CLOSED || state == FINISHED) && microwaveTime != "  :  ") {
		if (validateTime(microwaveTime)) {
			state = ON;
			extractMinutesSeconds(microwaveTime, minutes, seconds);
		}
	}
}

void resetMicrovawe() {
	if (state == OFF_CLOSED || state == FINISHED) {
		microwaveTime = "  :  ";

	}
}

void stopMicrowave() {
	if (state == ON) {
		state = OFF_CLOSED;
	}
}
//enum MicrowaveState { ON, OFF_CLOSED, OFF_OPENED, BROKEN, FINISHED };
