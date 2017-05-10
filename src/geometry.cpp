#include "geometry.hpp"

#include <math.h>

using namespace std;

Geometry::Geometry(void) {
    t << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

    s << 1, 0, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1;

    rx << 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;

    ry << 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;

    rz << 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;

    transformation << 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1;
}

Geometry::~Geometry(void) {
    /* Nothing to do */
}

void Geometry::crossProduct(vector<GLfloat>& out, vector<GLfloat>& A, vector<GLfloat>& B) {
    out[0] = A[1] * B[2] - B[1] * A[2];
    out[1] = A[2] * B[0] - B[2] * A[0];
    out[2] = A[0] * B[1] - B[0] * A[1];
}

Eigen::Matrix4d& Geometry::T(float x, float y, float z) {
    t(0, 3) = x;
    t(1, 3) = y;
    t(2, 3) = z;

    return t;
}

Eigen::Matrix4d& Geometry::S(float x, float y, float z) {
    s(0, 0) = x;
    s(1, 1) = y;
    s(2, 2) = z;

    return s;
}

Eigen::Matrix4d& Geometry::Rx(float theta) {
    rx(1, 1) = cos(theta);
    rx(1, 2) = -sin(theta);
    rx(2, 1) = sin(theta);
    rx(2, 2) = cos(theta);

    return rx;
}

Eigen::Matrix4d& Geometry::Ry(float theta) {
    ry(0, 0) = cos(theta);
    ry(0, 2) = sin(theta);
    ry(2, 0) = -sin(theta);
    ry(2, 2) = cos(theta);

    return ry;
}

Eigen::Matrix4d& Geometry::Rz(float theta) {
    rz(0, 0) = cos(theta);
    rz(0, 1) = -sin(theta);
    rz(1, 0) = sin(theta);
    rz(1, 1) = cos(theta);

    return rz;
}

Eigen::Matrix4d& Geometry::transform(Eigen::Vector3d& r, Eigen::Vector3d& u,
                                        Eigen::Vector3d& f) {
    transformation.block(0, 0, 3, 1) = r;
    transformation.block(0, 1, 3, 1) = u;
    transformation.block(0, 2, 3, 1) = f;
    transformation.block(0, 3, 3, 1) = t;

    return transformation;
}

/* Method that divides by the homogeneous coordinate */
void Geometry::homogeneous(Eigen::Vector4d& point) {
  point(0) /= point(3);
  point(1) /= point(3);
  point(2) /= point(3);
  point(3) /= point(3);
}

Eigen::Vector3d Geometry::normal(Eigen::Vector3d p1,
                                  Eigen::Vector3d p2,
                                  Eigen::Vector3d p3) {
  Eigen::Vector3d v1;
  Eigen::Vector3d v2;

  v1 = p2 - p1;
  v2 = p3 - p1;
  return v2.cross(v1);
}
