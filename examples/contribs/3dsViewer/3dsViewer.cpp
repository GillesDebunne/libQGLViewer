#include "3dsViewer.h"

#include <QKeyEvent>
#include <lib3ds/camera.h>
#include <lib3ds/light.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/mesh.h>
#include <lib3ds/vector.h>
#include <math.h>
#include <qfiledialog.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace qglviewer;

void Viewer::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
  case Qt::Key_L:
    loadFile();
    break;
  default:
    QGLViewer::keyPressEvent(e);
  }
}

QString Viewer::helpString() const {
  QString text("<h2>3 d s V i e w e r</h2>");
  text +=
      "This example uses the lib3ds library to load a 3ds object file.<br><br>";
  text += "Press <b>L</b>(oad) to open a 3ds file.<br><br>";
  text += "Note that certain 3ds files contain animated sequences that can ";
  text += "be played using the <b>Return</b> (animate) key.";
  return text;
}

void Viewer::loadFile() {
#if QT_VERSION < 0x040000
  QString name = QFileDialog::getOpenFileName(
      ".", "3DS files (*.3ds *.3DS);;All files (*)", this, "Choose",
      "Select a 3ds model");
#else
  QString name =
      QFileDialog::getOpenFileName(this, "Select a 3ds model", ".",
                                   "3DS files (*.3ds *.3DS);;All files (*)");
#endif

  // In case of Cancel
  if (name.isEmpty())
    return;

#if QT_VERSION < 0x040000
  file = lib3ds_file_load(name.latin1());
#else
  file = lib3ds_file_load(name.toLatin1().constData());
#endif
  if (!file) {
    qWarning("Error : Unable to open file ");
    exit(1);
  }

  if (file->cameras)
    camera_name = file->cameras->name;
  else
    camera_name = NULL;

  lib3ds_file_eval(file, 0);

  initScene();

  float min[3], max[3];
  lib3ds_file_bounding_box_of_objects(file, true, true, true, min, max);
  // lib3ds_file_bounding_box(file, min, max);
  setSceneBoundingBox(Vec(min), Vec(max));

  if (!file->cameras)
    camera()->showEntireScene();
  else
    updateGL();
  stopAnimation();
}

void Viewer::init() {
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  setKeyDescription(Qt::Key_L, "Loads a new 3ds file");

  restoreStateFromFile();
  help();
  loadFile();
}

void Viewer::initScene() {
  if (!file)
    return;

  // Lights
  GLfloat amb[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat dif[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat spe[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat pos[] = {0.0, 0.0, 0.0, 1.0};
  int li = GL_LIGHT0;
  for (Lib3dsLight *l = file->lights; l; l = l->next) {
    glEnable(li);

    glLightfv(li, GL_AMBIENT, amb);
    glLightfv(li, GL_DIFFUSE, dif);
    glLightfv(li, GL_SPECULAR, spe);

    pos[0] = l->position[0];
    pos[1] = l->position[1];
    pos[2] = l->position[2];
    glLightfv(li, GL_POSITION, pos);

    if (!l->spot_light)
      continue;

    pos[0] = l->spot[0] - l->position[0];
    pos[1] = l->spot[1] - l->position[1];
    pos[2] = l->spot[2] - l->position[2];
    glLightfv(li, GL_SPOT_DIRECTION, pos);
    ++li;
  }

  // Camera
  Lib3dsNode *c =
      lib3ds_file_node_by_name(file, camera_name, LIB3DS_CAMERA_NODE);
  Lib3dsNode *t =
      lib3ds_file_node_by_name(file, camera_name, LIB3DS_TARGET_NODE);
  if (!c || !t)
    return;

  // Lib3dsMatrix M;
  // lib3ds_matrix_camera(M, c->data.camera.pos, t->data.target.pos,
  // c->data.camera.roll);

  // cout << "Pos = " << Vec(c->data.camera.pos) << endl;
  // cout << "Tar = " << Vec(t->data.target.pos) << endl;
  // cout << "Rol = " << c->data.camera.roll << endl;

  camera()->setPosition(Vec(c->data.camera.pos));
  camera()->lookAt(Vec(t->data.target.pos));
  Vec up = camera()->frame()->transformOf(Vec(0.0, 0.0, 1.0));
  float angle = atan2(up.x, up.y);
  Quaternion q(Vec(0.0, 0.0, 1.0), c->data.camera.roll - angle);
  camera()->frame()->rotate(q);
  camera()->setFieldOfView(M_PI / 180.0 * c->data.camera.fov);
}

void Viewer::renderNode(Lib3dsNode *node) {
  for (Lib3dsNode *p = node->childs; p != 0; p = p->next)
    renderNode(p);

  if (node->type == LIB3DS_OBJECT_NODE) {
    if (strcmp(node->name, "$$$DUMMY") == 0)
      return;

    if (!node->user.d) {
      Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(file, node->name);
      if (!mesh)
        return;

      node->user.d = glGenLists(1);
      glNewList(node->user.d, GL_COMPILE);

      Lib3dsVector *normalL = new Lib3dsVector[3 * mesh->faces];

      Lib3dsMatrix M;
      lib3ds_matrix_copy(M, mesh->matrix);
      lib3ds_matrix_inv(M);
      glMultMatrixf(&M[0][0]);

      lib3ds_mesh_calculate_normals(mesh, normalL);

      for (unsigned int p = 0; p < mesh->faces; ++p) {
        Lib3dsFace *f = &mesh->faceL[p];
        Lib3dsMaterial *mat = 0;
        if (f->material[0])
          mat = lib3ds_file_material_by_name(file, f->material);

        if (mat) {
          static GLfloat a[4] = {0, 0, 0, 1};
          float s;
          glMaterialfv(GL_FRONT, GL_AMBIENT, a);
          glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
          glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
          s = pow(2, 10.0 * mat->shininess);
          if (s > 128.0)
            s = 128.0;
          glMaterialf(GL_FRONT, GL_SHININESS, s);
        } else {
          Lib3dsRgba a = {0.2, 0.2, 0.2, 1.0};
          Lib3dsRgba d = {0.8, 0.8, 0.8, 1.0};
          Lib3dsRgba s = {0.0, 0.0, 0.0, 1.0};
          glMaterialfv(GL_FRONT, GL_AMBIENT, a);
          glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
          glMaterialfv(GL_FRONT, GL_SPECULAR, s);
        }

        glBegin(GL_TRIANGLES);
        glNormal3fv(f->normal);
        for (int i = 0; i < 3; ++i) {
          glNormal3fv(normalL[3 * p + i]);
          glVertex3fv(mesh->pointL[f->points[i]].pos);
        }
        glEnd();
      }

      delete[] normalL;

      glEndList();
    }

    if (node->user.d) {
      glPushMatrix();
      Lib3dsObjectData *d = &node->data.object;
      glMultMatrixf(&node->matrix[0][0]);
      glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);
      glCallList(node->user.d);
      glPopMatrix();
    }
  }
}

void Viewer::draw() {
  if (!file)
    return;

  for (Lib3dsNode *p = file->nodes; p != 0; p = p->next)
    renderNode(p);
}

void Viewer::animate() {
  current_frame++;
  if (current_frame > file->frames)
    current_frame = 0;
  lib3ds_file_eval(file, current_frame);
  initScene();
}
