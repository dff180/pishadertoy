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
#include "MyGLWindow.h"
#include <iostream>

MyGLWindow::MyGLWindow(EGLconfig *_config) : EGLWindow(_config)
{
	std::cout<<"My GL Window Ctor\n";
	// init GL in this case we are going to create some render buffers
	// for colour and depth
	initializeGL();
	srand(time(NULL));
}
MyGLWindow::~MyGLWindow()
{

}
void MyGLWindow::initializeGL()
{
	// set the clear colour
	glClearColor(1,1,1,1);

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
	// to update we need to swap the buffers
	swapBuffers();
}
