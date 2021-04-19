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

vector<Face<char>> faces_GLOBAL;

/**
* S est la nature de l'information portée par une arête
*/
class GUI {
private:
    /* Initialize OpenGL Graphics */
    void initGL() {
        // Set "clearing" or background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    }

    /* Handler for window-repaint event. Call back when the window first appears and
               whenever the window needs to be re-painted. */
    static void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear the color buffer with current clearing color
        //On dessine les axes
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        glBegin(GL_LINES);
         glVertex2f(-1.0f, 0);
         glVertex2f(1.0f, 0);
        glEnd();
        glBegin(GL_LINES);
         glVertex2f(0, -1.0f);
         glVertex2f(0, 1.0f);
        glEnd();

        //On dessine les faces
        glColor3f(1.0f, 1.0f, 1.0f); // Blanc
        glBegin(GL_LINE_LOOP);
        for (Face<char> face : faces_GLOBAL)
            for (ArcTU<char> arc : face.arcs) {
                if(arc.bonSens)
                    glVertex2f((float)arc.arete->debut->v.x, (float)arc.arete->debut->v.y);
                else
                    glVertex2f((float)arc.arete->fin->v.x, (float)arc.arete->fin->v.y);
            }
        glEnd();
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
        glutCreateWindow("Voronoï. TEKELI USTA");  // Create window with the given title
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutDisplayFunc(render);       // Register callback handler for window re-paint event
        glutReshapeFunc(reshape);       // Register callback handler for window re-size event
        initGL();                       // Our own OpenGL initialization
    }

    /**
    * Dessine la liste des faces passée en paramètre
    */
    void dessiner(vector<Face<char>> faces) {
        // On met à l'échelle les faces
        faces_GLOBAL = scale(faces);;
        glutMainLoop();// Enter the event-processing loop
    }

    /**
    * Met à l'échelle les faces
    * Les faces sont à l'échelle quand tout -1 <= x <= 1 et -1 <= y <=1
    */
    vector<Face<char>> scale(vector<Face<char>> faces) {
        double maxX = 0;
        double maxY = 0;
        int absXArc, absYArc;
        for (Face<char> face : faces)
            for (ArcTU<char> arc : face.arcs) {
                // On calcule la coordonnée la plus éloignée en x et en y
                absXArc = abs(arc.debut()->v.x);
                absYArc = abs(arc.debut()->v.y);
                
                if (maxX < absXArc)
                    maxX = absXArc;
                if(maxY < absYArc)
                    maxY = absYArc;
            }

        for (Face<char> face : faces)
            for (ArcTU<char> arc : face.arcs) {
                // On met à l'éhelle chaque coordonnée
                arc.debut()->v.x /= (maxX / scale_factor);
                arc.debut()->v.y /= (maxY / scale_factor);
            }

        return faces;
    }
};
