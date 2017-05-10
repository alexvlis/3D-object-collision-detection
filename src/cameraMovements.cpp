#include "cameraMovements.hpp"

/*******************************************************************
                         PAN IMPLEMENTATION
*******************************************************************/

Pan::Pan(void) 
: delta(0.0f)
, oldX(0.0f) 
, m(0.0f)
, c(0.0f) {
    /* Nothing to do */
}

Pan::~Pan(void) {
    /* Nothing to do */
}

/* Actually pan the camera */
void Pan::Do(int x, int y) {
    /* Use previous mouse position to work out the displacement */
    delta = (oldX - x) * speed;
    oldX = x;
}

void Pan::capture(int x, int y) {
    oldX = x;
}

/* This method moves a point along the pan axis */
void Pan::translate(std::vector<GLfloat>& A, bool isCentre) {
    /* If the point is the centre the pan axis passes through the origin */
    if (isCentre) {
        A[0] += delta;
        A[2] = m * A[0];
    } else {
        A[0] += delta;
        A[2] = m * A[0] + c;
    }
}

/* Method to find the line on which the panning happends */
void Pan::findPanAxis(std::vector<GLfloat>& A, std::vector<GLfloat>& B) {
    m = (A[2] - B[2]) / (A[0] - B[0]);
    m = -1/m; /* Slope perpendicular to A and B */
    c = A[2] - m * A[0];
}

void Pan::resetDelta(void) {
    delta = 0.0f;
}

void Pan::reset(void) {
    delta = 0.0f;
    oldX = 0.0f;
}

/********************************************************************
                        ROTATE IMPLEMENTATION                
********************************************************************/

Rotate::Rotate(void) 
: deltaX(0.0f)
, deltaY(0.0f) 
, oldX(0.0f) 
, oldY(0.0f) 
, oldDeltaX(0.0f) 
, oldDeltaY(0.0f) {
    /* Nothing to do */
}

Rotate::~Rotate(void) {
    /* Nothing to do */
}

void Rotate::capture(int x, int y) {
    oldX = x;
    oldY = y;
}

/* Actually do the rotation of the camera */
void Rotate::Do(int x, int y) {
    deltaX = ((oldX - x) * speed) + oldDeltaX;
    deltaY = ((oldY - y) * speed) + oldDeltaY;
}

void Rotate::store(int x, int y) {
    oldDeltaX = deltaX;
    oldDeltaY = deltaY;
}

void Rotate::reset(void) {
    oldX = 0.0f;
    oldY = 0.0f;
    deltaX = 0.0f;
    deltaY = 0.0f;
    oldDeltaX = 0.0f;
    oldDeltaY = 0.0f;
}
