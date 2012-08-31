// by dff180

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <SOIL/SOIL.h>
#include "shader_utils.h"
#include "MyGLWindow.h"

MyGLWindow::MyGLWindow(EGLconfig *_config, std::string fragmentShader, std::string textureName) 
: EGLWindow(_config)
, _fragmentShader(fragmentShader)
, _textureName(textureName)
, _texture0(0)
{
  initializeGL();
  srand(time(NULL));
}

MyGLWindow::~MyGLWindow()
{
// TODO: Cleanup allocated resources here
}

int MyGLWindow::init_resources()
{
  GLfloat triangle_vertices[] = {
    -1.0, -1.0,
     1.0, -1.0,
    -1.0,  1.0,
     1.0, -1.0,
     1.0,  1.0,
    -1.0,  1.0
  };

  glGenBuffers(1, &_vbo_quad);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo_quad);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("shaders/triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if (_fragmentShader != "")
  {
    if ((fs = create_shader(_fragmentShader.c_str(), GL_FRAGMENT_SHADER)) == 0) return 0;
  }
  else
    if ((fs = create_shader("shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  _program = glCreateProgram();
  glAttachShader(_program, vs);
  glAttachShader(_program, fs);
  glLinkProgram(_program);
  glGetProgramiv(_program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(_program);
    return 0;
  }

  const char* attribute_name = "coord2d";
  _attribute_coord2d = glGetAttribLocation(_program, attribute_name);
  if (_attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  return 1;

}


void MyGLWindow::initializeGL()
{
  init_resources();

  // load texture if specified
  if (_textureName != "")
  {
    _texture0 = SOIL_load_OGL_texture
      (
      _textureName.c_str(),
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS 
      );
  
    /* check for an error during the load process */
    if( 0 == _texture0 )
    {
      printf( "SOIL loading error: '%s' '%s'\n", SOIL_last_result(), _textureName.c_str() );
    }
    else
    {
      glEnable(GL_TEXTURE_2D);
    }    
  }

  // Enable alpha blend
 //   glEnable(GL_BLEND);
 //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // set the clear colour
  glClearColor(1,1,1,1);

  // Start timer
  gettimeofday(&_startTime, NULL);
}

float MyGLWindow::getDeltaTimeS()
{
  timeval currentTime;
  gettimeofday(&currentTime, NULL);

  float deltaTime = (currentTime.tv_sec - _startTime.tv_sec); 
  deltaTime += (currentTime.tv_usec - _startTime.tv_usec) / 1000000.0; // us to s
  return deltaTime;
}

void MyGLWindow::paintGL()
{

  float r=(float)rand()/(float)RAND_MAX;
  float g=(float)rand()/(float)RAND_MAX;
  float b=(float)rand()/(float)RAND_MAX;

  // set the clear colour
  glClearColor(r,g,b,1);

  // clear screen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // setup fragment shader variables
  glUseProgram(_program);
  GLint unif_resolution, unif_time, unif_tex0;

  unif_time = glGetUniformLocation(_program, "time");
  float deltaTimeS = getDeltaTimeS();
  glUniform1f(unif_time, deltaTimeS);  
  
  unif_resolution = glGetUniformLocation(_program, "resolution");
  glUniform2f(unif_resolution, getMaxWidth(), getMaxHeight());

  unif_tex0 = glGetUniformLocation(_program, "tex0");
  if (unif_tex0 != -1)
  {
    if (_texture0 != 0)
    {
      glUniform1i(unif_tex0, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _texture0);
    }
  }

  /* Describe our vertices array to OpenGL */
  glBindBuffer(GL_ARRAY_BUFFER, _vbo_quad);
  glVertexAttribPointer(
    _attribute_coord2d, // attribute
    2,                 // number of elements per vertex, here (x,y)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );
  glEnableVertexAttribArray(_attribute_coord2d);

  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(_attribute_coord2d);

  // Swap back buffer to front
  swapBuffers();
}
