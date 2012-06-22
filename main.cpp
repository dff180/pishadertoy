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
#include <string>
#include "MyGLWindow.h"
#include "bcm_host.h"


int main(int argc, char ** argv)
{
 atexit( bcm_host_deinit);

	// Parse command line
	std::string fragmentShader;
	if (argc == 2)
		fragmentShader = argv[1];

	std::cout<<"starting GL test\n";
		bcm_host_init();
	std::cout<<"done bcm init\n";
	// here I create a config with RGB bit size 5,6,5 and no alpha
	EGLconfig *config = new EGLconfig();
//	config->setRGBA(5,6,5,0);
	config->setRGBA(8,8,8,8);
	// set the depth buffer
	config->setDepth(16);
	// now create a new window using the default config
	MyGLWindow win(config, fragmentShader);

/*
	// now set the size of the screen in this case I'm going to do a
	// rectangle in the middle (if you don't call this you would get a full
	// screen rect by default)
	uint32_t w=win.getMaxWidth();
	uint32_t h=win.getMaxHeight();
	// set this to true to upscale the dst rect
	win.setUpscale(false);
	win.setScreen(w/4,h/4,w/2,h/2);
	int x,y;
*/

	while(1)
	{
		win.paintGL();
		sleep(0.01);
	}
}

