
#define PROYECTO "Practica 4"

#include <iostream>	
#include <codebase.h>
#include <vector>
#include <gl\freeglut.h>
#include <cmath>

using namespace cb;
static GLuint ruedaGrande, ruedaPetita;
static int traslacion;

//los radios de los engranajes son los del medio, es decir el valle
static float altura, anchura, radioRuedaGrande, radioRuedaPetita;
static int nDientesRuedaGrande, nDientesRuedaPetita;

//parametros a escribir en la consola por el usuario
static int izquierda, derecha, arriba, abajo, cerca, lejos, fov, razon;

// Inicialización del ojo, punto de interés y vector vertical de la cámara
static Vec3 ojo = { 0,0,-1 };	//Define desde dónde se observa la escena. P.e. (0, 0, -1), significa que la cámara está en el eje Z negativo
static Vec3 poi = { 0,0,0 };	//Define el punto de interes hacia el que mira la camara
static Vec3 vup = { 0,1,0 };	//Define la direccion que se considera arriba (eje Y)

//ejes para el metodo de trasladar puntos
enum eje {
	X, Y, Z
};

vector<Vec3> puntosCircunferencia(float radio, int numPuntos, float desfase = 0)
{
	vector<Vec3> puntos;
	for (float angulo = desfase; angulo < 2 * PI + desfase; angulo += 2 * PI / numPuntos)
		puntos.push_back(Vec3(radio * cos(angulo), radio * sin(angulo), 0));
	return puntos;
}

//traslada un vector de puntos una distancia dada en un plano determinado por el eje
vector<Vec3> trasladar_plano(vector<Vec3> puntos,eje plano, float distancia)
{
	vector<Vec3> res;
	switch (plano)
	{
	case X:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x+distancia, puntos[i].y, puntos[i].z));
		}
		break;
	case Y:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x, puntos[i].y+distancia, puntos[i].z));
		}
		break;
	case Z:
		for (int i = 0; i < puntos.size(); i++)
		{
			res.push_back(Vec3(puntos[i].x, puntos[i].y, puntos[i].z+distancia));
		}
		break;
	default:
		break;
	}

	return res;
}

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
		//evitamos alterar la matriz inicial
		glPushMatrix();

		//calculamos el angulo de rotacion: numDiente * anguloEntreDientes y angulamos la matriz
		glRotatef(i * 360.0 / nPuntos, 0, 0, 1);

		//alineamos la posicion de la matriz con el radio del engranaje
		glTranslatef(0, radio * sin(2 * PI), 0);

		//pintamos el diente
		glCallList(pintarDiente);

		//volvemos a la matriz inicial
		glPopMatrix();
	}
	glPopAttrib();
	glEndList();

	// Creacion engranaje
	GLuint engranaje = glGenLists(1);
	glNewList(engranaje, GL_COMPILE);

	//dibujamos la cara delantera
	glCallList(cara);

	glPushMatrix();	//cambiamos la matriz
	glTranslatef(0, 0, anchura);	//trasladamos a la cara trasera
	glColor3fv(NEGRO);	//color negro para sombrear
	glCallList(cara);	//dibujamos la cara trasera
	glPopMatrix();

	//puntos de la cara trasera para dibujar los cuadrados que las unen
	vector<Vec3> picosB = trasladar_plano(picosF, Z, anchura);
	vector<Vec3> vallesB = trasladar_plano(vallesF, Z, anchura);
	vector<Vec3> ejesB = trasladar_plano(ejesF, Z, anchura);

	for (int i = 0; i < nPuntos; i++)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS); //hacemos push para añadir una nueva capa de atributos y no modificar la anterior
		glColor3f(0, 0, 0.3);	//azul oscuro para sombrear

		//lados del pico
		cb::quad(picosF[(i) % nPuntos], picosB[(i) % nPuntos], vallesB[(i + 1) % nPuntos], vallesF[(i + 1) % nPuntos]);
		cb::quad(picosF[(i + 1) % nPuntos], picosB[(i + 1) % nPuntos], vallesB[(i + 1) % nPuntos], vallesF[(i + 1) % nPuntos]);

		//lado del interior del eje
		cb:quad(ejesF[i % nPuntos], ejesB[i % nPuntos], ejesB[(i + 1) % nPuntos], ejesF[(i + 1) % nPuntos]);
		glPopAttrib();	//restauramos atributos anteriores

	}
	glEndList();

	return engranaje;

}


void init() {

	// Creo lista rueda	
	altura = 0.10f;
	anchura = 0.5f;

	// Inicialización Rueda
	nDientesRuedaGrande = 60;
	radioRuedaGrande = 1.0f;


	// Inicialización Pin
	nDientesRuedaPetita = 12;
	radioRuedaPetita = 0.2f;

	// Creacion de un RUEDA
	ruedaGrande = pintarEngranaje(altura, nDientesRuedaGrande, radioRuedaGrande, anchura);

	ruedaPetita = pintarEngranaje(altura, nDientesRuedaPetita, radioRuedaPetita, anchura);


	// Configurar el motor de render
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


// Funcion de atencion al evento de dibujo
void display()
{
	//limpiamos los buffers y habilitamos DEPTH_TEST para que calcule que superficies estan detras de otras
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	cb::ejes();

	//Cambia al modo MODELVIEW donde se aplican las operaciones que afectan la posición y orientacion de objetos y camara
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	//restauramos valor por defecto

	// configuramos movimiento de la camara
	Vec3 u = vup.cross(ojo - poi).normalize();	//Vector horizontal (hacia la derecha) calculado como el producto cruzado entre el vector vertical vup y la dirección de la cámara (ojo - poi)
	Vec3 v = (ojo - poi).cross(u).normalize();	//Vector vertical resultante del producto cruzado entre la dirección de la cámara y u


	// Rotacion alrededor de v pasando por poi
	// GIRO
	static float angulo = rad(1);
	ojo = ojo - poi; // Traslada al origen para calcular rotación
	ojo.rotate(angulo, v); // Gira en torno al eje vertical v
	ojo = ojo + poi; // Devuelve al punto de interés

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, vup.x, vup.y, vup.z);

	//redibujar engranaje grande
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(AZUL);
	glCallList(ruedaGrande);
	glPopAttrib();

	//calculamos la posicion de la rueda pequeña
	//radio primitivo rueda y radio primitivo pin suman la distancia minima de interseccion de los engranajes sin que se solape nada
	const float radioPicoRueda = radioRuedaGrande + altura;
	const float radioPrimitivoRueda = (radioRuedaGrande + radioPicoRueda) / 2;

	const float radioPicoPin = radioRuedaPetita + altura;
	const float radioPrimitivoPin = (radioRuedaPetita + radioPicoPin) / 2;

	//redibujar engranaje pequeño
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(NEGRO);
	glRotatef((360 / nDientesRuedaPetita) / 2, 0, 0, 1); //rotamos el engranaje la mitad del angulo que hay entre dos dientes
	// Aqui debe utilizarse la suma de los radios primitivos para que no se solapen
	glTranslatef(-(radioPrimitivoRueda + radioPrimitivoPin), 0, 0);
	glCallList(ruedaPetita);
	glPopAttrib();
	glPopMatrix();

	glFlush();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	//aseguramos que la escena ocupe toda la ventana poniendo el origen del área de dibujo está en la esquina inferior izquierda de la ventana
	//y dando la altura y anchura que recibumos de glut cuando llama al callback
	glViewport(0, 0, w, h);

	//relacion de aspecto: sirve para mantener las proporciones de los objetos en pantalla
	float ra = (float)w / h;

	//reestablece la matriz de proyeccion al default para eliminar transformaciones previas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*CAMARA EN PERSPECTIVA*/
	//gluPerspective(fov, ra , cerca, lejos);	// redimensiona usando una camara en perspectiva

	/*CAMARA ORTOGRAFICA*/

	if (w > h) {
		// si la ventana tiene mas anchura que altura ampliar el lado horizontal en proporcion a la relacion de aspecto
		glOrtho(izquierda * ra, derecha * ra, abajo, arriba, cerca, lejos);
	}
	else {
		//Si la altura es mayor que el ancho se ajustan los límites verticales dividiendo entre la relación de aspecto
		glOrtho(izquierda, derecha, abajo / ra, arriba / ra, cerca, lejos);	
	}

}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);	//inicializar glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);	//un solo buffer de color, colores RGB y uso de depth buffer para 3D
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);
	init();

	std::cout << "Escribe los siguientes parámetros en el orden correspondiente: izquierda, derecha, arriba, abajo, cerca, lejos, fov, razon " << std::endl;
	
	//coordenadas de los limites del volumen ortografico
	//std::cin >> izquierda;
	//std::cin >> derecha;
	//std::cin >> arriba;
	//std::cin >> abajo;
	
	//coordenadas de los planos que dan la profundidad de la camara
	//std::cin >> cerca;
	//std::cin >> lejos;
	
	//angulo de apertura de la camara
	//std::cin >> fov;
	
	//relacion de aspecto de la ventana
	//std::cin >> razon;
	izquierda = -2;
	derecha = 2;
	arriba = 2;
	abajo = -2;
	cerca = -10;
	lejos = 10;
	fov = 45;
	razon = 1;


	// Callbacks
	glutDisplayFunc(display);	//cada vez que se tenga que volver a renderizar la ventana (p.e. cambio de tamaño) lo maneja esta funcion
	glutReshapeFunc(reshape);	//funcion que se encarga de ajustar las proporciones cuando se redimentiona la ventana
	std::cout << PROYECTO << " running" << std::endl;
	// Bucle de atencion a eventos
	glutMainLoop();
}
