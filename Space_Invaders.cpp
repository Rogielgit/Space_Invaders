//******************************************************************************
//
// Aula de Computa��o Gr�fica ICMC - USP (18/05/2017)
//

// 
//******************************************************************************


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>


// Declara��o de vari�veis globais
GLfloat misselNave_y = 0, misselNave_x = 0;
GLfloat aviao_x = 0, missel1_tx = 0, missel2_tx = 0;

bool missel1_moving = false, missel2_moving = false;

int msec_missel1 = 0, msec_missel2 = 0;


void move_misselNave(int passo){

	misselNave_y += (1.0*passo)/100;
	glutPostRedisplay();
	glutTimerFunc(10, move_misselNave, passo);
}

// Fun��o para desenhar o jatinho           
void DesenhaAviao() {

	glColor3f(1.0f, 0.0f, 1.0f);
	glLineWidth(2);
	glBegin(GL_TRIANGLES);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
		glVertex2f(0.2f, -0.2f);
		glVertex2f(0.2f, 0.5f);
		glVertex2f(-0.2f, 0.5f);
		glVertex2f(-0.2f, -0.2f);
	glEnd();
}

// Fun��o para desenhar os m�sseis
void DesenhaMisseis() {

	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2);
	glBegin(GL_POLYGON);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(-1.0f, -0.7f);
		glVertex2f(-0.9f, -0.6f);
		glVertex2f(-0.8f, -0.7f);
		glVertex2f(-0.8f, -1.0f);
	glEnd();
}
           
// Fun��o callback de redesenho da janela de visualiza��o
void Desenha(void)
{	
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transforma��o corrente
	glLoadIdentity();
     
	// Limpa a janela de visualiza��o com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);
	
	

	/*Posicao do aviao*/
	glTranslatef(aviao_x,0.0f,0.0f);
	glTranslatef(0.0f,-0.7f,0.0f);
	glScalef(0.3f,0.3f,0.0f);
	glPushMatrix();

	//Missel 1
	glTranslatef(1.0f,misselNave_y+1.1f, 0.0f); // a soma na misselNave_y e a posicao inicial do tiro
	DesenhaMisseis();

	glPopMatrix();	

	//Aviao	
	DesenhaAviao();
	
	
	// Executa os comandos OpenGL 
	glFlush();
}

// Fun��o callback chamada quando o tamanho da janela � alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;
                   
	// Evita a divisao por zero
	if(h == 0) h = 1;

	// Atualiza as vari�veis
	largura = w;
	altura = h;
                                              
	// Especifica as dimens�es da Viewport
	glViewport(0, 0, largura, altura);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Estabelece a janela de sele��o (esquerda, direita, inferior, 
	// superior) mantendo a propor��o com a janela de visualiza��o
	if (largura <= altura) 
	{ 
		gluOrtho2D (-1.0f, 1.0f, -1.0f*altura/largura, 1.0f*altura/largura);
		//win = 1.0f;
	}              
	else 
	{ 
		gluOrtho2D (-1.0f*largura/altura, 1.0f*largura/altura, -1.0f, 1.0f);
	//	win = 1.0f*largura/altura;           
	}             
}

// Fun��o callback chamada para gerenciar eventos de teclas especiais(F1,PgDn,...)
void TeclasEspeciais(int key, int x, int y)
{
	// Move a base
	if (key == GLUT_KEY_LEFT){
		aviao_x -= 0.1;
	}
	if (key == GLUT_KEY_RIGHT){
		aviao_x += 0.1;
	}
	if (key == GLUT_KEY_UP) {
		glutTimerFunc(10, move_misselNave, 1);
	}
                                                
	glutPostRedisplay();
}

// Fun��o callback chamada para gerenciar eventos de teclas
void Teclado(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'a')
		aviao_x -= 0.1;
	if (key == 'd')
		aviao_x += 0.1;
	if (key == 'w')
		glutTimerFunc(10, move_misselNave, 1);
	
	glutPostRedisplay();
}
           
// Fun��o respons�vel por inicializar par�metros e vari�veis
void Inicializa(void)
{   
	// Define a cor de fundo da janela de visualiza��o como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluOrtho2D (-1.0f, 1.0f, -1.0f, 1.0f);
  gluOrtho2D(0, 1300, 0, 800);      
}



// Programa Principal 
int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Inicia uma inst�ncia da glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(1300, 800); 
	glutCreateWindow("Desenho de um prototipo de jatinho do Space Invaders!"); 
 
	// Registra a fun��o callback de redesenho da janela de visualiza��o
	glutDisplayFunc(Desenha);  
	
	// Registra a fun��o callback de redimensionamento da janela de visualiza��o
	glutReshapeFunc(AlteraTamanhoJanela);    

	// Registra a fun��o callback para tratamento das teclas especiais
	glutSpecialFunc(TeclasEspeciais);

	// Registra a fun��o callback para tratamento das teclas ASCII
	glutKeyboardFunc(Teclado);

	// Chama a fun��o respons�vel por fazer as inicializa��es 
	Inicializa(); 
 
	// Inicia o processamento e aguarda intera��es do usu�rio
	glutMainLoop();

	return 0;
}
