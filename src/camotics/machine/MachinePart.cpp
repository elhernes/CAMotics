/******************************************************************************\

  CAMotics is an Open-Source simulation and CAM software.
  Copyright (C) 2011-2019 Joseph Coffland <joseph@cauldrondevelopment.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#include "MachinePart.h"

#ifdef CAMOTICS_GUI
#include <camotics/view/GL.h>
#endif

using namespace CAMotics;
using namespace cb;
using namespace std;


MachinePart::MachinePart(const string &name,
                         SmartPointer<JSON::Value> &config) :
  name(name), vbuf(0) {
  if (config->hasList("color")) color.read(config->getList("color"));
  if (config->hasList("init")) init.read(config->getList("init"));
  if (config->hasList("home")) home.read(config->getList("home"));
  if (config->hasList("min")) min.read(config->getList("min"));
  if (config->hasList("max")) max.read(config->getList("max"));
  if (config->hasList("movement")) movement.read(config->getList("movement"));

  // Offset to home
  offset = home - init;
}


void MachinePart::setPosition(const Vector3D &position) {
  this->position = position * movement;
}


void MachinePart::read(const InputSource &source,
                       const Matrix4x4D &transform, bool reverseWinding) {
  vector<Vector2U> lines;
  vector<Vector3U> triangles;
  vector<Vector3F> vertices;

  while (source.getStream().good()) {
    string line = String::trim(source.getLine());
    if (line.empty()) break;

    if (line[0] == 't' || line[0] == 'l' || line[0] == 'v') {
      size_t equal = line.find('=');
      if (equal == string::npos) continue;

      int start = 1;
      while (line[start] == '0') start++;
      unsigned n = 0;
      if (start != (int)equal) n = String::parseU32(line.substr(start));

      vector<string> nums;
      String::tokenize(line.substr(equal + 1), nums, ",");

      if (line[0] == 't' && nums.size() == 3)
        triangles.push_back
          (Vector3U(String::parseU32(nums[0]),
                    String::parseU32(nums[reverseWinding ? 2 : 1]),
                    String::parseU32(nums[reverseWinding ? 1 : 2])));

      else if (line[0] == 'l' && nums.size() == 2)
        lines.push_back(Vector2U(String::parseU32(nums[0]),
                                 String::parseU32(nums[1])));

      else if (line[0] == 'v' && nums.size() == 3) {
        if (vertices.size() < n + 1) vertices.resize((n + 1) * 1.5);
         Vector4D v(String::parseDouble(nums[0]),
                    String::parseDouble(nums[1]),
                    String::parseDouble(nums[2]), 1);

         v = transform * v;

         vertices[n] = v.slice<3>();
      }
    }
  }

  // Assemble lines
  for (unsigned i = 0; i < lines.size(); i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 3; k++)
        this->lines.push_back(vertices[lines[i][j]][k]);

  // Assemble triangles
  for (unsigned i = 0; i < triangles.size(); i++) {
    Vector3F v[3];
    for (int j = 0; j < 3; j++) v[j] = vertices[triangles[i][j]];
    add(v);
  }
}


#ifdef CAMOTICS_GUI
void MachinePart::drawLines() {
#if 0 // TODO GL
  GLFuncs &glFuncs = getGLFuncs();

  if (!vbuf) {
    glFuncs.glGenBuffers(1, &vbuf);

    // Vertices
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, vbuf);
    glFuncs.glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(float),
                         &lines[0], GL_STATIC_DRAW);
  }

  glFuncs.glBindBuffer(GL_ARRAY_BUFFER, vbuf);
  glFuncs.glVertexPointer(3, GL_FLOAT, 0, 0);

  glFuncs.glBindBuffer(GL_ARRAY_BUFFER, 0);

  glFuncs.glEnableClientState(GL_VERTEX_ARRAY);
  GLboolean light;
  glFuncs.glGetBooleanv(GL_LIGHTING, &light);
  glFuncs.glDisable(GL_LIGHTING);

  glFuncs.glDrawArrays(GL_LINES, 0, lines.size() / 3);

  if (light) glFuncs.glEnable(GL_LIGHTING);
  glFuncs.glDisableClientState(GL_VERTEX_ARRAY);
#endif
}


void MachinePart::draw(bool wire) {
#if 0 // TODO GL
  GLFuncs &glFuncs = getGLFuncs();

  glFuncs.glPushMatrix();
  glFuncs.glTranslatef(offset[0], offset[1], offset[2]);
  glFuncs.glTranslatef(position[0], position[1], position[2]);

  if (wire) glFuncs.glColor3ub(color[0], color[1], color[2]);
  else glFuncs.glColor3ub(color[0] * 0.8, color[1] * 0.8, color[2] * 0.8);
  drawLines();

  if (!wire) {
    glFuncs.glColor3ub(color[0], color[1], color[2]);
    TriangleSurface::draw();
  }

  glFuncs.glPopMatrix();
#endif
}
#endif // CAMOTICS_GUI
