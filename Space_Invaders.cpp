//******************************************************************************
//
// 	Aula de Computação Gráfica ICMC - USP (18/05/2017)
// 
// 	Bruno Pinto Ferraz Fabbri 			4154844
// 	Frederico de Oliveira Sampaio		8922100
// 	Nícolas Bassetto Leite 					8937292
//	Rogiel dos Santos Silva 				8061793
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


// Declaração de variáveis globais
GLfloat misselNave_y = -1.0f, misselNave_x = 0.1f;
GLfloat aviao_x = 0, missel1_tx = 0, missel2_tx = 0;

// tamanho das entidades da nave
GLfloat triangle_scale = 10.0f, quad_scale = 10.0f; // Alterar a escala para redimensionar a nave
GLfloat triangle = 1.0f/triangle_scale, quad1 = 0.2f/quad_scale, quad2 = 0.5f/quad_scale;

// tamanho do missel
GLfloat missel_scale = 10.0f; // Alterar a escala para redimensionar o missel
GLfloat missel1 = 1.0f/missel_scale, missel2 = 0.7f/missel_scale, missel3 = 0.8f/missel_scale, missel4 = 0.6f/missel_scale, missel5 = 0.9f/missel_scale;


bool missel1_moving = false, missel2_moving = false;

int msec_missel1 = 0, msec_missel2 = 0;


void move_misselNave(int passo){

	misselNave_y += (1.0*passo)/100;
	glutPostRedisplay();
	glutTimerFunc(10, move_misselNave, passo);
}

// Função para desenhar o jatinho           
void DesenhaAviao() {

	glColor3f(1.0f, 0.0f, 1.0f);
	glLineWidth(2);
	glBegin(GL_TRIANGLES);
		glVertex2f(-triangle, -triangle);
		glVertex2f(triangle, -triangle);
		glVertex2f(0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
		glVertex2f(quad1, -quad1);
		glVertex2f(quad1, quad2);
		glVertex2f(-quad1, quad2);
		glVertex2f(-quad1, -quad1);
	glEnd();
}

// Função para desenhar os mísseis
void DesenhaMisseis() {

	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2);
	glBegin(GL_POLYGON);
		glVertex2f(-missel1, -missel1);
		glVertex2f(-missel1, -missel2);
		glVertex2f(-missel5, -missel4);
		glVertex2f(-missel3, -missel2);
		glVertex2f(-missel3, -missel1);
	glEnd();
}
           
// Função callback de redesenho da janela de visualização
void Desenha(void)
{	
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
     
	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);
	
	

	/*Posicao do aviao*/
	glTranslatef(aviao_x,0.0f,0.0f);
	glTranslatef(0.0f,-0.7f,0.0f);
	// glScalef(0.3f,0.3f,0.0f);
	glPushMatrix();

	//Missel 1
	glTranslatef(misselNave_x, misselNave_y+1.1f, 0.0f); // a soma na misselNave_y e a posicao inicial do tiro
	DesenhaMisseis();

	glPopMatrix();	

	//Aviao	
	DesenhaAviao();
	
	
	// Executa os comandos OpenGL 
	glFlush();
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;
                   
	// Evita a divisao por zero
	if(h == 0) h = 1;

	// Atualiza as variáveis
	largura = w;
	altura = h;
                                              
	// Especifica as dimensões da Viewport
	glViewport(0, 0, largura, altura);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Estabelece a janela de seleção (esquerda, direita, inferior, 
	// superior) mantendo a proporção com a janela de visualização
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

// Função callback chamada para gerenciar eventos de teclas especiais(F1,PgDn,...)
void TeclasEspeciais(int key, int x, int y)
{
	// Move a base
	if (key == GLUT_KEY_LEFT){

		// Move aviao
		aviao_x -= 0.1;
		
		// Move missel se ele ja foi disparado
		if(missel1_moving == true)
			misselNave_x += 0.1;
	}

	if (key == GLUT_KEY_RIGHT){
	
		// Move aviao
		aviao_x += 0.1;

		// Move missel se ele ja foi disparado
		if(missel1_moving == true)
			misselNave_x -= 0.1;
	}
	if (key == GLUT_KEY_UP) {
		missel1_moving = true;
		glutTimerFunc(10, move_misselNave, 1);
	}
                                                
	glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos de teclas
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
           
// Função responsável por inicializar parâmetros e variáveis
void Inicializa(void)
{   
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluOrtho2D (-1.0f, 1.0f, -1.0f, 1.0f);
  gluOrtho2D(0, 1300, 0, 800);      
}



// Programa Principal 
int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Inicia uma instância da glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(1300, 800); 
	glutCreateWindow("Desenho de um prototipo de jatinho do Space Invaders!"); 
 
	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);  
	
	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);    

	// Registra a função callback para tratamento das teclas especiais
	glutSpecialFunc(TeclasEspeciais);

	// Registra a função callback para tratamento das teclas ASCII
	glutKeyboardFunc(Teclado);

	// Chama a função responsável por fazer as inicializações 
	Inicializa(); 
 
	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
