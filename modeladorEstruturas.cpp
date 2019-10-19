
#include <iostream>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <utility>

using namespace std;

typedef struct forma {
    vector <GLfloat> vertices;
    vector <GLubyte> faces;
    float tx, ty, tz;
    GLfloat r, g, b;
}forma;

int num_linhas_grid = 40;

int selecionado = -1;

GLfloat pos_lx = 10.0;
GLfloat pos_ly = 3.0;
GLfloat pos_lz = 7.0;

vector < pair<int, forma> > formas;


void Inicializa (void) {  

    glClearColor(229.0/255, 229.0/255, 229.0/255, 1.0f);    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35,1.0f,0.1f,1000);

    GLfloat light_position[] = {pos_lx, pos_ly, pos_lz, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	
	GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

    glMatrixMode (GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);


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
    	formas[selecionado].second.ty += 0.5;  
    } else if (key == 'z') {
    	formas[selecionado].second.ty -= 0.5;  
    }
    glutPostRedisplay();      
}

// Função para armazenar uma primitiva no vetor de formas
void ArmazenaBloco(float altura, float largura, float profundidade, int cor){
    forma bloco;
    bloco.vertices = {
        (5 - largura/2), (0 + altura/2), 0,
        (5 + largura/2), (0 + altura/2), 0,
        (5 - largura/2), (0 - altura/2), 0,
        (5 + largura/2), (0 - altura/2), 0,
        (5 - largura/2), (0 + altura/2), 0-profundidade,
        (5 + largura/2), (0 + altura/2), 0-profundidade,
        (5 - largura/2), (0 - altura/2), 0-profundidade,
        (5 + largura/2), (0 - altura/2), 0-profundidade
    };
    bloco.faces = {
        0, 2, 3, 1,
        1, 3, 7, 5,
        5, 7, 6, 4,
        4, 0, 2, 6,
        4, 5, 1, 0,
        6, 7, 3, 2        
    };
    bloco.tx = 0;
    bloco.ty = 0;
    bloco.tz = 0;
    if (cor == 0) {
        bloco.r = 115.0/255;
        bloco.g = 106.0/255;
        bloco.b = 104.0/255;
    } else if (cor == 1) {
        bloco.r = 35.0/255;
        bloco.g = 37.0/255;
        bloco.b = 42.0/255;
    } else if (cor == 3) {
        bloco.r = 16.0/255;
        bloco.g = 18.0/255;
        bloco.b = 140.0/255;
    } else if (cor == 4) {
        bloco.r = 210.0/255;
        bloco.g = 78.0/255;
        bloco.b = 25.0/255;
    }

    formas.push_back(make_pair(0, bloco));
    selecionado++;
}

// Desenha um hexaedro na cena
void DesenhaBloco(int i) {  
    forma bloco = formas[i].second;
    glPushMatrix();
    glTranslatef(bloco.tx,bloco.ty,bloco.tz);
    glColor3f(bloco.r, bloco.g, bloco.b);    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);  
    glVertexPointer(3, GL_FLOAT, 0, &(bloco.vertices[0]));
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
    //desenha quadrado da luz
	glBegin(GL_POLYGON);
		glVertex3f(pos_lx, pos_ly, pos_lz);
		glVertex3f(pos_lx+0.5, pos_ly+0.5, pos_lz);
		glVertex3f(pos_lx+0.5, pos_ly, pos_lz);
		glVertex3f(pos_lx, pos_ly-0.5, pos_lz);
	glEnd();

 
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

void MenuCorViga(int op) {
    ArmazenaBloco(1, 3, 1, op);
    glutPostRedisplay();
}

void MenuCorBloco(int op) {
    ArmazenaBloco(1, 1, 1, op);
    glutPostRedisplay();
}

void MenuCorColuna(int op) {
    ArmazenaBloco(6, 1, 1, op);
    glutPostRedisplay();
}

void MenuCorParede(int op) {
    ArmazenaBloco(6, 3, 0.1, op);
    glutPostRedisplay();
}  

void MenuCorParedeV(int op) {
    ArmazenaBloco(6, 0.3, 2.5, op);
    glutPostRedisplay();
}  

void CriaMenu() {


    int menu, primitivas, corBloco, corParedeH, corParedeV, corColuna, corViga;

    corBloco = glutCreateMenu(MenuCorBloco);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corColuna = glutCreateMenu(MenuCorColuna);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corParedeH = glutCreateMenu(MenuCorParede);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);        

    corParedeV = glutCreateMenu(MenuCorParedeV);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corViga = glutCreateMenu(MenuCorViga);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);      

    primitivas = glutCreateMenu(MenuPrimitiva);
    glutAddSubMenu("Bloco",corBloco);
    glutAddSubMenu("Parede Horizontal", corParedeH);
    glutAddSubMenu("Parede Vertical", corParedeV);    
    glutAddSubMenu("Coluna", corColuna);
    glutAddSubMenu("Viga", corViga);

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Primitivas",primitivas);
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
