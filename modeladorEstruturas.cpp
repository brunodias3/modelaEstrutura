
#include <iostream>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <utility>
#include "load_bmp.h"

using namespace std;

typedef struct forma {
    vector <GLfloat> vertices;
    vector <GLubyte> faces;
    vector <GLfloat> normais;
    vector <GLfloat> textura;
    int textop;
    float tx, ty, tz;
}forma;

int num_linhas_grid = 40;

int selecionado = -1;

GLfloat pos_lx = 8.0;
GLfloat pos_ly = 3.0;
GLfloat pos_lz = 5.0;
GLuint textureID[10]; 

vector < pair<int, forma> > formas;

void carregaTextura(int posicao, const char* caminho){
  	unsigned int width, height;
  	unsigned char *data;

  	glBindTexture(GL_TEXTURE_2D, textureID[posicao]);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  	data = loadBMP(caminho, width, height);
  	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	

}

void Inicializa (void) {  

    glClearColor(229.0/255, 229.0/255, 229.0/255, 1.0f);    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35,1.0f,0.1f,1000);

    GLfloat light_position[] = {pos_lx, pos_ly, pos_lz, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	
	GLfloat light_diffuse[]={1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel (GL_SMOOTH);

    glMatrixMode (GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

  	glGenTextures(10, textureID);

  	carregaTextura(0, "texturas/concreto.bmp");
  	carregaTextura(1, "texturas/tijolos.bmp");
  	carregaTextura(2, "texturas/tijolos_xadrez.bmp");
  	carregaTextura(3, "texturas/pedras.bmp");
  	carregaTextura(4, "texturas/madeira.bmp");
  	carregaTextura(5, "texturas/azulejo_marmore.bmp");
  	carregaTextura(6, "texturas/azulejo_ceramica.bmp");

	glEnable(GL_TEXTURE_2D);

}


void DesenhaGrid() {
    for (int i = 0; i < num_linhas_grid; i++) {
        glPushMatrix();
        if (i < num_linhas_grid/2){
            glTranslatef(0, 0, i);
        } else {
            glTranslatef(i - (num_linhas_grid / 2), 0, 0);
            glRotatef(-90, 0, 1, 0);
        }
        glBegin(GL_LINES);
            glColor3f(81.0/255, 155.0/255, 79.0/255);
            glLineWidth(1);
            glVertex3f(0, -0.1, 0);
            glVertex3f((num_linhas_grid/2)-1, -0.1, 0);
        glEnd();
        glPopMatrix();
    }
}

void TrocaPrimitiva(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT) {
		selecionado++;
		if (selecionado > formas.size() - 1) {
			selecionado = 0;
		}
	} else if (key == GLUT_KEY_LEFT) {
		selecionado--;
		if (selecionado < 0) {
			selecionado = formas.size() - 1;
		}
	}
	glutPostRedisplay();
}

void ControlaTeclado(unsigned char key, int x, int y){
    if (key == 'w') {
        formas[selecionado].second.tz -= 0.5;
    } else if (key == 's') {
        formas[selecionado].second.tz += 0.5;
    } else if (key == 'a') {
        formas[selecionado].second.tx -= 0.5;
    } else if (key == 'd') {
         formas[selecionado].second.tx += 0.5;     
    } else if (key == 'q') {
    	formas[selecionado].second.ty += 0.25;  
    } else if (key == 'z') {
    	formas[selecionado].second.ty -= 0.25;  
    }
    glutPostRedisplay();      
}

// Função para armazenar uma primitiva no vetor de formas
void ArmazenaBloco(float altura, float largura, float profundidade, int op, int pv){
    forma bloco;
    bloco.vertices = {
        (5 - largura/2), (altura/2 + altura/2), 0,
        (5 + largura/2), (altura/2 + altura/2), 0,
        (5 - largura/2), (altura/2 - altura/2), 0,
        (5 + largura/2), (altura/2 - altura/2), 0,
        (5 - largura/2), (altura/2 + altura/2), 0-profundidade,
        (5 + largura/2), (altura/2 + altura/2), 0-profundidade,
        (5 - largura/2), (altura/2 - altura/2), 0-profundidade,
        (5 + largura/2), (altura/2 - altura/2), 0-profundidade
    };
    bloco.faces = {
        0, 2, 3, 1,
        1, 3, 7, 5,
        5, 7, 6, 4,
        4, 0, 2, 6,
        4, 5, 1, 0,
        6, 7, 3, 2        
    };
    bloco.normais = {
    	1.0, 1.0, 1.0,
    	1.0, 1.0, 1.0,
    	1.0, -1.0, 1.0,
    	1.0, -1.0, 1.0,
    	1.0, 1.0, -1.0,
    	1.0, 1.0, -1.0,
    	1.0, -1.0, -1.0,
    	1.0, -1.0, -1.0
    };
    if (pv == 0) {
	    bloco.textura = {
	    	0.0, 1.0,
	    	1.0, 1.0,
	    	0.0, 0.0,
	    	1.0, 0.0,
	    	0.0, 1.0,
	    	1.0, 1.0,
	    	0.0, 0.0,
	    	1.0, 0.0    	
	    };
	} else if (pv == 1) {
		bloco.textura = {
			1.0, 1.0,
			0.0, 1.0,
			1.0, 0.0,
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0
		};
	} else if (pv == 2){
		bloco.textura = {
			0.0, 1.0,
			0.0, 0.0,
			0.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			1.0, 0.0,
			1.0, 1.0,
			1.0, 0.0
		};
	}
    bloco.tx = 0;
    bloco.ty = 0;
    bloco.tz = 0;

    bloco.textop = op;

    formas.push_back(make_pair(0, bloco));
    selecionado = formas.size()-1;
}

// Desenha um hexaedro na cena
void DesenhaBloco(int i) {  
    forma bloco = formas[i].second;
    glPushMatrix();
    glTranslatef(bloco.tx,bloco.ty,bloco.tz);
  	glBindTexture(GL_TEXTURE_2D, textureID[bloco.textop]); 
  	glColor3f(115.0/255, 106.0/255, 104.0/255);   
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);  
    glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
    glEnable(GL_DEPTH_TEST);  
    glVertexPointer(3, GL_FLOAT, 0, &(bloco.vertices[0]));
    glNormalPointer(GL_FLOAT, 0, &(bloco.normais[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(bloco.textura[0]));
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, &(bloco.faces[0]));
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Desenha() {
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13,0,-45);
    glRotatef(40,1,1,0);    
    DesenhaGrid();

    glColor3f(0.0, 0.0, 0.0);

 
    // Percorre o vetor de formas todas as vezes que o display for chamado                
    for (int i=0; i<formas.size(); ++i) {
     // Se a forma for um hexaedro, chama a função DesenhaBloco
        if(formas[i].first == 0) {
             DesenhaBloco(i);
        }
    }    
    glFlush ();
    glutSwapBuffers();
}

void MenuPrincipal(int op)
{
// Remove a última forma do vetor de formas (Desfazer)
    if(op == 0 && formas.size() > 0) {
        formas.pop_back();  
        selecionado--;      
    }
}

void MenuPrimitiva(int op) {
}

void MenuTextViga(int op) {
    ArmazenaBloco(1, 3, 1, op, 0);
    glutPostRedisplay();
}

void MenuTextBloco(int op) {
    ArmazenaBloco(1, 1, 1, op, 0);
    glutPostRedisplay();
}

void MenuTextColuna(int op) {
    ArmazenaBloco(6, 1, 1, op, 0);
    glutPostRedisplay();
}

void MenuTextParede(int op) {
    ArmazenaBloco(6, 4, 0.1, op, 0);
    glutPostRedisplay();
}  

void MenuTextParedeV(int op) {
    ArmazenaBloco(6, 0.3, 5, op, 1);
    glutPostRedisplay();
} 

void MenuTextTeto(int op) {
	ArmazenaBloco(0.4, 4, 5, op, 2);
    glutPostRedisplay();	
}

void CriaMenu() {


    int menu, estruturas, textBloco, textParedeH, textParedeV, textColuna, textViga, textTeto;

    textBloco = glutCreateMenu(MenuTextBloco);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6); 

    textColuna = glutCreateMenu(MenuTextColuna);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6); 

    textParedeH = glutCreateMenu(MenuTextParede);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6);       

    textParedeV = glutCreateMenu(MenuTextParedeV);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6); 

    textViga = glutCreateMenu(MenuTextViga);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6);    

    textTeto = glutCreateMenu(MenuTextTeto);
    glutAddMenuEntry("Concreto", 0);
    glutAddMenuEntry("Tijolos", 1);
    glutAddMenuEntry("Tijolos Xadrez", 2);
    glutAddMenuEntry("Pedras", 3);
	glutAddMenuEntry("Madeira", 4);    
    glutAddMenuEntry("Azulejo Marmore", 5);
    glutAddMenuEntry("Azulejo Azul Ceramica", 6);      

    estruturas = glutCreateMenu(MenuPrimitiva);
    glutAddSubMenu("Bloco",textBloco);
    glutAddSubMenu("Parede Horizontal", textParedeH);
    glutAddSubMenu("Parede Vertical", textParedeV);
    glutAddSubMenu("Teto", textTeto);        
    glutAddSubMenu("Coluna", textColuna);
    glutAddSubMenu("Viga", textViga);


    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Estruturas", estruturas);
    glutAddMenuEntry("Desfazer", 0);
   
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Mouse function para criação do menu com o botão direito
void GerenciaMouse(int button, int state, int x, int y) {        
    if (button == GLUT_RIGHT_BUTTON) {
         if (state == GLUT_DOWN) {
            CriaMenu();  
            glutPostRedisplay();                                        
        }
    } 
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);    
    glutInitWindowSize(800,600);
    glutInitWindowPosition(15,10);
    glutCreateWindow("Exemplo de Menu e Exibição de Caracteres");
    //glutPassiveMotionFunc(MoveMouse);    
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(ControlaTeclado); 
    glutDisplayFunc(Desenha);  
    //glutReshapeFunc(AlteraTamanhoJanela);
    glutSpecialFunc(TrocaPrimitiva);
    Inicializa();
    glutMainLoop();

    return 0;
}
