#include "module.hpp"
#include "glm.h"

#include <iostream>

Module::Module(void)
: rendermode('f')
, leftbuttonstate(false)
, rightbuttonstate(false)
, lightposition({1.0f, 3.0f, 3.0f, 1.0f})
, model(nullptr)
, planeflag(false)
, planeSelectFlag(false)
, drawnothing(false)
, drawclipped(false) {

    g_textureID = new GLuint[2];
    setModel(eBUNNY);
}

Module::~Module(void) {
    delete(g_textureID);
    delete(&bunny);
    delete(&screwdriver);
}

void Module::drawAxis(void) {
    glDisable(GL_LIGHTING);

    /* Z AXIS */
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();

    /* X AXIS */
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
    glEnd();

    /* Y AXIS */
    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}

/* This method initializes the textures */
void Module::setupTexture(void) {
    int width[2];
    int height[2];

    /* Load the textures from file */
    image[0] = glmReadPPM("checker.ppm", &width[0], &height[0]);
    image[1] = glmReadPPM("opengl.ppm", &width[1], &height[1]);

    glGenTextures(2, g_textureID);

    /* Map each texture to an ID */
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, g_textureID[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, image[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
}

GLuint *Module::getTextureID(void) {
    return g_textureID;
}

void Module::setRenderMode(GLuint mode) {
    model->setRenderMode(mode);
}

void Module::translatePlane(int x, int y) {
  if (planeflag) {
    plane.translate(x, y);
    clip();
  }
}

void Module::clip(void) {
  int rc;

  rc = plane.clip(*model);

  if (rc == CLIPPED) {
    drawclipped = true;
    drawnothing = false;
  } else if (rc == ALL_VISIBLE) {
    drawclipped = false;
    drawnothing = false;
  } else if (rc == NO_VISIBLE) {
    drawnothing = true;
    drawclipped = false;
  }
}

void Module::setPlane(void) {
  planeflag = !planeflag;
  if (planeflag) {
    clip();
  } else {
    drawclipped = false;
    drawnothing = false;
    planeSelectFlag = false;
  }
}

void Module::setModel(eModel emodel) {
    switch (emodel) {
        case eBUNNY:
            model = &bunny;
            if (!model->isLoaded()) {
                model->load();
                model->index();
                model->centre(-0.5, 0, -0.15, 0.5);
            }
            break;
        case eSCREWDRIVER:
            model = &screwdriver;
            if (!model->isLoaded()) {
                model->load();
                model->index();
                model->centre(0, 5, 0, 2);
            }
            break;
        default:
            break;
    }
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelUp(void) {
    model->rotate(-0.1, 0, 0);
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelDown(void) {
    model->rotate(0.1, 0, 0);
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelLeft(void) {
    model->rotate(0, -0.1, 0);
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelRight(void) {
    model->rotate(0, 0.1, 0);
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelClockwise(void) {
    model->rotate(0, 0, -0.1);
    if (planeflag) {
      clip();
    }
}

void Module::rotateModelCounterclockwise(void) {
    model->rotate(0, 0, 0.1);
    if (planeflag) {
      clip();
    }
}

void Module::draw(void) {
  if (planeflag) {
    plane.draw();
    if (drawclipped) {
      model->drawClipped();
    } else if (!drawnothing) {
      model->draw();
    }
  } else {
    model->draw();
  }
  drawAxis();
}

void Module::reset(void) {
    rendermode = 'f';
    camera.reset();
    plane.reset();
    model->reset();
}
