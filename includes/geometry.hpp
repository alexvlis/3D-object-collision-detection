#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header

#include "Eigen/Dense"

class Geometry {
    public:
        Geometry(void);
        ~Geometry(void);

        void homogeneous(Eigen::Vector4d& point);
        Eigen::Matrix4d& T(float x, float y, float z);
        Eigen::Matrix4d& S(float x, float y, float z);
        Eigen::Matrix4d& Rx(float theta);
        Eigen::Matrix4d& Ry(float theta);
        Eigen::Matrix4d& Rz(float theta);
        Eigen::Vector3d normal(Eigen::Vector3d p1,
                                Eigen::Vector3d p2,
                                Eigen::Vector3d p3);

        Eigen::Matrix4d& transform(Eigen::Vector3d& r,
                                    Eigen::Vector3d& u,
                                    Eigen::Vector3d& f);

        std::vector<std::vector<float> >* multiply(
                                const std::vector<std::vector<float> >& A,
                                const std::vector<std::vector<float> >& B);

        void crossProduct(std::vector<GLfloat>& out,
                            std::vector<GLfloat>& A,
                            std::vector<GLfloat>& B);

    private:
        Eigen::Matrix4d t;
        Eigen::Matrix4d s;
        Eigen::Matrix4d rx;
        Eigen::Matrix4d ry;
        Eigen::Matrix4d rz;
        Eigen::Matrix4d transformation;
};

#endif
