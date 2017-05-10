#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <vector>

#include "geometry.hpp"
#include "Eigen/Core"
#include "Eigen/Dense"


class Cube {
    public:
        Cube();
        ~Cube();

        void draw(void);
        void reset(void);
        void setTextureID(const GLuint texture) {textureID = texture;}
        void setRenderMode(GLuint mode);
        Eigen::Vector4d min(void);
        Eigen::Vector4d max(void);
        virtual void adjust(Eigen::MatrixXd& mvertices) =0;
        virtual void scale(Eigen::MatrixXd& mvertices,
                           float x, float y, float z) =0;
        virtual void translate(Eigen::MatrixXd& mvertices,
                               float x, float y, float z) =0;
        virtual void rotate(Eigen::MatrixXd& mvertices,
                            float thetaX, float thetaY, float thetaZ) =0;
    protected:
        void define(void);
        virtual void init(void) {}

        static Geometry geometry;
        std::vector<Eigen::Vector4d> vertices;
        std::vector<Eigen::Vector3d> normals;
        std::vector<Eigen::Vector4d> indices;
        Eigen::Vector4d Pmax;
        Eigen::Vector4d Pmin;

        GLuint RENDER_MODE;
        GLuint textureID; /* ID to map texture of cube */

        const int point_size;
};

class ABB: public Cube {
    public:
        ABB();
        ~ABB();

        void adjust(Eigen::MatrixXd& mvertices);
        void scale(Eigen::MatrixXd& mvertices, float x, float y, float z);
        void translate(Eigen::MatrixXd& mvertices, float x, float y, float z);
        void rotate(Eigen::MatrixXd& mvertices,
                     float thetaX, float thetaY, float thetaZ);
    private:
        void ComputeMinMax(Eigen::MatrixXd& mvertices);
        void init(void);

        Eigen::Vector4d size;
        Eigen::Vector4d oldsize;
};

class OBB: public Cube {
    public:
        OBB();
        ~OBB();

        void adjust(Eigen::MatrixXd& vertices);
        void scale(Eigen::MatrixXd& mvertices, float x, float y, float z);
        void translate(Eigen::MatrixXd& mvertices, float x, float y, float z);
        void rotate(Eigen::MatrixXd& mvertices,
                     float thetaX, float thetaY, float thetaZ);
    private:
        void pca(Eigen::MatrixXd& mvertices);
        void transform(void);

        Eigen::Matrix4d T;
};

#endif /* CUBE_HPP */
