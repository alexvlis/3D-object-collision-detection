#include "cube.hpp"
#include <math.h>
#include <iostream>

Geometry Cube::geometry;

Cube::Cube(void)
: point_size(5)
, RENDER_MODE(GL_QUADS) {
    define();
}

Cube::~Cube(void) {
    /* Nothing to do */
}

void Cube::define(void) {
    vertices.resize(8);
    indices.resize(6);
    normals.resize(6);

    vertices[0] << 1, 1, 1, 1;
    vertices[1] << -1, 1, 1, 1;
    vertices[2] << -1, -1, 1, 1;
    vertices[3] << 1, -1, 1, 1;

    vertices[4] << 1, 1, -1, 1;
    vertices[5] << -1, 1, -1, 1;
    vertices[6] << -1, -1, -1, 1;
    vertices[7] << 1, -1, -1, 1;

    indices[0] << 0, 1, 2, 3;
    indices[1] << 4, 5, 6, 7;
    indices[2] << 0, 4, 7, 3;
    indices[3] << 1, 2, 6, 5;
    indices[4] << 0, 1, 5, 4;
    indices[5] << 3, 2, 6, 7;

    normals[0] << 0, 0, 1;
    normals[1] << 0, 0, -1;
    normals[2] << 0, 1, 0;
    normals[3] << 0, -1, 0;
    normals[4] << 1, 0, 0;
    normals[5] << -1, 0, 0;

    init();
}

void Cube::draw(void) {
    std::vector<Eigen::Vector4d>::iterator it;

    glColor4f(0.0f, 0.0f, 255.0f, 0.08f);

    int j = 0;
    for (it = indices.begin(); it != indices.end(); ++it) {
        glBegin(RENDER_MODE);
            glNormal3f(normals[j](0), normals[j](1), normals[j](2));
            for (int k = 0; k < (*it).size(); k++) {
                glVertex3f(vertices[(*it)(k)](0)/vertices[(*it)(k)](3),
                            vertices[(*it)(k)](1)/vertices[(*it)(k)](3),
                            vertices[(*it)(k)](2)/vertices[(*it)(k)](3));
            }
        glEnd();
        j++;
    }
}

/* Method to return the world space max of the box */
Eigen::Vector4d Cube::max(void) {
  std::vector<Eigen::Vector4d>::iterator it;
  Eigen::Vector4d max;

  max << 0, 0, 0, 1;
  for (it = vertices.begin(); it != vertices.end(); ++it) {
    if ((*it).x() > max.x()) {
      max(0) = (*it).x();
    }
    if ((*it).y() > max.y()) {
      max(1) = (*it).y();
    }
    if ((*it).z() > max.z()) {
      max(2) = (*it).z();
    }
  }
  return max;
}

/* Method to return the world space min of the box */
Eigen::Vector4d Cube::min(void) {
  std::vector<Eigen::Vector4d>::iterator it;
  Eigen::Vector4d min;

  min << 0, 0, 0, 1;
  for (it = vertices.begin(); it != vertices.end(); ++it) {
    if ((*it).x() < min.x()) {
      min(0) = (*it).x();
    }
    if ((*it).y() < min.y()) {
      min(1) = (*it).y();
    }
    if ((*it).z() < min.z()) {
      min(2) = (*it).z();
    }
  }
  return min;
}

void Cube::setRenderMode(GLuint mode) {
    RENDER_MODE = mode;
}

void Cube::reset(void) {
}

/*
 *  Implementation of Bounding Boxes
 */
ABB::ABB(void) {
    init();
}

ABB::~ABB(void) {

}

void ABB::init(void) {
    oldsize << 2, 2, 2, 0;
}

/* Compute axis aligned bounding box */
void ABB::adjust(Eigen::MatrixXd& mvertices) {
    ComputeMinMax(mvertices);

    vertices[0] << Pmax(0), Pmax(1), Pmax(2), 1;
    vertices[1] << Pmin(0), Pmax(1), Pmax(2), 1;
    vertices[2] << Pmin(0), Pmin(1), Pmax(2), 1;
    vertices[3] << Pmax(0), Pmin(1), Pmax(2), 1;

    vertices[4] << Pmax(0), Pmax(1), Pmin(2), 1;
    vertices[5] << Pmin(0), Pmax(1), Pmin(2), 1;
    vertices[6] << Pmin(0), Pmin(1), Pmin(2), 1;
    vertices[7] << Pmax(0), Pmin(1), Pmin(2), 1;
}

void ABB::ComputeMinMax(Eigen::MatrixXd& mvertices) {
    Pmax = mvertices.colwise().maxCoeff();
    Pmin = mvertices.colwise().minCoeff();
}

/* All these transformations are wrappers around the adjust method */
void ABB::rotate(Eigen::MatrixXd& mvertices,
                    float thetaX, float thetaY, float thetaZ) {
    adjust(mvertices);
}

void ABB::scale(Eigen::MatrixXd& mvertices,
                    float x, float y, float z) {
    adjust(mvertices);
}

void ABB::translate(Eigen::MatrixXd& mvertices,
                    float x, float y, float z) {
    adjust(mvertices);
}


/*
 *  OBB IMPLEMENTATION
 */
OBB::OBB(void) {
    Pmax << 1, 1, 1, 0;
    Pmin << -1, -1, -1, 0;
}

OBB::~OBB(void) {

}

void OBB::adjust(Eigen::MatrixXd& mvertices) {
    /* Do PCA analysis */
    pca(mvertices);

    vertices[0] << Pmax(0), Pmax(1), Pmax(2), 1;
    vertices[1] << Pmin(0), Pmax(1), Pmax(2), 1;
    vertices[2] << Pmin(0), Pmin(1), Pmax(2), 1;
    vertices[3] << Pmax(0), Pmin(1), Pmax(2), 1;

    vertices[4] << Pmax(0), Pmax(1), Pmin(2), 1;
    vertices[5] << Pmin(0), Pmax(1), Pmin(2), 1;
    vertices[6] << Pmin(0), Pmin(1), Pmin(2), 1;
    vertices[7] << Pmax(0), Pmin(1), Pmin(2), 1;

    /* Transform the cube using the eigenvector matrix */
    transform();
}

void OBB::transform(void) {
    std::vector<Eigen::Vector4d>::iterator it;

    for (it = vertices.begin(); it != vertices.end(); ++it) {
        *it = T * (*it);
    }
}

void OBB::rotate(Eigen::MatrixXd& mvertices,
                    float thetaX, float thetaY, float thetaZ) {
    std::vector<Eigen::Vector4d>::iterator it;
    Eigen::Matrix4d Rx;
    Eigen::Matrix4d Ry;
    Eigen::Matrix4d Rz;

    Rx = geometry.Rx(thetaX);
    Ry = geometry.Ry(thetaY);
    Rz = geometry.Rz(thetaZ);

    for (it = vertices.begin(); it != vertices.end(); ++it) {
        *it = Rz * Ry * Rx * (*it);
    }
}

void OBB::scale(Eigen::MatrixXd& mvertices, float x, float y, float z) {
    Eigen::Matrix4d S;
    std::vector<Eigen::Vector4d>::iterator it;

    S = geometry.S(x, y, z);
    for (it = vertices.begin(); it != vertices.end(); ++it) {
        *it = S * (*it);
    }
}

void OBB::translate(Eigen::MatrixXd& mvertices, float x, float y, float z) {
    Eigen::Matrix4d T;
    std::vector<Eigen::Vector4d>::iterator it;

    T = geometry.T(x, y, z);
    for (it = vertices.begin(); it != vertices.end(); ++it) {
        *it = T * (*it);
    }
}

void OBB::pca(Eigen::MatrixXd& mvertices) {
    Eigen::Vector4d mean;
    Eigen::Matrix3d C;
    float E;

    /* Compute the mean of each dimension */
    mean = mvertices.colwise().mean();

    /* Compute covariance matrix */
    for (int i = 0; i < C.rows(); i++) {
        for (int j = 0; j < C.cols(); j++) {
            E = (mvertices.col(i).cwiseProduct(mvertices.col(j))).mean();
            C(i, j) = E - (mean(i) * mean(j));
        }
    }
    /* Find the eigenvectors of the covariance matrix */
    Eigen::EigenSolver<Eigen::Matrix3d> es(C);

    /* Normalize eigenvectors */
    es.eigenvectors().colwise().normalize();

    /* Create the transformation matrix */
    T.block(0, 0, 3, 3) = es.eigenvectors().real();
    T.row(3) = Eigen::Vector4d::Zero();
    T.col(3) = Eigen::Vector4d::Zero();
    T(3, 3) = 1;

    /* Transform each point to the local frame */
    for (int i = 0; i < mvertices.rows(); i++) {
        mvertices.row(i) = T.inverse() * mvertices.row(i).transpose();
    }
    /* Find max min points of the box */
    Pmax = mvertices.colwise().maxCoeff().transpose();
    Pmin = mvertices.colwise().minCoeff().transpose();

    /* Go back to world-space coordinates */
    for (int i = 0; i < mvertices.rows(); i++) {
        mvertices.row(i) = T * mvertices.row(i).transpose();
    }
}
