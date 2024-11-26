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
	ejes();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(engranaje);

	glFlush();
}

void init()
{
    // Definición de los parámetros del engranaje
    static const float radio = 0.7;
    static const int nPuntos = 35;
    static const float desfase = PI / 2;
    vector<Vec3> pico = puntosCircunferencia(radio, nPuntos, desfase);
    vector<Vec3> valle = puntosCircunferencia((radio * 2.5) / 4, nPuntos, desfase - (PI / nPuntos));
    vector<Vec3> eje = puntosCircunferencia(radio / 4, nPuntos, desfase);

    // Generación de la lista de visualización para el engranaje
    engranaje = glGenLists(1);
    glNewList(engranaje, GL_COMPILE);
    glRotatef(-45, 0, 1, 0);

    for (int i = 0; i < nPuntos; i++) {

		// Dibujar cara superior
		glColor3f(1, 0, 0);
		glBegin(GL_TRIANGLE_STRIP);
		Vec3 v1f = valle[i % nPuntos];
		Vec3 p1f = pico[i % nPuntos];
		Vec3 e1f = eje[i % nPuntos];
		Vec3 v2f = valle[(i + 1) % nPuntos];
		Vec3 e2f = eje[(i + 1) % nPuntos];
		glVertex3fv(v1f);
		glVertex3fv(p1f);
		glVertex3fv(e1f);
		glVertex3fv(v2f);
		glVertex3fv(e2f);
		glEnd();

		// Dibujar cara inferior desplazada y almacenar sus puntos desplazados
		glColor3f(0, 1, 0);
		Vec3 v1b, p1b, e1b, v2b, e2b; // Puntos desplazados en Z
		v1b = Vec3(v1f.x, v1f.y, v1f.z + 0.5f);
		p1b = Vec3(p1f.x, p1f.y, p1f.z + 0.5f);
		e1b = Vec3(e1f.x, e1f.y, e1f.z + 0.5f);
		v2b = Vec3(v2f.x, v2f.y, v2f.z + 0.5f);
		e2b = Vec3(e2f.x, e2f.y, e2f.z + 0.5f);

		glPushMatrix();
		glTranslatef(0, 0, 0.5);

		glBegin(GL_TRIANGLE_STRIP);
		glVertex3fv(v1f);
		glVertex3fv(p1f);
		glVertex3fv(e1f);
		glVertex3fv(v2f);
		glVertex3fv(e2f);
		glEnd();
		glPopMatrix();

		// Dibujar el perfil lateral usando los puntos calculados
		glColor3f(0, 0, 1);
		glBegin(GL_QUAD_STRIP);

		// Cada par (superior, inferior) forma un lado del perfil
		glVertex3fv(v1f);
		glVertex3fv(v1b);

		glVertex3fv(p1f);
		glVertex3fv(p1b);

		glVertex3fv(e1f);
		glVertex3fv(e1b);

		glVertex3fv(v2f);
		glVertex3fv(v2b);

		glVertex3fv(e2f);
		glVertex3fv(e2b);

		glEnd();

    }

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