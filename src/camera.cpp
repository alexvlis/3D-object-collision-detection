#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#include "camera.hpp"

Camera::Camera(void) 
: eye({5.0f, 10.0f, 10.0f}) 
, centre({0.0f, 0.0f, 0.0f})
, up({0.0f, 1.0f, 0.0f}) { 

}

Camera::~Camera(void) {
    delete &pan;
    delete &rotate;
}

void Camera::set(void) {
    std::vector<GLfloat> xAxis(3, 0);

    /* Set the camera to a fixed position */
    gluLookAt(eye[0], eye[1], eye[2],
              centre[0], centre[1], centre[2],
              up[0], up[1], up[2]);
    
    /* Rotate the camera */
    geometry.crossProduct(xAxis, eye, up); /* Cross product will give horizontal axis */
    glRotatef(rotate.getY(), xAxis[0], xAxis[1], xAxis[2]); /* Rotate around camera's x axis */
    glRotatef(-rotate.getX(), up[0], up[1], up[2]);

    /* Pan the camera */
    pan.findPanAxis(eye, centre); /* Find the line we need to pan */
    pan.translate(eye, false);
    pan.translate(centre, true);
    pan.resetDelta();

    /* Zoom the camera */
    zoom();
}

void Camera::zoomIn(void) {
    scale = 0.9f;
}

void Camera::zoomOut(void) {
    scale = 1.1f;
}

void Camera::zoom(void) {
    eye[0] *= scale;
    eye[1] *= scale;
    eye[2] *= scale;

    /* Reset the scale */
    scale = 1.0f;
}

void Camera::reset(void) {
    eye = {5.0f, 10.0f, 10.0f};
    centre = {0.0f, 0.0f, 0.0f};

    pan.reset();
    rotate.reset();
}
