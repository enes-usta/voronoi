#pragma once
#include<Windows.h>    
// first include Windows.h header file which is required    
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>    
#include<stdio.h>    
#include<math.h>    
#include<string.h>
#include "Face.h"
#include "Sommet.h"
#include "Color.h"
#include <unordered_map>

#define MAX_ARRAY 100000

vector<Face<Color*, Color*>*>* faces_GLOBAL;
vector<Sommet<Vecteur2D>*>* sommets_GLOBAL;
bool scaled[MAX_ARRAY] = { false };


/**
* S est la nature de l'information porte par une arte
*/
class GUI {
public:
    int windowWidth = 800;
    int windowHeight = 800;

    /* Main function: GLUT runs as a console application starting at main()  */
    GUI(int argc, char** argv) {
        glutInit(&argc, argv);          // Initialize GLUT        
        glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
        int x = glutGet(GLUT_SCREEN_WIDTH) / 2 - windowWidth / 2;
        int y = glutGet(GLUT_SCREEN_HEIGHT) / 2 - windowHeight / 2;
        glutInitWindowPosition(x, y); // Position the window's initial top-left corner
        glutCreateWindow("Voronoï TEKELI USTA");  // Create window with the given title
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutDisplayFunc(render);       // Register callback handler for window re-paint event
        glutReshapeFunc(reshape);       // Register callback handler for window re-size event
        initGL();                       // Our own OpenGL initialization
    }

    /**
    * Dessine les faces et les sommets passés en paramètre
    */
    void dessiner(vector<Face<Color*, Color*>*>* faces, vector<Sommet<Vecteur2D>*>* sommets) {
        // On met  l'chelle les faces
        //faces_GLOBAL = scale(faces);
        //sommets_GLOBAL = scale(sommets);
        scale(faces, sommets);
        glutMainLoop();// Enter the event-processing loop
    }


private:
    /* Initialize OpenGL Graphics */
    void initGL() {
        // Set "clearing" or background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
        glLineWidth(3);
        glPointSize(5);

    }

    static void dessinerAxes() {
        glBegin(GL_LINES);
        glVertex2f(-1.0f, 0);
        glVertex2f(1.0f, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(0, -1.0f);
        glVertex2f(0, 1.0f);
        glEnd();
    }

    static void dessinerAretes() {
        for (Face<Color*, Color*>* face : (*faces_GLOBAL)) {
            glBegin(GL_LINE_LOOP);
            for (ArcTU<Color*>* arc : face->arcs) {
                if(arc->arete->v != nullptr)
                    glColor3f(arc->arete->v->r, arc->arete->v->g, arc->arete->v->b);
                glVertex2f((float)arc->debut()->v.x, (float)arc->debut()->v.y);
            }
            glEnd();
        }
    }

    static void dessinerFaces() {
        for (Face<Color*, Color*>* face : (*faces_GLOBAL)) {
            glBegin(GL_POLYGON);
            if (face->v != nullptr) {
                glColor3f(face->v->r, face->v->g, face->v->b);
                for (ArcTU<Color*>* arc : face->arcs)
                    glVertex2f((float)arc->debut()->v.x, (float)arc->debut()->v.y);
            }

            glEnd();
        }
    }

    static void dessinerSommets() {
        glBegin(GL_POINTS);
        for (Sommet<Vecteur2D>* s : (*sommets_GLOBAL))
            glVertex2f((float)s->v.x, (float)s->v.y);

        glEnd();
    }

    static void dessinerSommetsFaces() {
        for (Face<Color*, Color*>* face : (*faces_GLOBAL)) {
            glBegin(GL_POINTS);
            for (ArcTU<Color*> *arc : face->arcs)
                glVertex2f((float)arc->debut()->v.x, (float)arc->debut()->v.y);
            glEnd();
        }
    }

    /* Handler for window-repaint event. Call back when the window first appears and
               whenever the window needs to be re-painted. */
    static void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color
        //On dessine les axes
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        dessinerAxes();

        //On dessine les faces
        glColor3f(0.0f, 0.0f, 0.0f); // Noir
        dessinerFaces();

        //On dessine les arêtes
        glColor3f(1.0f, 1.0f, 1.0f); // Blanc
        dessinerAretes();

        //On dessine les sommets
        glColor3f(0.0f, 0.0f, 1.0f); // Bleu
        dessinerSommets();

        //On dessine les sommets des faces
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
        dessinerSommetsFaces();

        glFlush();  // Render now
    }

    /* Handler for window re-size event. Called back when the window first appears and
    whenever the window is re-sized with its new width and height */
    static void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
        // Compute aspect ratio of the new window
        if (height == 0) height = 1;                // To prevent divide by 0
        GLfloat aspect = (GLfloat)width / (GLfloat)height;

        // Set the viewport to cover the new window
        glViewport(0, 0, width, height);

        // Set the aspect ratio of the clipping area to match the viewport
        glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
        glLoadIdentity();             // Reset the projection matrix
        if (width >= height) {
            // aspect >= 1, set the height from -1 to 1, with larger width
            gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
        }
        else {
            // aspect < 1, set the width to -1 to 1, with larger height
            gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
        }
    }

    void scale(vector<Face<Color*, Color*>*>* faces, vector<Sommet<Vecteur2D>*>* sommets) {
        if (sommets == NULL)
            sommets = new vector<Sommet<Vecteur2D>*>;

        if (faces == NULL)
            faces = new vector<Face<Color*, Color*>*>;

        // Calcul du max dans les sommets et les faces
        double maxX = 0, minX = 0, maxY = 0, minY = 0;

        for (Sommet<Vecteur2D>* s : (*sommets)) {
            maxX = max(maxX, s->v.x);
            minX = min(minX, s->v.x);
            maxY = max(maxY, s->v.y);
            minY = min(minY, s->v.y);
        }

        for (Face<Color*, Color*>* face : (*faces))
            for (ArcTU<Color*>* arc : face->arcs) {
                int x = arc->debut()->v.x;
                int y = arc->debut()->v.y;
                maxX = max(maxX, x);
                minX = min(minX, x);
                maxY = max(maxY, y);
                minY = min(minY, y);
            }

        // On met  l'chelle
        for (Sommet<Vecteur2D>* s : (*sommets))
            scale(s, maxX, minX, maxY, minY);

        for (Face<Color*, Color*>* face : (*faces))
            for (ArcTU<Color*>* arc : face->arcs)
                scale(arc->debut(), maxX, minX, maxY, minY);


        sommets_GLOBAL = sommets;
        faces_GLOBAL = faces;

    }

    /**
    * Met à l'échelle le sommet passé en paramètre
    */
    void scale(Sommet<Vecteur2D>* sommet, double maxX, double minX, double maxY, double minY) {
        double max1 = max(abs(maxX), abs(minX));
        double max2 = max(abs(maxY), abs(minY));
        int eps1 = 1;
        int eps2 = 1;

        if (maxX - minX < 0)
            eps1 = -1;

        if (maxY - minY < 0)
            eps2 = -1;

        double lambda = 1 / max(max1, max2);
        double a = -((maxX + minX) / 2) / max1;
        double b = -((maxY + minY) / 2) / max2;

        if (!scaled[sommet->clef]) {
            sommet->v.x = sommet->v.x * lambda * eps1 + a;
            sommet->v.y = sommet->v.y * lambda * eps2 + b;
            scaled[sommet->clef] = true;
        }
    }
};
/*
class ChangementRepere {
public:
    double lambda1, lambda2, a, b;

    ChangementRepere(const double lambda1, const double lambda2, const double a, const double b) {
        this->lambda1 = lambda1;
        this->lambda2 = lambda2;
        this->a = a;
        this->b = b;
    }

    ChangementRepere(const Vecteur2D p1, const Vecteur2D p2, const Vecteur2D p1p, const Vecteur2D p2p) {
        // Calculer membres
    }

    const Vecteur2D appliquer(const Vecteur2D& v) const{
        return Vecteur2D(lambda1 * v.x + a, lambda2 * v.y + b);
    }

    const Vecteur2D operator()(const Vecteur2D & v) const{
        return this->appliquer(v);
    }

};
*/