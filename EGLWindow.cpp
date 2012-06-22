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
#include "EGLWindow.h"
#include "EGLConfig.h"
#include "GLES2/gl2.h"
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>



// instrument this a bit like QGLWidget by adding a
// a context so we can choose GL version rgb size etc.
// This can be done with a context object at some stage.
EGLWindow::EGLWindow(EGLconfig *_config)
{
 // toggle we don't yet have an active surface
 m_activeSurface=false;
 // set default to not upscale the screen resolution
 m_upscale=false;
 // set our display values to 0 (not once ported to cx11 will use nullptr but
 // current pi default compiler doesn't support it yet
 m_display=0;
 m_context=0;
 m_surface=0;

 // now find the max display size (we will use this later to assert if the user
 // defined sizes are in the correct bounds
 int32_t success = 0;
 success = graphics_get_display_size(0 , &m_width, &m_height);
 assert( success >= 0 );
 std::cout<<"max width and height "<<m_width<<" "<<m_height<<"\n";
 m_maxWidth=m_width;
 m_maxHeight=m_height;
 // if we have a user defined config we will use that else we need to create one
 if (_config == 0)
 {
 	std::cout<<"making new config\n";
	m_config= new EGLconfig();
 }
 else
 {
		m_config=_config;
 }
 // this code actually creates the surface
 makeSurface(0,0,m_width,m_height);

}


void EGLWindow::setScreen(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h)
{
	// destroy our surface the make a new one
	destroySurface();
	makeSurface(_x,_y,_w,_h);
}


void EGLWindow::makeSurface(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h)
{
	// this code does the main window creation
	EGLBoolean result;

	static EGL_DISPMANX_WINDOW_T nativeWindow;
	// our source and destination rect for the screen
	VC_RECT_T dstRect;
	VC_RECT_T srcRect;

	// config you use OpenGL ES2.0 by default
	static const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};


	// get an EGL display connection
	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(m_display == EGL_NO_DISPLAY)
	{
		std::cerr<<"error getting display\n";
		exit(EXIT_FAILURE);
	}
	// initialize the EGL display connection
	int major,minor;

	result = eglInitialize(m_display, &major, &minor);
	std::cout<<"EGL init version "<<major<<"."<<minor<<"\n";
	if(result == EGL_FALSE)
	{
		std::cerr<<"error initialising display\n";
		exit(EXIT_FAILURE);
	}
	// get our config from the config class
	m_config->chooseConfig(m_display);
	EGLConfig config=m_config->getConfig();
	// bind the OpenGL API to the EGL
	result = eglBindAPI(EGL_OPENGL_ES_API);
	if(result ==EGL_FALSE)
	{
		std::cerr<<"error binding API\n";
		exit(EXIT_FAILURE);
	}
	// create an EGL rendering context
	m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, context_attributes);
	if(m_context ==EGL_NO_CONTEXT)
	{
		std::cerr<<"couldn't get a valid context\n";
		exit(EXIT_FAILURE);
	}
	// create an EGL window surface the way this works is we set the dimensions of the srec
	// and destination rectangles.
	// if these are the same size there is no scaling, else the window will auto scale

	dstRect.x = _x;
	dstRect.y = _y;
	if(m_upscale == false)
	{
		dstRect.width = _w;
		dstRect.height = _h;
	}
	else
	{
		dstRect.width = m_maxWidth;
		dstRect.height = m_maxHeight;
	}
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.width = _w << 16;
	srcRect.height = _h << 16;
	// whilst this is mostly taken from demos I will try to explain what it does
	// there are very few documents on this ;-0
	// open our display with 0 being the first display, there are also some other versions
	// of this function where we can pass in a mode however the mode is not documented as
	// far as I can see
  m_dispmanDisplay = vc_dispmanx_display_open(0);
  // now we signal to the video core we are going to start updating the config
	m_dispmanUpdate = vc_dispmanx_update_start(0);
	// this is the main setup function where we add an element to the display, this is filled in
	// to the src / dst rectangles
	m_dispmanElement = vc_dispmanx_element_add ( m_dispmanUpdate, m_dispmanDisplay,
		0, &dstRect, 0,&srcRect, DISPMANX_PROTECTION_NONE, 0 ,0,DISPMANX_NO_ROTATE);
	// now we have created this element we pass it to the native window structure ready
	// no create our new EGL surface
	nativeWindow.element = m_dispmanElement;
	nativeWindow.width =_w;
	nativeWindow.height =_h;
	// we now tell the vc we have finished our update
	vc_dispmanx_update_submit_sync( m_dispmanUpdate );

	// finally we can create a new surface using this config and window
	m_surface = eglCreateWindowSurface( m_display, config, &nativeWindow, NULL );
	assert(m_surface != EGL_NO_SURFACE);
	// connect the context to the surface
	result = eglMakeCurrent(m_display, m_surface, m_surface, m_context);
	assert(EGL_FALSE != result);
	m_activeSurface=true;

}



void EGLWindow::destroySurface()
{
	if(m_activeSurface == true)
	{
		eglSwapBuffers(m_display, m_surface);
		// here we free up the context and display we made earlier
		eglMakeCurrent( m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
		eglDestroySurface( m_display, m_surface );
		eglDestroyContext( m_display, m_context );
		eglTerminate( m_display );
		m_activeSurface=false;
	}
}


EGLWindow::~EGLWindow()
{
	// clear screen
	std::cout<<"dtor called\n";
	glClear( GL_COLOR_BUFFER_BIT );
	destroySurface();
}

void EGLWindow::swapBuffers() const
{
	eglSwapBuffers(m_display, m_surface);
}



void EGLWindow::resizeScreen(uint32_t _w, uint32_t _h)
{
	destroySurface();
	makeSurface(0,0,_w,_h);
}
