#include <iostream>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <utility>

using namespace std;

typedef struct forma{
    vector <GLfloat> vertices;
    vector <GLubyte> faces;
    GLfloat r, g, b;
}forma;

char texto[500];
GLfloat win;
GLint view_w, view_h;
float coordX = 0, coordY = 0;
float lado = 20;
float zAtual = -1;
GLdouble viewX = 0.0, viewY = 0.0, viewZ = -1.0;
vector < pair<int, forma> > formas;


void DesenhaTexto(char *string) 
{  
  	glPushMatrix();
        // Posição no universo onde o texto será colocado          
        glRasterPos3f(-win,win-(win*0.08), -1); 
        // Exibe caracter a caracter
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++); 
	glPopMatrix();
}

void ArmazenaBloco(float altura, float largura, float profundidade, int cor){
    forma bloco;
    bloco.vertices = { 
        (coordX - largura/2), (coordY + altura/2), zAtual,
        (coordX + largura/2), (coordY + altura/2), zAtual,
        (coordX - largura/2), (coordY - altura/2), zAtual,
        (coordX + largura/2), (coordY - altura/2), zAtual,
        (coordX - largura/2), (coordY + altura/2), zAtual-profundidade,
        (coordX + largura/2), (coordY + altura/2), zAtual-profundidade,
        (coordX - largura/2), (coordY - altura/2), zAtual-profundidade,
        (coordX + largura/2), (coordY - altura/2), zAtual-profundidade 
    };
    bloco.faces = {
        0, 2, 3, 1,
        1, 3, 7, 5,
        5, 7, 6, 4,
        4, 0, 2, 6,
        4, 5, 1, 0,
        6, 7, 3, 2        
    };

    if(cor == 0){
        bloco.r = 115.0/255;
        bloco.g = 106.0/255;
        bloco.b = 104.0/255;
    }
    else if(cor == 1){
        bloco.r = 0.0;
        bloco.g = 0.0;
        bloco.b = 0.0;
    }
    else if(cor == 2){
        bloco.r = 1.0;
        bloco.g = 1.0;
        bloco.b = 1.0;
    }
    else if(cor == 3){
        bloco.r = 16.0/255;
        bloco.g = 18.0/255;
        bloco.b = 140.0/255;
    }
    else if(cor == 4){
        bloco.r = 210.0/255;
        bloco.g = 78.0/255;
        bloco.b = 25.0/255;
    }

    formas.push_back(make_pair(0, bloco));
}

void DesenhaBloco(int i)
{   
    forma bloco = formas[i].second;
    glColor3f(bloco.r, bloco.g, bloco.b);    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);   
    glVertexPointer(3, GL_FLOAT, 0, &(bloco.vertices[0]));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, &(bloco.faces[0]));
    glDisableClientState(GL_VERTEX_ARRAY);
   
}

void Inicializa (void)
{   
    glClearColor(184.0/255, 212.0/255, 245.0/255, 1.0f);
    win = 100;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-win, win, -win, win, 1, win);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewX,viewY,viewZ,0.0,0.0,-1.0, 0.0,1.0,0.0);    

}

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);                
    for(int i=0; i<formas.size(); i++){
        if(formas[i].first == 0){
            DesenhaBloco(i);
        }
    }     

    glColor3f(0.0f, 0.0f, 0.0f);
     DesenhaTexto(texto);
     glFlush ();
     glutSwapBuffers();
}

void MenuPrincipal(int op)
{
    if(op == 0 && formas.size() > 0){
        formas.pop_back();
        glutPostRedisplay();        
    }
}

void MenuPrimitiva(int op){
}

void MenuCorViga(int op)
{
    ArmazenaBloco(10, 30, 0.1, op);
    glutPostRedisplay();
}

void MenuCorBloco(int op)
{
    ArmazenaBloco(10, 10, 0.1, op);
    glutPostRedisplay();
}

void MenuCorColuna(int op)
{
    ArmazenaBloco(30, 10, 0.05, op);
    glutPostRedisplay();
}

void MenuCorParede(int op)
{
    ArmazenaBloco(30, 60, 0.01, op);
    glutPostRedisplay();
}   

void MenuCorParedeV(int op)
{
    ArmazenaBloco(30, 5, 0.5, op);
    glutPostRedisplay();
}   

void mudaVisao(unsigned char key, int x, int y){
    if(key == 'w'){
        viewY += 0.1;
    }
    if(key == 's'){
        viewY -= 0.1;
    }
    if(key == 'a'){
        viewX -= 0.1;
    }
    if(key == 'd'){
        viewX += 0.1;        
    } 
    glutPostRedisplay();       
}

void CriaMenu() 
{


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

void MoveMouse(int x, int y)
{
     coordX = -100.0 + 200.0 * (double)x / (double)view_w; 
     coordY = 100.0 - 200.0 * (double)y / (double)view_h;   
     glutPostRedisplay();
}

void GerenciaMouse(int button, int state, int x, int y)
{        
    if (button == GLUT_RIGHT_BUTTON){
         if (state == GLUT_DOWN){
            CriaMenu();  
            glutPostRedisplay();                                         
         }
    } 
}

void TeclasEspeciais(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
           zAtual -= 0.1;
           if (zAtual < -win) zAtual = -win;
    }
    if(key == GLUT_KEY_DOWN) {
           zAtual += 0.1;
           if (zAtual > -1.0) zAtual = -1;
    }
    sprintf(texto, "Profundidade: %0.2f", zAtual);           
    glutPostRedisplay();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{ 
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;                   

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-win, win, -win, win, 1, win);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewX,viewY,viewZ,0.0,0.0,-1.0, 0.0,1.0,0.0);   
}

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);    
    glutInitWindowSize(350,300);
    glutInitWindowPosition(15,10);
    glutCreateWindow("Exemplo de Menu e Exibição de Caracteres");
    glutPassiveMotionFunc(MoveMouse);    
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(mudaVisao);
    glutDisplayFunc(Desenha);   
    glutReshapeFunc(AlteraTamanhoJanela); 
    glutSpecialFunc(TeclasEspeciais); 
    Inicializa();
    glutMainLoop();

    return 0;
}