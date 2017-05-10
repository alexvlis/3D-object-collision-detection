#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>

/* Non-system includes */
#include "Eigen/Core"
#include "objloader.h"
#include "cube.hpp"
#include "geometry.hpp"
#include "enums.hpp"

class Model {
    public:
        Model(char *filename);
        ~Model(void);

        void load(void);
        void centre(float x, float y, float z, float scale);
        void index(void);
        void draw(void);
        void drawClipped(void);
        void rotate(float thetaX, float thetaY, float thetaZ);
        void setRenderMode(GLuint mode);
        bool isLoaded(void) {return loaded;}
        void setBoundingBox(eBoundingBox eBox);
        void reset(void);
        void smooth(void);
        Cube& obb(void) {return Obb;}
        std::vector<Eigen::Vector3d>& getClipped(void) {return clipped;}
        Eigen::MatrixXd& getVertices(void) {return matrix;}
        std::vector<Eigen::Vector3d>& getClippedNormals(void) {return cnormals;}
        std::vector<Eigen::Vector3d>& getNormals(void) {return nnormals;}

    private:
        void findNormals(void);
        void computeVertexNormals(void);

        std::vector<Eigen::Vector3d> vertices;
        std::vector<Eigen::Vector3d> indices;
        Eigen::MatrixXd matrix;
        std::vector<Eigen::Vector3d> normals;
        std::vector<Eigen::Vector3d> nnormals;
        std::vector<Eigen::Vector3d> vnormals;
        std::vector<Eigen::Vector3d> clipped;
        std::vector<Eigen::Vector3d> cnormals;

        const GLfloat mat_amb[4] = {0.1, 0.8, 0.1, 1.0};
        const GLfloat mat_diff[4] = {0.1, 0.8, 0.1, 1.0};
        const GLfloat mat_spec[4] = {1.0, 1.0, 1.0, 1.0};
        const GLfloat shininess[1] = {100.0};

        static Geometry geometry; /* All models share this object */
        Cube* box;
        ABB Abb;
        OBB Obb;

        char *objfilename;
        GLuint RENDER_MODE;
        bool loaded;
        bool smoothflag;
};

#endif /* MODEL_HPP */
