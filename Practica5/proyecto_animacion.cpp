

#define PROYECTO "Animacion"
#define _USE_MATH_DEFINES

//identificadores de acceso global de las listas de dibujo
#define NORIA 1
#define ENGRANAJE_GRANDE 2
#define ENGRANAJE_PEQUEÑO 3
#define CABINA 4

//dependencias
#include <GL/glut.h>
#include <GL/glu.h> 
#include <GL/gl.h>
#include <gl\freeglut.h>
#include <codebase.h>

#include <cmath>
#include <math.h>

#include<stdio.h>
#include <iostream>
#include <sstream>

#include <vector>

using namespace std;
using namespace cb;

GLfloat rotacion;	//angulo actual de la rueda, determina la posicion de la rotacion
GLfloat velocidad_rotacion = 0;	//velocidad de rotacion

static const int n_cabinas = 14;

//angulo de la camara en los ejes x e y para modelizar el desplazamiento con las flechas
float angx = 0;
float angy = 0;

vector<Vec3> puntosCircunferencia(float radio, int numPuntos, float desfase = 0)
{
	vector<Vec3> puntos;
	for (float angulo = desfase; angulo < 2 * PI + desfase; angulo += 2 * PI / numPuntos)
		puntos.push_back(Vec3(radio * cos(angulo), radio * sin(angulo), 0));
	return puntos;
}

//ejes para el metodo de trasladar puntos
enum eje {
	X, Y, Z
};

//traslada un vector de puntos una distancia dada en un plano determinado por el eje
vector<Vec3> trasladar_plano(vector<Vec3> puntos, eje plano, float distancia)
{
	vector<Vec3> res;
	switch (plano)
	{
	case X:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x + distancia, puntos[i].y, puntos[i].z));
		}
		break;
	case Y:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x, puntos[i].y + distancia, puntos[i].z));
		}
		break;
	case Z:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x, puntos[i].y, puntos[i].z + distancia));
		}
		break;
	default:
		break;
	}

	return res;
}

//engranaje de las practicas anteriores
GLuint pintarEngranaje(float altura, int nPuntos, float radio, float anchura) {

	const float desfase = PI / 2;

	//cara delantera
	vector<Vec3> picosF = puntosCircunferencia(radio + altura, nPuntos, desfase);
	vector<Vec3> vallesF = puntosCircunferencia(radio, nPuntos, desfase - (PI / nPuntos));
	vector<Vec3> ejesF = puntosCircunferencia(radio - altura, nPuntos, desfase);

	// lista para pintar de un DIENTE -> aA1b2
	//da igual que puntos cogemos de la lista para dibujarlo porque los desplazamientos se hacen con las matrices
	GLuint pintarDiente = glGenLists(1);
	glNewList(pintarDiente, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);

	glVertex3fv(vallesF[1]);

	glVertex3fv(picosF[1]);
	glVertex3fv(ejesF[1]);

	glVertex3fv(vallesF[2]);
	glVertex3fv(ejesF[2]);

	glEnd();
	glEndList();

	//lista para pintar una cara
	GLuint cara = glGenLists(1);
	glNewList(cara, GL_COMPILE);
	for (int i = 0; i < nPuntos; i++)
	{
		glPushMatrix();

		//calculamos el angulo de rotacion: numDiente * anguloEntreDientes y angulamos la matriz
		glRotatef(i * 360.0 / nPuntos, 0, 0, 1);

		//alineamos la posicion de la matriz con el radio del engranaje
		glTranslatef(0, radio * sin(2 * PI), 0);

		//pintamos el diente
		glCallList(pintarDiente);

		glPopMatrix();
	}
	glPopAttrib();
	glEndList();

	// Creacion engranaje
	GLuint engranaje = glGenLists(1);
	glNewList(engranaje, GL_COMPILE);

	//dibujamos la cara delantera
	glCallList(cara);

	glPushMatrix();
	glTranslatef(0, 0, anchura);	//trasladamos a la cara trasera
	glColor3fv(NEGRO);
	glCallList(cara);
	glPopMatrix();

	//puntos de la cara trasera para dibujar los cuadrados que las unen
	vector<Vec3> picosB = trasladar_plano(picosF, Z, anchura);
	vector<Vec3> vallesB = trasladar_plano(vallesF, Z, anchura);
	vector<Vec3> ejesB = trasladar_plano(ejesF, Z, anchura);

	for (int i = 0; i < nPuntos; i++)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS); //hacemos push para añadir una nueva capa de atributos y no modificar la anterior
		glColor3f(0, 0, 0.3);

		//lados del pico
		cb::quad(picosF[(i) % nPuntos], picosB[(i) % nPuntos], vallesB[(i + 1) % nPuntos], vallesF[(i + 1) % nPuntos]);
		cb::quad(picosF[(i + 1) % nPuntos], picosB[(i + 1) % nPuntos], vallesB[(i + 1) % nPuntos], vallesF[(i + 1) % nPuntos]);

		//lado del interior del eje
	cb:quad(ejesF[i % nPuntos], ejesB[i % nPuntos], ejesB[(i + 1) % nPuntos], ejesF[(i + 1) % nPuntos]);
		glPopAttrib();

	}
	glEndList();

	return engranaje;

}

//se que parte de esto no lo hemos usado en clase pero me hacia ilusion que se vieran los controles en la ventana :)
void renderTexto(const char* texto, float x, float y) {
	// cambiar a proyección ortográfica para dibujar texto en 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);  // coordenadas en porcentaje de la ventana

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();  // guardar configuración actual
	glLoadIdentity();

	// posicionar el texto en la pantalla
	glRasterPos2f(x, y);
	for (const char* c = texto; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}

	// restaurar configuraciones originales
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);  // volver al modo de modelado
}

// funcion de muestra de FPS
void muestraFPS()
{
	static int tiempoAnterior = glutGet(GLUT_ELAPSED_TIME);;
	static int frames = 0;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempoTranscurrido = (ahora - antes);

	int tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	frames++;
	if (tiempoTranscurrido > 1000)
	{
		stringstream titulo;
		titulo << PROYECTO << " (FPS: " << frames << ")";
		glutSetWindowTitle(titulo.str().c_str());
		frames = 0;
		antes = ahora;
	}
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);	//habilitar test de profundidad

	//lista que dibuja las circunferencias y el eje del chasis de la noria
	glNewList(NORIA, GL_COMPILE);
	glColor3f(0, 0, 0.6);

	/* chasis de la noria */

	// generar los puntos de las circunferencias interna y externa
	vector<Vec3> circunferenciaExterior = puntosCircunferencia(30, 360);
	vector<Vec3> circunferenciaInterior = puntosCircunferencia(28, 360);

	// rueda frontal
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, 4);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, 4);
	}
	glEnd();

	// laterales rueda frontal
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, 4);
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, 3);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, 4);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, 3);
	}
	glEnd();

	// interior rueda frontal
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, 3);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, 3);
	}
	glEnd();

	// rueda trasera
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, -4);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, -4);
	}
	glEnd();

	// laterales rueda trasera
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, -4);
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, -3);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, -4);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, -3);
	}
	glEnd();

	// interior cara trasera
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++) {
		int index = i % 360;
		glVertex3f(circunferenciaExterior[index].x, circunferenciaExterior[index].y, -3);
		glVertex3f(circunferenciaInterior[index].x, circunferenciaInterior[index].y, -3);
	}
	glEnd();


	/* Dibujar el eje de la noria */
	vector<Vec3> verticesEje = puntosCircunferencia(5, 360, 0);
	glColor3f(0.337, 0.196, 0.196);
	// frente del eje
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 10);
	for (int i = 0; i < 360; i++)
	{
		glVertex3f(verticesEje[i].x, verticesEje[i].y, 10);
	}
	glEnd();

	// lateral del eje 
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 360; i++) {
		glVertex3f(verticesEje[i].x, verticesEje[i].y, 10);
		glVertex3f(verticesEje[i].x, verticesEje[i].y, -10);
	}
	glEnd();

	// parte trasera del eje
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, -10);
	for (int i = 0; i < 360; i++)
	{
		glVertex3f(verticesEje[i].x, verticesEje[i].y, -10);
	}
	glEnd();

	glEndList();

	// lista que dibuja una cabina de la noria con su soporte
	glNewList(CABINA, GL_COMPILE);
	glColor3f(0.5, 0, 0);

	vector<Vec3> puntosBarra = puntosCircunferencia(0.5, 360);

	glColor3f(0.529, 0.337, 0.231);

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 360; i++) {
		glVertex3f(puntosBarra[i].x, puntosBarra[i].y, 4);
		glVertex3f(puntosBarra[i].x, puntosBarra[i].y, -4);
	}
	glEnd();

	// cabina
	glPushMatrix();
	glColor3f(0.4667, 0.4627, 0.7333);
	glTranslatef(0, -2, 0);
	glScalef(3, 2, 2);
	glutSolidCube(2);

	glPopMatrix();

	glEndList();
}

//dibujar la escena completa
void dibujarNoria() {

	//base de la noria: circunferencias y eje
	glPushMatrix();
	glCallList(NORIA);
	glPopMatrix();

	//radios de la noria y cabinas ajustados a la rotacion
	glPushMatrix();
	glRotatef(rotacion, 0, 0, 1);

	vector<Vec3> circunferenciaExterior = puntosCircunferencia(28, 360, rotacion);

	for (int i = 0; i < n_cabinas; i++) {
		float angulo_cabina = (360.0 / n_cabinas) * i;	//angulo en el que se encuentra la cabina respecto a la circunferencia de la noria
		glPushMatrix();

		glColor3f(0.529, 0.337, 0.231);

		//radio cara
		glPushMatrix();
		glRotatef(angulo_cabina, 0, 0, 1);
		glTranslatef(0, 14.5, 4);
		glScalef(1.5, 28, 1);
		glutSolidCube(1.1);
		glPopMatrix();

		//radio trasero
		glPushMatrix();
		glRotatef(angulo_cabina, 0, 0, 1);
		glTranslatef(0, 14.5, -4);
		glScalef(1.5, 28, 1);
		glutSolidCube(1.1);
		glPopMatrix();

		glTranslatef(28 * cos((angulo_cabina * M_PI) / 180), 28 * sin((angulo_cabina * M_PI) / 180), 0);
		glRotatef(-rotacion, 0, 0, 1);
		glCallList(CABINA);
		glPopMatrix();
	}
	glPopMatrix();

	//patas de la noria
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glRotatef(-30, 0, 0, 1);
	glTranslatef(0, -25, 7.5);
	glScalef(3, 40, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glRotatef(30, 0, 0, 1);
	glTranslatef(0, -25, 7.5);
	glScalef(3, 40, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glRotatef(-30, 0, 0, 1);
	glTranslatef(0, -25, -7.5);
	glScalef(3, 40, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glRotatef(30, 0, 0, 1);
	glTranslatef(0, -25, -7.5);
	glScalef(3, 40, 2);
	glutSolidCube(1);
	glPopMatrix();

	//engranaje
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.7176, 0.6706, 0.8863);
	glTranslatef(0, 0, 11);
	pintarEngranaje(3, 10, 2, 5);
	glPopAttrib();
	glPopMatrix();
}

void dibujarControles() {
	glColor3f(0.0f, 0.0f, 0.0f);
	renderTexto("Controles:", 5, 95);
	renderTexto("Planta / Alzado / Perfil: 1/2/3", 5, 90);
	renderTexto("Mover arriba: Flecha ARRIBA", 5, 85);
	renderTexto("Mover abajo: Flecha ABAJO", 5, 80);
	renderTexto("Mover izquierda: Flecha IZQUIERDA", 5, 75);
	renderTexto("Mover derecha: Flecha DERECHA", 5, 70);
	renderTexto("Encender/Apagar noria: E", 5, 65);
	renderTexto("Aumentar velocidad: +", 5, 60);
	renderTexto("Reducir velocidad: -", 5, 55);
}


void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.75f, 1.0f, 1.0f);	//color de fondo del escenario
	glPushMatrix();
	glTranslatef(0, 0, -100);

	//rotamos la escena segun la angulacion de la camara
	glRotatef(angx, 1, 0, 0);
	glRotatef(angy, 0, 1, 0);

	dibujarNoria();
	glPopMatrix();

	//dibujar controles en pantalla
	dibujarControles();

	//mostrar FPS
	muestraFPS();

	glutSwapBuffers();
}

//cambios de dibujo que seran visibles en el siguiente frame
void idle() {
	// tiempo transcurrido
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempoTranscurrido = (ahora - antes) / 1000.0f;

	// rotacion de la rueda dependiene del tiempo
	rotacion += tiempoTranscurrido * velocidad_rotacion;

	// actualizamos tiempo
	antes = ahora;

	glutPostRedisplay();
}

void reshape(int w, int h) {

	glViewport(0, 0, w, h);

	float relacion_aspecto = (GLfloat)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 800.0);

	//volvemos al modo de dibujo para evitar modificar sin querar la matriz de proyeccion y evitar efectos indeseados
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//flechas de movimiento
void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		angx += 2.0;
		break;
	case GLUT_KEY_DOWN:
		angx -= 2.0;
		break;
	case GLUT_KEY_LEFT:
		angy += 2.0;
		break;
	case GLUT_KEY_RIGHT:
		angy -= 2.0;
		break;
	}
	glutPostRedisplay();
}

//controles
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+':
		if (velocidad_rotacion < 200)
			velocidad_rotacion += 5;
		break;
	case '-':
		if (velocidad_rotacion > 0)
			velocidad_rotacion -= 5;
		break;
	case 'e':
	case 'E':
		if (velocidad_rotacion > 0) {
			velocidad_rotacion = 0;
		}
		else {
			velocidad_rotacion = 25;
		};
		break;
	case '1':	//planta
		angx = 90;
		angy = 0;
		break;
	case '2':	//alzado
		angx = 0;
		angy = 0;
		break;
	case '3':	//perfil
		angy = 90;
		angx = 0;
		break;
	};
	glutPostRedisplay();
}

//comenzar animacion
void iniciarRotacion(int value) {
	if (velocidad_rotacion == 0) velocidad_rotacion = 25;
}

int main(int argc, char** argv) {

	//inicializar glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //doble buffer para evitar parpadeos

	glutInitWindowSize(1000, 700);
	glutInitWindowPosition(500, 100);
	glutCreateWindow(PROYECTO);

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutIdleFunc(idle);			//cambios a hacer para el siguiente frame a ejecutar cuando ya no queden eventos en la cola

	glutTimerFunc(5000, iniciarRotacion, 5000);	//callback para iniciar la rotacion automaticamente a los 5s

	glutKeyboardFunc(keyboard);		//callback de eventos de teclado (letras)
	glutSpecialFunc(specialKeys);	//callback de eventos de teclado (flechas)
	glutMainLoop();
	return 0;
}
