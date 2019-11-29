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

#include "GLShader.h"

#include <cbang/Exception.h>
#include <cbang/Catch.h>
#include <cbang/util/Resource.h>

#include <vector>

using namespace CAMotics;
using namespace cb;
using namespace std;

namespace CAMotics {
  extern const DirectoryResource resource0;
}


#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

GLShader::GLShader(const string &source, int type) {
  GLContext gl;
  shader = gl.glCreateShader((GLenum)type);
  if (!shader) THROW("Failed to create GL shader");


  // Load source
  std::string src;

  // prepend precision def for iOS
#ifdef __APPLE__
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
  src += "precision highp float;\n";
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#endif
#endif // __APPLE__

  src += source;
  const GLchar *str = (const GLchar *)src.c_str();
  gl.glShaderSource(shader, 1, &str, 0);

  // Compile
  gl.glCompileShader(shader);

  GLint success = 0;
  gl.glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE) {
    GLint len = 0;
    gl.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    // The len includes the NULL character
    vector<GLchar> error(len);
    gl.glGetShaderInfoLog(shader, len, &len, &error[0]);

    THROW("Failed to compile GL shader: " << &error[0]);
  }
}


GLShader::~GLShader() {
  try {
    if (shader && GLContext::isActive()) GLContext().glDeleteShader(shader);
  } CATCH_ERROR;
}


GLShader GLShader::loadResource(const string &path, int type) {
  const Resource *r = resource0.find(path);
  if (!r) THROW("Could not find GL shader resource '" << path << "'");
  return GLShader(r->toString(), type);
}
