#include "model.hpp"
#include "Eigen/Dense"
#include <iostream>

Geometry Model::geometry;

Model::Model(char *filename)
: RENDER_MODE(GL_TRIANGLES)
, box(nullptr)
, loaded(false)
, smoothflag(false) {
    objfilename = filename;
}

Model::~Model(void) {
    /* Nothing to do */
}

void Model::load(void) {
    /* Load the mesh of the bunny */
    loadOBJ(objfilename, vertices, indices);
    loaded = true;
}

void Model::centre(float x, float y, float z, float scale) {
    std::vector<Eigen::Vector4d>::iterator it;
    Eigen::Matrix4d T;
    Eigen::Matrix4d S;

    S = geometry.S(scale, scale, scale);
    T = geometry.T(x, y, z);

    for (int i = 0; i < matrix.rows(); i++) {
        matrix.row(i) = T * S * matrix.row(i).transpose();
    }
    /* Now we can compute the normals */
    findNormals();
}

/* Method that actually draws the model */
void Model::draw(void) {
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glColor3f(0.0f, 50.0f, 0.0f);

    if (RENDER_MODE != GL_TRIANGLES) {
      glDisable(GL_LIGHTING);
    }
    /* Draw the model */
    int j = 0;
    int i = 0;
    if (smoothflag) {
      for (i = 0; i < matrix.rows(); i++) {
        glBegin(RENDER_MODE);
          glNormal3f(vnormals[i](0), vnormals[i](1), vnormals[i](2));
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));
          i++;
          glNormal3f(vnormals[i](0), vnormals[i](1), vnormals[i](2));
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));
          i++;
          glNormal3f(vnormals[i](0), vnormals[i](1), vnormals[i](2));
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));

          j++;
        glEnd();
      }
    } else {
      for (i = 0; i < matrix.rows(); i++) {
        glBegin(RENDER_MODE);
          glNormal3f(nnormals[j](0), nnormals[j](1), nnormals[j](2));
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));
          i++;
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));
          i++;
          glVertex3f(matrix(i, 0)/matrix(i, 3), matrix(i, 1)/matrix(i, 3), matrix(i, 2)/matrix(i, 3));

          j++;
          glEnd();
        }
      }
    if (RENDER_MODE != GL_TRIANGLES) {
      glEnable(GL_LIGHTING);
    }
    /* Draw the bounding box */
    if (box) {
        box->draw();
    }
}

/* Draw the clipped model */
void Model::drawClipped(void) {
  std::vector<Eigen::Vector3d>::iterator it;

  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
  glColor3f(0.0f, 50.0f, 0.0f);

  /* The should not be light for any other render mode */
  if (RENDER_MODE != GL_TRIANGLES) {
    glDisable(GL_LIGHTING);
  }
  int j = 0;
  for (it = clipped.begin(); it != clipped.end(); std::advance(it, 3)) {
    glBegin(RENDER_MODE);
      glNormal3f(cnormals[j].x(), cnormals[j].y(), cnormals[j].z());
      glVertex3f((*it).x(), (*it).y(), (*it).z());
      glVertex3f((*std::next(it, 1)).x(), (*std::next(it, 1)).y(), (*std::next(it, 1)).z());
      glVertex3f((*std::next(it, 2)).x(), (*std::next(it, 2)).y(), (*std::next(it, 2)).z());
    glEnd();
    j++;
  }
  if (RENDER_MODE != GL_TRIANGLES) {
    glEnable(GL_LIGHTING);
  }
}

/* Method that computes the normal of each face */
void Model::findNormals(void) {
    std::vector<Eigen::Vector3d>::iterator it;
    Eigen::Vector3d normal;
    Eigen::Vector4d v1;
    Eigen::Vector4d v2;
    Eigen::Vector3d tmp1;
    Eigen::Vector3d tmp2;

    normals.clear();
    for (int i = 0; i < matrix.rows(); i += 3) {
        v1 = matrix.row(i+1) - matrix.row(i);
        v2 = matrix.row(i+2) - matrix.row(i);

        /* Store in 3D vector in order to do cross */
        tmp1 << v1.x(), v1.y(), v1.z();
        tmp2 << v2.x(), v2.y(), v2.z();

        normal = tmp2.cross(tmp1);
        normals.push_back(normal);
        nnormals.push_back(normal.normalized());
    }
}

void Model::computeVertexNormals(void) {
  int j;
  int instance;
  vnormals.resize(matrix.rows());
  for (int i = 0; i < matrix.rows(); i++) {
    instance = 0;
    j = 0;
    /* Find the neighbours */
    while(instance < 5 && j < matrix.rows()) {
      if (matrix.row(i) == matrix.row(j)) {
        vnormals[i] += normals[ceil(j/3)];
        instance++;
      }
      j++;
    }
    vnormals[i].normalize();
  }
}

void Model::smooth(void) {
  computeVertexNormals();
  smoothflag = true;
}

/* Method that does the indexing of the model */
void Model::index(void) {
    Eigen::Vector4d tmp1;
    Eigen::Vector4d tmp2;
    Eigen::Vector4d tmp3;
    std::vector<Eigen::Vector3d>::iterator it;

    matrix.resize(indices.size() * 3, 4);

    int i = matrix.rows();
    for (it = indices.begin(); it != indices.end(); ++it) {
        tmp1 << vertices[(*it)(0)-1](0),
                vertices[(*it)(0)-1](1),
                vertices[(*it)(0)-1](2),
                1;

        tmp2 << vertices[(*it)(1)-1](0),
                vertices[(*it)(1)-1](1),
                vertices[(*it)(1)-1](2),
                1;

        tmp3 << vertices[(*it)(2)-1](0),
                vertices[(*it)(2)-1](1),
                vertices[(*it)(2)-1](2),
                1;

        matrix.row(--i) = tmp1;
        matrix.row(--i) = tmp2;
        matrix.row(--i) = tmp3;
    }
}

void Model::rotate(float thetaX, float thetaY, float thetaZ) {
    std::vector<Eigen::Vector4d>::iterator it;
    Eigen::Matrix4d Rx;
    Eigen::Matrix4d Ry;
    Eigen::Matrix4d Rz;

    Rx = geometry.Rx(thetaX);
    Ry = geometry.Ry(thetaY);
    Rz = geometry.Rz(thetaZ);

    int j = 0;
    if (smoothflag) {
      for (int i = 0; i < matrix.rows(); i++) {
          vnormals[i] = (Rz * Ry * Rx).block(0, 0, 3, 3) * vnormals[i];
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();
          i++;
          vnormals[i] = (Rz * Ry * Rx).block(0, 0, 3, 3) * vnormals[i];
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();
          i++;
          vnormals[i] = (Rz * Ry * Rx).block(0, 0, 3, 3) * vnormals[i];
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();
        }
    } else {
      for (int i = 0; i < matrix.rows(); i++) {
          nnormals[j] = (Rz * Ry * Rx).block(0, 0, 3, 3) * nnormals[j];
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();
          i++;
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();
          i++;
          matrix.row(i) = Rz * Ry * Rx * matrix.row(i).transpose();

          j++;
        }
      }
    /* Adjust the bounding box to the rotated model */
    if (box) {
        box->rotate(matrix, thetaX, thetaY, thetaZ);
    }
}

void Model::setBoundingBox(eBoundingBox eBox) {
    switch (eBox) {
        case eABB:
            box = &Abb;
            break;
        case eOBB:
            box = &Obb;
            break;
        default:
            break;
    }
    box->adjust(matrix);
}

void Model::setRenderMode(GLuint mode) {
    RENDER_MODE = mode;
}

void Model::reset(void) {

}
