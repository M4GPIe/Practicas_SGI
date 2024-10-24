#include <iostream>
#include <gl\freeglut.h>

void pentagonPoly()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	
	//glColor3f(1, 0, 0);
	glVertex2f(0.0, 0.75);
	//glColor3f(0, 1, 0);
	glVertex2f(-1.0, 0);
	//glColor3f(0, 0, 1);
	glVertex2f(-0.5, -1);
	//glColor3f(1, 1, 0);
	glVertex2f(0.5, -1);
	//glColor3f(0, 1, 1);
	glVertex2f(1, 0);

	glEnd();
	glFlush();
}

void pentagonTriStrip()
{
	//every three vertex is a triangle
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0,0);
	glVertex2f(0.0, 0.75);
	glVertex2f(0, 0);
	glVertex2f(-1.0, 0);
	glVertex2f(0, 0);
	glVertex2f(-0.5, -1);
	glVertex2f(0, 0);
	glVertex2f(0.5, -1);
	glVertex2f(0.0, 0);
	glVertex2f(1, 0);
	glVertex2f(0.0, 0.75);
	glEnd();
	glFlush();
}

void pentagonTriFan()
{
	glClear(GL_TRIANGLE_FAN);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.75);
	glVertex2f(-1.0, 0);
	glVertex2f(-0.5, -1);
	glVertex2f(0.5, -1);
	glVertex2f(1, 0);
	glEnd();
	glFlush();

}

void exer3()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1, 0, 0);

	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.75);
	glVertex2f(-1.0, 0);
	glVertex2f(-0.5, -1);
	glVertex2f(0.5, -1);
	glVertex2f(1, 0);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0, 1);

	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.75);
	glVertex2f(-1.0, 0);
	glVertex2f(-0.5, -1);
	glVertex2f(0.5, -1);
	glVertex2f(1, 0);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glColor3f(0, 1, 0);
	glPointSize(10);

	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.75);
	glVertex2f(-1.0, 0);
	glVertex2f(-0.5, -1);
	glVertex2f(0.5, -1);
	glVertex2f(1, 0);
	glEnd();

	glFlush();
}

/*
void display()
{
	exer3();
}

void reshape(GLint w, GLint h)
{
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Practica2");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}*/