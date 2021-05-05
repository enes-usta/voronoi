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
GLfloat world_left = 0, world_right = 0, world_bottom = 0, world_top = 0;

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
        changement_repere(faces, sommets);
        glutMainLoop();// Enter the event-processing loop
    }


private:
    /* Initialize OpenGL Graphics */
    void initGL() {
        // Set "clearing" or background color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Black and opaque
        glLineWidth(1);
        glPointSize(5);

    }

    /* Handler for window-repaint event. Call back when the window first appears and
               whenever the window needs to be re-painted. */
    static void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color
        //On dessine les axes
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        //dessinerAxes();

        //On dessine les faces
        glColor3f(0.0f, 0.0f, 0.0f); // Noir
        dessinerFaces();

        //On dessine les arêtes
        glColor3f(0.0f, 0.0f, 0.0f); // Blanc
        dessinerAretes();

        //On dessine les sommets
        glColor3f(0.0f, 0.0f, 1.0f); // Bleu
//        dessinerSommets();

        //On dessine les sommets des faces
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
//        dessinerSommetsFaces();

        glFlush();  // Render now
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

        cout << world_left<< endl;
        cout << world_right << endl;
        cout << world_bottom << endl;
        cout << world_top << endl << endl;

        if (width >= height) {
            // aspect >= 1, set the height from -1 to 1, with larger width
            gluOrtho2D(world_left * aspect, world_right * aspect, world_bottom, world_top);
        }
        else {
            // aspect < 1, set the width to -1 to 1, with larger height
            gluOrtho2D(world_left, world_right, world_bottom / aspect, world_top / aspect);
        }
    }

    void changement_repere(vector<Face<Color*, Color*>*>* faces, vector<Sommet<Vecteur2D>*>* sommets) {
        if (sommets == NULL)
            sommets = new vector<Sommet<Vecteur2D>*>;

        if (faces == NULL)
            faces = new vector<Face<Color*, Color*>*>;

        // Calcul des coins dans les sommets et les faces
        for (Sommet<Vecteur2D>* s : (*sommets)) {
            world_left = min(world_left, s->v.x);
            world_right = max(world_right, s->v.x);
            world_bottom = min(world_bottom, s->v.y);
            world_top = max(world_top, s->v.y);
        }

        for (Face<Color*, Color*>* face : (*faces))
            for (ArcTU<Color*>* arc : face->arcs) {
                int x = arc->debut()->v.x;
                int y = arc->debut()->v.y;
                world_left = min(world_left, x);
                world_right = max(world_right, x);
                world_bottom = min(world_bottom, y);
                world_top = max(world_top, y);
            }

        sommets_GLOBAL = sommets;
        faces_GLOBAL = faces;

    }
};
