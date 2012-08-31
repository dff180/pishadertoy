// by dff180

#include <iostream>
#include <string>
#include <stdlib.h>
#include "MyGLWindow.h"
#include "bcm_host.h"

int main(int argc, char ** argv)
{
  atexit( bcm_host_deinit);

  // Parse command line
  std::string fragmentShader;
  std::string textureName;

  if (argc >= 2)
    fragmentShader = argv[1];

  if (argc >= 3)
    textureName = argv[2];

  // Init graphics
  std::cout << "bcm_host_init()" << std::endl;
  bcm_host_init();
  std::cout << "bcm_host_init() : Done\n";

  EGLconfig *config = new EGLconfig();
  config->setRGBA(8,8,8,8);
  
  config->setDepth(16);
  
  MyGLWindow win(config, fragmentShader, textureName);

  while(1)
  {
    win.paintGL();
    sleep(0.01);
  }
}

