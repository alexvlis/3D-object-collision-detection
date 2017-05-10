#ifndef PLANE_HPP
#define PLANE_HPP

#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Eigen/Core"
#include "geometry.hpp"
#include "model.hpp"

#define NO_VISIBLE 0
#define ALL_VISIBLE 1
#define CLIPPED 2

class Plane {
    public:
        Plane(void);
        ~Plane(void);

        void draw(void);
        void capture(int x, int y);
        void translate(int x, int y);
        void reset(void);
        int clip(Model& model);

    private:
      void homogeneous(Eigen::Vector4d& point);
      Eigen::Vector3d rayIntersection(Eigen::Vector3d& pStart,
                                    Eigen::Vector3d& pEnd,
                                    Eigen::Vector3d& normal,
                                    Eigen::Vector3d pointOnPlane);

        std::vector<Eigen::Vector4d> vertices;
        Eigen::Vector3d normal;
        float oldY;
        float delta;
        const float speed = 0.001;

        Geometry geometry;
};

#endif
