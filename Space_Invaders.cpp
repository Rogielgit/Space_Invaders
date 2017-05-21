//******************************************************************************
//
// 	Aula de Computação Gráfica ICMC - USP (18/05/2017)
// 
// 	Bruno Pinto Ferraz Fabbri 			4154844
// 	Frederico de Oliveira Sampaio		8922100
// 	Nícolas Bassetto Leite 				8937292
//	Rogiel dos Santos Silva 			8061793
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
#include <unistd.h>
#include <math.h>


// Declaração de variáveis globais

GLfloat aviao_x = 0, missel1_tx = 0, missel2_tx = 0;

// tamanho das entidades da nave
GLfloat triangle_scale = 10.0f, quad_scale = 10.0f; // Alterar a escala para redimensionar a nave
GLfloat triangle = 1.0f/triangle_scale, quad1 = 0.2f/quad_scale, quad2 = 0.5f/quad_scale;

// tamanho do missel
GLfloat missel_scale = 10.0f; // Alterar a escala para redimensionar o missel
GLfloat missel1 = 1.0f/missel_scale, missel2 = 0.7f/missel_scale, missel3 = 0.8f/missel_scale, missel4 = 0.6f/missel_scale, missel5 = 0.9f/missel_scale;

// Alien
GLfloat alien = 0.05f, posiAliensx = 0.0f, posiAliensy = 0.0f, posiAliens_x[25], posiAliens_y[25];

// Variaveis para caminhar com a matriz de aliens
GLint counter = 0, right_or_left = 1, line_down = 1, quantTiros = 0 , flag = 0, velocidadeAlien = 4500, numeTiro = 0;

bool missel1_moving = false, missel2_moving = false;

// Vetor para controlar se o alien esta morto (marcar com 0 quando ele morrer)
bool alien_is_live [25] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool tiroNave [10] = {1,1,1,1,1,1,1,1,1,1};
GLfloat misselNave_y[10];
GLfloat misselNave_x[10];

int msec_missel1 = 0, msec_missel2 = 0;


// Função para desenhar os aliens           
void DesenhaAlien() {

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2);
	
	glTranslatef(0.1f*counter,-0.1f*line_down,0.0f);
	

	// Vai para o ponto inicial em cima na esquerda
	glTranslatef(-1.0f,1.0f,0.0f);

	// Desce a linha a cada 5 aliens
	for(int i = 0; i < 25; i++){
		if(alien_is_live[i] == 1){
			glBegin(GL_QUADS);
				glVertex2f(alien, -alien);
				glVertex2f(alien, alien);
				glVertex2f(-alien, alien);
				glVertex2f(-alien, -alien);
			glEnd();
			posiAliens_x[i] = posiAliensx - 1.0f + (0.1f*counter);
			posiAliens_y[i] = posiAliensy + 1.8 - (0.1f*line_down); // posicao dos aliens em y  
			posiAliensx += 0.3f;
			
		}
		glTranslatef(0.3f,0.0f,0.0f); // espaco entre os blocos
		if ((i+1)%5 == 0 ){
			posiAliensy += -0.2f; // posicao dos aliens em y  
			glTranslatef(-1.5f,-0.2f,0.0f);	// Volta para a esquerda e desce para a linha de baixo		
			posiAliensx = 0.0f;
		}

		for (int j = 0; j < 10; j++)
		{
			if (tiroNave[j] == 0) // tiro esta ativo
			{
				//printf("posiAliens_x %0.2f & posiAliens_y %0.2f\n", posiAliens_x[i], posiAliens_y[i]);
				//printf("x = %0.2f e y = %0.2f\n",misselNave_x[i], misselNave_y[i]);

			
				if ((misselNave_x[j] >= posiAliens_x[i])  && (misselNave_x[j] <= posiAliens_x[i] + 0.1))
				{
					if ((misselNave_y[j] >= posiAliens_y[i]) && misselNave_y[j] <= posiAliens_y[j] + 0.1){
						alien_is_live[i] = 0;
						tiroNave[j] = 1;
						//printf("posiAliens_x %0.2f & posiAliens_y %0.2f\n", posiAliens_x[i],posiAliens_y[i]);
						//printf("x = %0.2f e y = %0.2f\n",misselNave_x[i], misselNave_y[i]);
						//printf("certo\n");

					}	
				}
			}
			if (misselNave_y[j] >= 2) // altura da tela
			{
				//misselNave_y[j] = -1.0;
				//misselNave_x[j] = 0.1;
				tiroNave[j] = 1; // volta ao inicio.
			}

		}
	}
	posiAliensy = 0.0f;

	// Volta o tanto que foi transladado para desenhar a matriz de aliens
	glTranslatef(-0.1f*floor(counter/1),0.1f*line_down,0.0f);
	
	
	// Retorna ao ponto em que estava no inicio da funcao
	glTranslatef(0.8f,-0.2f,0.0f);
	if (counter > 8) // manda para o inicio do lado esquerdo
	{
		counter = 1;
	}
	
}

void move_misselNave(int passo){
	
	int i;
	for (i = 0; i < 10; i++)
	{
		if (tiroNave[i] == 0)
		{
			misselNave_y[i] += (1.0*passo)/100;
			//printf("tiro_y: %0.2f  -- tiro_x :%0.2f\n ", misselNave_y[i], misselNave_x[i]);
			/*if (misselNave_y[i] >= 2) // altura da tela
			{
				misselNave_y[i] = -1;
				misselNave_x[i] = 0.1;
				tiroNave[i] = 1; // volta ao inicio.
			}*/
		
		}
	}
	glutPostRedisplay();
	glutTimerFunc(14, move_misselNave, passo); // 14 eh a velocidade do tiro


}

// Função para desenhar o jatinho           
void DesenhaAviao() {

	glColor3f(1.0f, 1.0f, 0.0f);
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

	int i = 0 ;
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2);


	while (i < 10) // 0 se foi disparado
	{
		if (tiroNave[i] == 0){
			glBegin(GL_POLYGON);
				glVertex2f(-missel1, -missel1);
				glVertex2f(-missel1, -missel2);
				glVertex2f(-missel5, -missel4);
				glVertex2f(-missel3, -missel2);
				glVertex2f(-missel3, -missel1);
			glEnd();
		}
		i++;
	}
	
}
           
// Função callback de redesenho da janela de visualização
void Desenha(void)
{	
	int i;
	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transformação corrente
	glLoadIdentity();
     
	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);

	DesenhaAlien();
	

	/*Posicao do aviao*/
	glTranslatef(aviao_x,0.0f,0.0f);
	glTranslatef(0.0f,-0.7f,0.0f);

	glPushMatrix();

	//Missel 1
	glLoadIdentity();

	for( i= 0; i < 10; i++){
		if (tiroNave[i] == 0)
		{	

			glPushMatrix();
			glTranslatef(misselNave_x[i], misselNave_y[i], 0.0f); // a soma na misselNave_y e a posicao inicial do tiro
			//printf("x = %0.2f e y = %0.2f\n",misselNave_x[i], misselNave_y[i]);

			DesenhaMisseis();
			glPopMatrix();
		}
	}	

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
void movimentoAlien(int passo)
{
	counter++;
	glutTimerFunc(velocidadeAlien, movimentoAlien, 1); // define aqui a velocidade do alien

}

void tecla_direita() {
	int i;
	// Move aviao para direita
	aviao_x += 0.1;

	// Move missel se ele ja foi disparado

	/*for( i= 0; i < 10; i++){
		if (tiroNave[i] == 0)
			misselNave_x[i] -= 0.1;	
	}*/
}

void tecla_esquerda() {
	int i;
	// Move aviao para esquerda
	aviao_x -= 0.1;
	
	// Move missel se ele ja foi disparado
/*	for( i= 0; i < 10; i++){
		if (tiroNave[i] == 0)
			misselNave_x[i] += 0.1;	
		
	}*/
}

void tecla_cima(){

	missel1_moving = true;
	
	quantTiros++;
	if (quantTiros % 10 == 0){ // a cada 5 tiros desce uma coluna
		line_down++; // colocar um delemitador
		velocidadeAlien-= 200; // velocidade dos aliens a cada 7 tiros 
	}
	tiroNave[numeTiro] = 0; // para desenhar
	misselNave_x[numeTiro] = aviao_x - 0.1f; // posicao na hora do tiro;
	misselNave_y[numeTiro] = -0.8f;
	numeTiro++;	
	
	if (numeTiro == 10)
		numeTiro = 0;
	glutPostRedisplay();
	
	if (flag == 0){ // para chamar somente uma vez as funcoes abaixo
		glutTimerFunc(1, movimentoAlien, 1);
		glutTimerFunc(1, move_misselNave, 1);
		flag = 1;	
	}
	
	
}



// Função callback chamada para gerenciar eventos de teclas especiais(F1,PgDn,...)
void TeclasEspeciais(int key, int x, int y)
{
	// Move a base
	if (key == GLUT_KEY_LEFT)
		tecla_esquerda();

	if (key == GLUT_KEY_RIGHT)
		tecla_direita();

	if (key == GLUT_KEY_UP) 
		tecla_cima();


	glutPostRedisplay();
}


// Função callback chamada para gerenciar eventos de teclas
void Teclado(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'a')
		tecla_esquerda();
	if (key == 'd')
		tecla_direita();
	if (key == 'w')
		tecla_cima();
	
	glutPostRedisplay();
}
           
// Função responsável por inicializar parâmetros e variáveis
void Inicializa(void)
{   
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  	gluOrtho2D(0, 1300, 0, 800);
  	  glMatrixMode(GL_PROJECTION); // Especificações de observação de cena
}



// Programa Principal 
int main(int argc, char* argv[])
{
	glutInit(&argc, argv); // Inicia uma instância da glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	//glutInitWindowPosition(1300,800);     
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
