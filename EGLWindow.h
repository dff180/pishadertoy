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
#ifndef EGLWINDOW_H__
#define EGLWINDOW_H__

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <bcm_host.h>

#include "EGLConfig.h"



class EGLWindow
{
	public :

		/// @brief the ctor, will create a default config or the user
		/// can pass one in to use
		/// @param _config the config to  use
		EGLWindow(EGLconfig *_config=0);

		~EGLWindow();
		/// @brief must be implemented by the user this will
		/// be called after the ctor once we have a valid GL context
		virtual void paintGL()=0;
		/// @brief tell EGL to re-draw the current buffer
		void swapBuffers() const;
		/// @brief resize the screen with origin at 0,0
		/// @param _w the width
		/// @param _h the height
		void resizeScreen(
											 uint32_t _w,
											 uint32_t _h
										);
		/// @brief set screen using x,y and w/h
		/// @param[in] _x the left origin of the screen
		/// @param[in] _y the top origin of the screen
		/// @param[in] _w the width  of the screen
		/// @param[in] _h the height of the screen
		void setScreen(
										uint32_t _x,
										uint32_t _y,
										uint32_t _w,
										uint32_t _h
									);
		/// @brief get the max width
		inline uint32_t getMaxWidth()const {return m_maxWidth;}
		/// @brief get the  width
		inline uint32_t getWidth()const {return m_width;}
	/// @brief get the max width
		inline uint32_t getMaxHeight()const {return m_maxHeight;}
		/// @brief get the  width
		inline uint32_t getHeight()const {return m_height;}
		/// @brief set the flag to upscale the screen dst rectangle.
		/// by default this is not set
		inline void setUpscale(bool _f){m_upscale=_f;}


	protected :
		/// @brief this must be implemented by the user, it is called only once when the
		/// window is created
		virtual void initializeGL()=0;
		/// @brief the current window width
		uint32_t m_width;
		/// @brief the current window height
		uint32_t m_height;
		/// @brief max width reported from EGL
		uint32_t m_maxWidth;
		/// @brief max heigh reported from EGL
		uint32_t m_maxHeight;
		/// @brief the display
		EGLDisplay m_display;
		/// @brief the surface
		EGLSurface m_surface;
		/// @brief the gl context
		EGLContext m_context;
		/// @brief our config either created by the user and passed in or
		/// a default one will be created for us
		EGLconfig *m_config;
		/// @brief vc display manager element
		DISPMANX_ELEMENT_HANDLE_T m_dispmanElement;
		/// @brief vc display manager display structure
 		DISPMANX_DISPLAY_HANDLE_T m_dispmanDisplay;
		/// @brief vc display manager update structure
 		DISPMANX_UPDATE_HANDLE_T m_dispmanUpdate;
	 private :
	  /// @brief destroy the surface if it exists
	 	void destroySurface();
	 	/// @brief this is the main function to create our surface
	 	/// @param[in] _x the left origin of the screen
		/// @param[in] _y the top origin of the screen
		/// @param[in] _w the width  of the screen
		/// @param[in] _h the height of the screen

	 	void makeSurface(
	 										uint32_t _x,
	 										uint32_t _y,
	 										uint32_t _w,
	 										uint32_t _h
	 									);
	 	/// @brief flag to indicate if the surface has been created
	 	bool m_activeSurface;
	 	/// @brief flag to indicate if we upscale to full screen resolution
	 	bool m_upscale;

};


#endif
