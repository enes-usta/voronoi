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
#include "Arc.h"

/**
* S est la nature de l'information portée par une arête
*/
class GUI {
private:
    vector<Face<char>> faces;
    /* Initialize OpenGL Graphics */
    void initGL() {
        // Set "clearing" or background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    }

    /* Handler for window-repaint event. Call back when the window first appears and
               whenever the window needs to be re-painted. */
    static void display() {
        glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

        /*glBegin(GL_LINE_LOOP);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(50.0f, 50.0f, 0.0f);
        glVertex3f(50.0f, 100.0f, 0.0f);
        glEnd();*/
        glBegin(GL_LINE_LOOP);
        for each (Face<char> face in faces)
            for each (Arc<char> arc in faces->arcs)
                glVertex3f((float)arc->arete->debut->v.x, (float)arc->arete->debut->v.y, 0.0f);
        glEnd();
        glFlush();  // Render now
    }

public:
    void dessiner(vector<Face<char>> faces) {
        this->faces = faces;
    }

    /* Main function: GLUT runs as a console application starting at main()  */
    GUI(int argc, char** argv) {
        glutInit(&argc, argv);          // Initialize GLUT
        glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));   // Set the window's initial width & height
        glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
        glutCreateWindow("Voronoï. TEKELI USTA");  // Create window with the given title
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutDisplayFunc(display);       // Register callback handler for window re-paint event
        initGL();                       // Our own OpenGL initialization
        glutMainLoop();                 // Enter the event-processing loop
    }
};
