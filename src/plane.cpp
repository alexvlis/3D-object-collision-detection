#include "plane.hpp"
#include <math.h>

Plane::Plane(void) {
    vertices.resize(4);

    vertices[0] << 1.5, 1.5, 0, 1;
    vertices[1] << 1.5, -1.5, 0, 1;
    vertices[2] << -1.5, -1.5, 0, 1;
    vertices[3] << -1.5, 1.5, 0, 1;

    normal << 0, 0, 1;
}

Plane::~Plane(void) {

}

void Plane::draw(void) {
    glShadeModel(GL_SMOOTH);
    glColor4f(255.0f, 0.0f, 0.0f, 0.85f);
    glBegin(GL_QUADS);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(vertices[0].x(), vertices[0].y(), vertices[0].z());
        glVertex3f(vertices[1].x(), vertices[1].y(), vertices[1].z());
        glVertex3f(vertices[2].x(), vertices[2].y(), vertices[2].z());
        glVertex3f(vertices[3].x(), vertices[3].y(), vertices[3].z());
    glEnd();
}

void Plane::capture(int x, int y) {
  oldY = y;
}

void Plane::translate(int x, int y) {
  Eigen::Matrix4d T;
  std::vector<Eigen::Vector4d>::iterator it;

  delta = ((y - oldY) * speed);
  oldY = y;

  T = geometry.T(0, 0, delta);

  for (it = vertices.begin(); it != vertices.end(); ++it) {
    *it = T * (*it);
  }
}

void Plane::reset(void) {
  vertices[0] << 1.5, 1.5, 0, 1;
  vertices[1] << 1.5, -1.5, 0, 1;
  vertices[2] << -1.5, -1.5, 0, 1;
  vertices[3] << -1.5, 1.5, 0, 1;
}

int Plane::clip(Model& model) {
  if (model.obb().min().z() > vertices[0].z()) {
    /* Draw everything */
    return ALL_VISIBLE;
  } else if (model.obb().max().z() < vertices[0].z()) {
    /* Draw nothing */
    return NO_VISIBLE;
  } else {
    /* Copy the visible vertices */
    double theta;
    int invisible;
    double angle1, angle2, angle3;
    bool p1in, p2in, p3in;
    Eigen::Vector3d pA, pB, pC;
    Eigen::Vector3d px0, px1;
    Eigen::MatrixXd& mvertices = model.getVertices();
    std::vector<Eigen::Vector3d>& clipped = model.getClipped();
    std::vector<Eigen::Vector3d>& cnormals = model.getClippedNormals();
    std::vector<Eigen::Vector3d>& normals = model.getNormals();

    clipped.clear();
    cnormals.clear();
    normal.normalize();
    geometry.homogeneous(vertices[0]);
    theta = normal.dot(vertices[0].segment(0, 3));

    int j = 0;
    for (int i = 0; i < mvertices.rows(); i += 3) {
      angle1 = normal.dot(mvertices.row(i).segment(0, 3));
      angle2 = normal.dot(mvertices.row(i+1).segment(0, 3));
      angle3 = normal.dot(mvertices.row(i+2).segment(0, 3));
      invisible = 0;
      p1in = false;
      p2in = false;
      p3in = false;

      if (theta > angle1) {
        invisible++;
        p1in = true;
      }
      if (theta > angle2) {
        invisible++;
        p2in = true;
      }
      if (theta > angle3) {
        invisible++;
        p3in = true;
      }
      /* Determine how many vertices are on the negative side of the plane */
      if (invisible == 0) {
        /* No vertex intesects the plane */
        clipped.push_back(mvertices.row(i).segment(0, 3).transpose());
        clipped.push_back(mvertices.row(i+1).segment(0, 3));
        clipped.push_back(mvertices.row(i+2).segment(0, 3));
        cnormals.push_back(normals[j]);
      } else if (invisible == 3) {
          /* Everything is outside hence do nothing */
      } else if (invisible == 1) {
        /* Decide which vertex is invisible */
        if (p1in) {
          pA = mvertices.row(i).segment(0, 3);
          pB = mvertices.row(i+1).segment(0, 3);
          pC = mvertices.row(i+2).segment(0, 3);
        } else if (p2in) {
          pA = mvertices.row(i+1).segment(0, 3);
          pB = mvertices.row(i).segment(0, 3);
          pC = mvertices.row(i+2).segment(0, 3);
        } else if (p3in) {
          pA = mvertices.row(i+2).segment(0, 3);
          pB = mvertices.row(i+1).segment(0, 3);
          pC = mvertices.row(i).segment(0, 3);
        }
        /* Find intersection points */
        px0 = rayIntersection(pA, pB, normal, vertices[0].segment(0, 3));
        px1 = rayIntersection(pA, pC, normal, vertices[0].segment(0, 3));

        /* Add triangle */
        clipped.push_back(pB);
        clipped.push_back(px1);
        clipped.push_back(pC);
        cnormals.push_back(normals[j]);

        /* Add new triangle */
        clipped.push_back(pB);
        clipped.push_back(px0);
        clipped.push_back(px1);
        cnormals.push_back(normals[j]);
      } else if (invisible == 2) {
        /* Work out which vertex is visible */
        if (!p1in) {
          pA = mvertices.row(i+1).segment(0, 3);
          pB = mvertices.row(i+2).segment(0, 3);
          pC = mvertices.row(i).segment(0, 3);
        } else if (!p2in) {
          pA = mvertices.row(i).segment(0, 3);
          pB = mvertices.row(i+2).segment(0, 3);
          pC = mvertices.row(i+1).segment(0, 3);
        } else if (!p3in) {
          pA = mvertices.row(i).segment(0, 3);
          pB = mvertices.row(i+1).segment(0, 3);
          pC = mvertices.row(i+2).segment(0, 3);
        }
        /* Find the intersection points */
        px0 = rayIntersection(pC, pB, normal, vertices[0].segment(0, 3));
        px1 = rayIntersection(pC, pA, normal, vertices[0].segment(0, 3));

        /* Add the triangle */
        clipped.push_back(pC);
        clipped.push_back(px0);
        clipped.push_back(px1);
        cnormals.push_back(normals[j]);
      }
      j++;
    }
    return CLIPPED;
  }
}

Eigen::Vector3d Plane::rayIntersection(Eigen::Vector3d& pStart,
                                Eigen::Vector3d& pEnd,
                                Eigen::Vector3d& normal,
                                Eigen::Vector3d pointOnPlane) {
  Eigen::Vector3d point;
  Eigen::Vector3d ray;
  Eigen::Vector3d nray;
  double length;
  double cosAngle;
  double delta;

  ray = pEnd - pStart;
  nray = ray;
  nray.normalize();

  cosAngle = normal.dot(nray); /* cosine of angle between ray and normal */
  if (fabs(cosAngle) < 0.000000000001) {
    /* The ray is parallel to the plane */
    length = ray.norm();
  } else {
    delta = normal.dot(pointOnPlane) - normal.dot(pStart);
    length = delta/cosAngle;
  }
  point = nray * length;
  point += pStart;

  return point;
}
