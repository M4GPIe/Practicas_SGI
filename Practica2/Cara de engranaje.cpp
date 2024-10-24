#include <iostream>
#include <gl\freeglut.h>
#include <iostream>	
#include <codebase.h>
#include <vector>
#include <math.h>

using namespace cb;

GLuint engranaje;

vector<Vec3> puntosCircunferencia(float radio, int numPuntos, float desfase = 0)
{
	vector<Vec3> puntos;
	for (float angulo = desfase; angulo < 2 * PI + desfase; angulo += 2 * PI / numPuntos)
		puntos.push_back(Vec3(radio * cos(angulo), radio * sin(angulo), 0));
	return puntos;
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3fv(BLANCO);
	glCallList(engranaje);


	glFlush();
}

void init()
{
	// Objeto rueda
	static const float radio = 0.7;
	static const int nPuntos = 30;
	static const float desfase = PI / 2;
	vector<Vec3> pico = puntosCircunferencia(radio, nPuntos, desfase);
	vector<Vec3> valle = puntosCircunferencia(radio / 2, nPuntos, desfase - (PI / nPuntos));
	vector<Vec3> eje = puntosCircunferencia(radio / 4, nPuntos, desfase);

	engranaje = glGenLists(1);
	glNewList(engranaje, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < nPuntos; i++) {
		glVertex3fv(valle[i % nPuntos]);
		glVertex3fv(pico[i % nPuntos]);
		glVertex3fv(eje[i % nPuntos]);
		glVertex3fv(valle[(i + 1) % nPuntos]);
		glVertex3fv(eje[(i + 1) % nPuntos]);
	}

	glEnd();
	glEndList();
}

void reshape(GLint w, GLint h)
{
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Practica2");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}