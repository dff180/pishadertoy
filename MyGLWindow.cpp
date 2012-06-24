/*
  Copyright (C) 2012 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received m_a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <stdio.h>

#include <SOIL/SOIL.h>
#include "shader_utils.h"
#include "MyGLWindow.h"

MyGLWindow::MyGLWindow(EGLconfig *_config, std::string fragmentShader, std::string textureName) 
: EGLWindow(_config)
, _fragmentShader(fragmentShader)
, _textureName(textureName)
, _texture0(0)
{
	std::cout<<"My GL Window Ctor\n";
	// init GL in this case we are going to create some render buffers
	// for colour and depth
	initializeGL();
	srand(time(NULL));
}

MyGLWindow::~MyGLWindow()
{
//TODO is this ever called
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

  glGenBuffers(1, &vbo_quad);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if (_fragmentShader != "")
  {
    if ((fs = create_shader(_fragmentShader.c_str(), GL_FRAGMENT_SHADER)) == 0) return 0;
  }
  else
    if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

//  if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
//  if ((fs = create_shader("apple.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
//  if ((fs = create_shader("nautilus.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
//  if ((fs = create_shader("monjori.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
//  if ((fs = create_shader("shapes.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program);
    return 0;
  }

  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
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
 // 	glEnable(GL_BLEND);
 //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set the clear colour
//	glClearColor(1,1,1,1);

	// Start timer
	gettimeofday(&startTime, NULL);
}

float MyGLWindow::getDeltaTimeS()
{
	timeval currentTime;
	gettimeofday(&currentTime, NULL);

	float deltaTime = (currentTime.tv_sec - startTime.tv_sec); 
	deltaTime += (currentTime.tv_usec - startTime.tv_usec) / 1000000.0; // us to s
	return deltaTime;
}

void MyGLWindow::paintGL()
{

  // scale the colour based on the width
	float r=(float)rand()/(float)RAND_MAX;
	float g=(float)rand()/(float)RAND_MAX;
	float b=(float)rand()/(float)RAND_MAX;
	// set the clear colour
	glClearColor(r,g,b,1);
	// clear screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// jt

  glUseProgram(program);
  GLint unif_resolution, unif_time, unif_tex0;

  unif_time = glGetUniformLocation(program, "time");
  float deltaTimeS = getDeltaTimeS();
//printf("%f\n", deltaTimeS);
  glUniform1f(unif_time, deltaTimeS);  
  
  unif_resolution = glGetUniformLocation(program, "resolution");
  glUniform2f(unif_resolution, 640.0, 480.0);

  unif_tex0 = glGetUniformLocation(program, "tex0");
  if (unif_tex0 != -1)
  {
    if (_texture0 != 0)
    {
      glUniform1i(unif_tex0, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, _texture0);
    }
  }

//printf("%d, %d\n", unif_time, unif_resolution);

  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
  glVertexAttribPointer(
    attribute_coord2d, // attribute
    2,                 // number of elements per vertex, here (x,y)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );
  glEnableVertexAttribArray(attribute_coord2d);

  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(attribute_coord2d);


// jt end

	// to update we need to swap the buffers
	swapBuffers();
}
