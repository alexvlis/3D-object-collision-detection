#ifndef MODULE_HPP
#define MODULE_HPP

#include "camera.hpp"
#include "model.hpp"
#include "plane.hpp"

class Module {
    public:
        Module();
        ~Module();

        /* Plane interface */
        void setPlane(void);
        void translatePlane(int x, int y);
        void planeCapture(int x, int y) {plane.capture(x, y);}
        bool isPlaneSelected(void) {return planeSelectFlag;}
        void selectPlane(void) {planeSelectFlag = !planeSelectFlag;}
        void clip(void);

        /* Model interface */
        void rotateModelUp(void);
        void rotateModelDown(void);
        void rotateModelLeft(void);
        void rotateModelRight(void);
        void rotateModelClockwise(void);
        void rotateModelCounterclockwise(void);
        void setModel(eModel emodel);
        Model *hasModel(void) {return model;}
        void setBoundingBox(eBoundingBox eBox) {model->setBoundingBox(eBox);}
        void drawSmooth(void) {model->smooth();}

        /* General module interface */
        void draw(void);
        void setRenderMode(GLuint mode);
        GLfloat* getLightPosition(void) {return lightposition.data();}
        void setLeftButtonState(bool value) {leftbuttonstate = value;}
        void setRightButtonState(bool value) {rightbuttonstate = value;}
        bool getLeftButtonState(void) {return leftbuttonstate;}
        bool getRightButtonState(void) {return rightbuttonstate;}
        Camera& getCamera(void) {return camera;}
        void drawAxis(void);
        void reset(void);
        void setupTexture(void);
        GLuint *getTextureID(void);

    private:
        unsigned char *image[2];
        GLuint *g_textureID;
        unsigned char rendermode;
        std::vector<GLfloat> lightposition;
        bool leftbuttonstate;
        bool rightbuttonstate;
        bool planeflag;
        bool planeSelectFlag;
        bool drawnothing;
        bool drawclipped;

        Camera camera;
        Model *model;
        Model& bunny = *(new Model("bunny.obj"));
        Model& screwdriver = *(new Model("screwdriver.obj"));
        Plane plane;
};

#endif /* MODULE_HPP */
