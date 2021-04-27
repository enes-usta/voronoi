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
#include <unordered_map>

#define MAX_ARRAY 1000

vector<Face<char>*>* faces_GLOBAL;
vector<Sommet<Vecteur2D>*>* sommets_GLOBAL;

GLfloat ctrlpoints[4][3] = {
        { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0},
        {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0} };


/**
* S est la nature de l'information porte par une arte
*/
class GUI {
public:
    int windowWidth = 800;
    int windowHeight = 800;
    float scale_factor = 0.75;


    /* Main function: GLUT runs as a console application starting at main()  */
    GUI(int argc, char** argv) {
        glutInit(&argc, argv);          // Initialize GLUT        
        glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
        int x = glutGet(GLUT_SCREEN_WIDTH) / 2 - windowWidth / 2;
        int y = glutGet(GLUT_SCREEN_HEIGHT) / 2 - windowHeight / 2;
        glutInitWindowPosition(x, y); // Position the window's initial top-left corner
        glutCreateWindow("Vorono. TEKELI USTA");  // Create window with the given title
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutDisplayFunc(render);       // Register callback handler for window re-paint event
        glutReshapeFunc(reshape);       // Register callback handler for window re-size event
        initGL();                       // Our own OpenGL initialization
    }

    /**
    * Dessine la liste des faces passe en paramtre
    */
    void dessiner(vector<Face<char>*>* faces, vector<Sommet<Vecteur2D>*>* sommets) {
        // On met  l'chelle les faces
        faces_GLOBAL = scale(faces);
        sommets_GLOBAL = scale(sommets);
        glutMainLoop();// Enter the event-processing loop
    }


private:
    /* Initialize OpenGL Graphics */
    void initGL() {
        // Set "clearing" or background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
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

    static void dessinerFaces() {
        for (Face<char>* face : (*faces_GLOBAL)) {
            glBegin(GL_LINE_LOOP);
            for (ArcTU<char> arc : face->arcs) {
                if (arc.bonSens)
                    glVertex2f((float)arc.arete->debut->v.x, (float)arc.arete->debut->v.y);
                else
                    glVertex2f((float)arc.arete->fin->v.x, (float)arc.arete->fin->v.y);
            }
            glEnd();
        }
    }

    static void dessinerSommetsATrianguler() {
        glPointSize(5);
        if (sommets_GLOBAL->size()) {
            glBegin(GL_POINTS);
            for (Sommet<Vecteur2D>* s : (*sommets_GLOBAL))
                glVertex2f((float)s->v.x, (float)s->v.y);

            glEnd();
        }
    }

    static void dessinerSommets() {
        glPointSize(5);

        for (Face<char>* face : (*faces_GLOBAL)) {
            glBegin(GL_POINTS);
            for (ArcTU<char> arc : face->arcs) {
                if (arc.bonSens)
                    glVertex2f((float)arc.arete->debut->v.x, (float)arc.arete->debut->v.y);
                else
                    glVertex2f((float)arc.arete->fin->v.x, (float)arc.arete->fin->v.y);
            }
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
        glColor3f(1.0f, 1.0f, 1.0f); // Blanc
        dessinerFaces();

        //On dessine les sommets
        glColor3f(0.0f, 0.0f, 1.0f); // Bleu
        dessinerSommetsATrianguler();

        //On dessine les sommets
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
        dessinerSommets();

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

    /**
   * Met  l'chelle les faces
   * Les faces sont  l'chelle quand tout -1 <= x <= 1 et -1 <= y <=1
   */
    vector<Face<char>*>* scale(vector<Face<char>*>* faces) {
        double maxX = 0;
        double maxY = 0;
        int absXArc, absYArc;
        bool scaled[MAX_ARRAY] = { false };

        if (needToResize(faces)) {
            for (Face<char>* face : (*faces))
                for (ArcTU<char> arc : face->arcs) {
                    // On calcule la coordonne la plus loigne en x et en y
                    absXArc = abs(arc.debut()->v.x);
                    absYArc = abs(arc.debut()->v.y);

                    if (maxX < absXArc)
                        maxX = absXArc;
                    if (maxY < absYArc)
                        maxY = absYArc;
                }

            for (Face<char>* face : (*faces))
                for (ArcTU<char> arc : face->arcs) {
                    // On met  l'helle chaque coordonne
                    if (!scaled[arc.debut()->clef]) {
                        arc.debut()->v.x /= (maxX / scale_factor);
                        arc.debut()->v.y /= (maxY / scale_factor);
                        scaled[arc.debut()->clef] = true;
                    }

                }
        }
        else {
            cout << "fdsf" << endl;

            for (Face<char>* face : (*faces))
                for (ArcTU<char> arc : face->arcs) {
                    // On met  l'helle chaque coordonne
                    if (!scaled[arc.debut()->clef]) {
                        arc.debut()->v.x *= scale_factor;
                        arc.debut()->v.y *= scale_factor;
                        scaled[arc.debut()->clef] = true;
                    }

                }
        }

        return faces;
    }

    /**
    * Met  l'chelle les sommets
    * Les sommets sont  l'chelle quand tout -1 <= x <= 1 et -1 <= y <=1
    */
    vector<Sommet<Vecteur2D>*>* scale(vector<Sommet<Vecteur2D>*>* sommets) {
        double maxX = 0;
        double maxY = 0;
        int absXArc, absYArc;
        if (needToResize(sommets)) {
            for (Sommet<Vecteur2D>* s : (*sommets)) {
                absXArc = abs(s->v.x);
                absYArc = abs(s->v.y);

                if (maxX < absXArc)
                    maxX = absXArc;
                if (maxY < absYArc)
                    maxY = absYArc;
            }

            for (Sommet<Vecteur2D>* s : (*sommets)) {
                s->v.x /= (maxX / scale_factor);
                s->v.y /= (maxY / scale_factor);
            }
        }
        else {
            for (Sommet<Vecteur2D>* s : (*sommets)) {
                s->v.x *= scale_factor;
                s->v.y *= scale_factor;
            }
        }


        return sommets;
    }

    /** Retourne vrai si on doit (et on peut) scale les sommets
    *   On ne peut pas scale si il y a une valeur entre 0 et 1 (car la division de mise  l'chelle multiplierait)
    */
    bool needToResize(vector<Face<char>*>* faces) {
        for (Face<char>* face : (*faces))
            for (ArcTU<char> arc : face->arcs)
                if ((abs(arc.debut()->v.x) < 1 && abs(arc.debut()->v.x) > 0)
                    || (abs(arc.debut()->v.y) < 1 && abs(arc.debut()->v.y) > 0))
                    return false;

        return true;
    }

    /** Retourne vrai si on doit (et on peut) scale les sommets
    *   On ne peut pas scale si il y a une valeur entre 0 et 1 (car la division de mise  l'chelle multiplierait)
    */
    bool needToResize(vector<Sommet<Vecteur2D>*>* sommets) {
        for (Sommet<Vecteur2D>* s : (*sommets))
            if ((abs(s->v.x) < 1 && abs(s->v.x) > 0)
                || (abs(s->v.y) < 1 && abs(s->v.y) > 0))
                return false;

        return true;
    }

};
