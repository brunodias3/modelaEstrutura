
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
    int tx, ty, tz;
    GLfloat r, g, b;
}forma;

int num_linhas_grid = 40;
GLdouble posX, posY, posZ;


vector < pair<int, forma> > formas;


void Inicializa (void) {  

    glClearColor(229.0/255, 229.0/255, 229.0/255, 1.0f);    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35,1.0f,0.1f,1000);
    glMatrixMode (GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

/*void DesenhaCubo() {
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(a,b,c);
    glutSolidCube(2);
    glPopMatrix();
}*/

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

// Keyboard function para alterar a posição da câmera
/*void mudaVisao(unsigned char key, int x, int y){
    if (key == 'w') {
        viewY += 0.001;
    } else if (key == 's') {
        viewY -= 0.001;
    } else if (key == 'a') {
        viewX -= 0.001;
    } else if (key == 'd') {
        viewX += 0.001;        
    }
    glutPostRedisplay();      
}*/

// Passive Motion function para pegar a coordenada do mouse normalizada
void MoveMouse(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    cout << posX << " " << posY << " " << posZ << endl;

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
        bloco.r = 0.0;
        bloco.g = 0.0;
        bloco.b = 0.0;
    } else if (cor == 2) {
        bloco.r = 1.0;
        bloco.g = 1.0;
        bloco.b = 1.0;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13,0,-45);
    glRotatef(40,1,1,0);    
    DesenhaGrid();
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
    }
}

void MenuPrimitiva(int op) {
}

void MenuCorViga(int op) {
    ArmazenaBloco(10, 30, 0.1, op);
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
    ArmazenaBloco(6, 6, 0.1, op);
    glutPostRedisplay();
}  

void MenuCorParedeV(int op) {
    ArmazenaBloco(6, 0.3, 5, op);
    glutPostRedisplay();
}  

void CriaMenu() {


    int menu, primitivas, corBloco, corParedeH, corParedeV, corColuna, corViga;

    corBloco = glutCreateMenu(MenuCorBloco);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Branco", 2);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corColuna = glutCreateMenu(MenuCorColuna);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Branco", 2);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corParedeH = glutCreateMenu(MenuCorParede);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Branco", 2);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);        

    corParedeV = glutCreateMenu(MenuCorParedeV);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Branco", 2);
    glutAddMenuEntry("Azul", 3);
    glutAddMenuEntry("Vermelho", 4);

    corViga = glutCreateMenu(MenuCorViga);
    glutAddMenuEntry("Cinza Concreto", 0);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Branco", 2);
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
    glutInitDisplayMode(GLUT_DOUBLE);    
    glutInitWindowSize(800,600);
    glutInitWindowPosition(15,10);
    glutCreateWindow("Exemplo de Menu e Exibição de Caracteres");
    glutPassiveMotionFunc(MoveMouse);    
    glutMouseFunc(GerenciaMouse);
    //glutKeyboardFunc(mudaVisao); 
    glutDisplayFunc(Desenha);  
    //glutReshapeFunc(AlteraTamanhoJanela);
    //glutSpecialFunc(TeclasEspeciais);
    Inicializa();
    glutMainLoop();

    return 0;
}
