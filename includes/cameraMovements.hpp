#ifndef CAMERAMOVEMENTS_HPP
#define CAMERAMOVEMENTS_HPP

#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <vector>

class Pan {
    public:
        Pan(void);
        ~Pan(void);

        GLfloat get(void) {return delta;}

        void Do(int x, int y);
        void capture(int x, int y);

        void translate(std::vector<GLfloat>& A, bool isCentre);
        void findPanAxis(std::vector<GLfloat>& A, std::vector<GLfloat>& B);
        void resetDelta(void);
        void reset(void);

    private:
        GLfloat delta;
        GLfloat oldX;

        GLfloat m;
        GLfloat c;

        const GLfloat speed = 0.01f;
};

class Rotate {
    public:
        Rotate(void);
        ~Rotate(void);

        void Do(int x, int y);
        void capture(int x, int y);
        void store(int x, int y);
        void reset(void);

        GLfloat getX(void) {return deltaX;}
        GLfloat getY(void) {return deltaY;}

    private:
        GLfloat oldX;
        GLfloat oldY;
        GLfloat deltaX;
        GLfloat deltaY;
        GLfloat oldDeltaX;
        GLfloat oldDeltaY;

        const GLfloat speed = 0.1f;
};

#endif /* CAMERAMOVEMEMENTS_HPP */
