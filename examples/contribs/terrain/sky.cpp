// TP OpenGL: Joerg Liebelt, Serigne Sow
#include "sky.h"
#include <QGLWidget>

bool SKY::LoadTexture(int side, const QString &filename) {
  QImage image;
  if (image.load(filename)) {
    textures[side] = QGLWidget::convertToGLFormat(image);

    // construire les textures openGL
    glGenTextures(1, &(texIDs[side]));
    glBindTexture(GL_TEXTURE_2D, (texIDs[side]));
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // les deux commandes suivantes servent a elargir la texture pq. la surface
    // soit remplie;
    //.. clamp_to_edge = repeter la derniere ligne de la texture (marche bien
    //ici)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[side].width(),
                 textures[side].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 textures[side].bits());
  } else {
    qWarning("Sky texture loading failed for side %d", side);
    return false;
  }
  return true;
}

void SKY::Render() {
  // on ne veut pas que le background disparaisse (POURTANT, IL DISPARAIT!! J'y
  // arrive pas...)  glDisable( GL_CULL_FACE );  glDisable( GL_DEPTH_TEST );
  // glDepthMask( GL_FALSE );
  glDisable(GL_LIGHTING);

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  // on construit a partir de la position de la camera (ou du milieu...)
  glTranslatef(vecCenter[0], vecCenter[1], vecCenter[2]);

  // face
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_FRONT]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMax[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMax[2]);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMax[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMax[2]);
  glEnd();

  // derriere
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_BACK]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMin[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMin[2]);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMin[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMin[2]);
  glEnd();

  // droite
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_RIGHT]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMin[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMin[2]);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMax[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMax[2]);
  glEnd();

  // gauche
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_LEFT]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMax[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMax[2]);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMin[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMin[2]);
  glEnd();

  // au-dessus
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_TOP]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMax[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMin[0], vecMax[1], vecMin[2]);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMin[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMax[0], vecMax[1], vecMax[2]);
  glEnd();

  // au-dessous
  glBindTexture(GL_TEXTURE_2D, texIDs[SKY_BOTTOM]);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMin[2]);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(vecMin[0], vecMin[1], vecMax[2]);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMax[2]);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(vecMax[0], vecMin[1], vecMin[2]);
  glEnd();
  glPopMatrix();
  // glDepthMask( GL_TRUE );
  // glEnable( GL_DEPTH_TEST );
  // glEnable( GL_CULL_FACE );
}
