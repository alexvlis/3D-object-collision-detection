#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>

#include "cameraMovements.hpp"
#include "geometry.hpp"

class Camera {
    public:
        Camera(void);
        ~Camera(void);

        void zoomIn(void);
        void zoomOut(void);
        void set(void);
        void reset(void);

        Pan& pan = *(new Pan);
        Rotate& rotate = *(new Rotate);

    private:
        void zoom(void);

        std::vector<GLfloat> eye;
        std::vector<GLfloat> centre;
        std::vector<GLfloat> up;
    
        GLfloat scale = 1.0f;

        Geometry geometry;
};


#endif /* CAMERA_HPP */
