#include "module.hpp"

#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#define MOUSE_SCROLL_UP 3
#define MOUSE_SCROLL_DOWN 4

Module *module = NULL;

GLfloat light_position[] = { -30.0f, 25.0f, 25.0f, 1.0f };
GLfloat light_amb[] = {0.09, 0.09, 0.09, 1.0};
GLfloat light_diff[] = {0.01, 0.01, 0.01, 1.0};
GLfloat light_spec[] = {1, 1, 1, 1.0};

/*
 * Scene initialisation
 */
void InitGL(void)
{
    glShadeModel(GL_SMOOTH);                            /* Enable smooth shading */
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f);               /* White background */
    glClearDepth(1.0f);                                 /* Depth buffer setup */
    glEnable(GL_DEPTH_TEST);                            /* Enables depth testing */
    glDepthFunc(GL_LEQUAL);                             /* The type of depth testing to do */
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glShadeModel(GL_SMOOTH);

    /* Setup lights */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void idle (void) {
    glutPostRedisplay();   // trigger display callback
}

void display (void)
{
    Camera& camera = module->getCamera();

    glEnable(GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    /* Fix the camera */
    camera.set();

    /* Draw everything */
    module->draw();

    glDisable(GL_TEXTURE_2D);

    glFlush();
    glutSwapBuffers();
}

/*
 * The reshape function sets up the viewport and projection
 */
void reshape ( int width, int height )
{
    /* Prevent a divide by zero error by making height equal 1 */
    if (height==0) {
        height=1;
    }
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Need to calculate the aspect ratio of the window for gluPerspective
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    // Return to ModelView mode for future operations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Callback for standard keyboard presses
 */
void keyboard ( unsigned char key, int x, int y )
{
        switch(key) {
            /* Exit the program when escape is pressed */
            case 27:
                exit(0);
                break;

            /* Process keyboard keys */
            case 'v':
                module->setRenderMode(GL_POINTS); /* Draw vertices */
                break;
            case 'e':
                module->setRenderMode(GL_LINE_LOOP); /* Draw edges */
                break;
            case 'f':
                module->setRenderMode(GL_TRIANGLES); /* Draw faces */
                break;
            case 's':
                module->drawSmooth();
                break;
            case 'a':
                module->setBoundingBox(eABB);
                break;
            case 'o':
                module->setBoundingBox(eOBB);
                break;
            case 'p':
                module->setPlane();
                break;
            case 'm':
                glutFullScreen(); /* Go fullscreen */
                break;
            case 'n':
                glutReshapeWindow(800, 800);
                break;
            case 'z':
                module->rotateModelClockwise();
                break;
            case 'x':
                module->rotateModelCounterclockwise();
                break;
            case 'r':
                module->reset(); /* Reset the scene */
                break;
            default:
                break;
        }
    glutPostRedisplay();
}

/*
 * Callback for special keys
 */
void special_keys ( int a_keys, int x, int y )
{
    GLuint *texture;

    /* Handle special arrow keys to rotate the cube */
    switch ( a_keys ) {
        case GLUT_KEY_UP:
            module->rotateModelUp();
            break;
        case GLUT_KEY_DOWN:
            module->rotateModelDown();
            break;
        case GLUT_KEY_LEFT:
            module->rotateModelLeft();
            break;
        case GLUT_KEY_RIGHT:
            module->rotateModelRight();
            break;
        case GLUT_KEY_F1:
            module->setModel(eBUNNY);
            break;
        case GLUT_KEY_F2:
            module->setModel(eSCREWDRIVER);
            break;
        case GLUT_KEY_F3:
            module->selectPlane();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

/*
 * Callback to handle mouse clicks
 */
void mouseButton(int button, int state, int x, int y) {
    Camera& camera = module->getCamera();

    /* Use the left button to orbit */
    if (GLUT_LEFT_BUTTON == button && GLUT_DOWN == state) {
        module->setLeftButtonState(true);
        if (module->isPlaneSelected()) {
          module->planeCapture(x, y);
        } else {
          camera.rotate.capture(x, y);
        }
    }else if (GLUT_LEFT_BUTTON == button && GLUT_UP == state) {
        if (!module->isPlaneSelected()) {
          camera.rotate.store(x, y); /* Remember where we left off */
        }
        module->setLeftButtonState(false);
    }

    /* Use right button to pan */
    if (GLUT_RIGHT_BUTTON == button && GLUT_DOWN == state) {
        module->setRightButtonState(true);
        camera.pan.capture(x, y);
    } else if (GLUT_RIGHT_BUTTON == button && GLUT_UP == state) {
        module->setRightButtonState(false);
    }

    /* Use scroll wheel to zoom */
    if (MOUSE_SCROLL_UP == button) {
        camera.zoomIn();
    } else if (MOUSE_SCROLL_DOWN == button) {
        camera.zoomOut();
    }
}

/*
 * Callback to handle mouse movements
 */
void mouseMove(int x, int y) {
    Camera& camera = module->getCamera();

    if (module->getLeftButtonState()) {
      if (module->isPlaneSelected()) {
          module->translatePlane(x, y);
      } else {
        /* Rotate if left button is pressed and the mouse has moved */
        camera.rotate.Do(x, y);
      }
    }
    else if (module->getRightButtonState()) {
        /* Pan if the right button is pressed and the mouse has moved */
        camera.pan.Do(x, y);
    }
}

/*
 * Entry point to the application
 */
int main(int argc, char** argv)
{
    module = new Module;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Awesome Cube");
    InitGL();

    // Callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutIdleFunc(idle);

    glutMainLoop();
    delete module;
    return 0;
}
