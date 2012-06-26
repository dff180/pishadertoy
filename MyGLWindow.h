// by dff180

#ifndef MYGLWINDOW_H__
#define MYGLWINDOW_H__

#include <string>
#include <sys/time.h>
#include "EGLWindow.h"

class MyGLWindow : public EGLWindow
{
  public :
    //MyGLWindow(EGLconfig *_config=0);
    MyGLWindow(EGLconfig *_config, std::string fragmentShader, std::string textureName="");

    ~MyGLWindow();
 
    virtual void paintGL();
  protected :
  
    virtual void initializeGL();
    int init_resources();
    float getDeltaTimeS();

    std::string _fragmentShader;
    std::string _textureName;
    timeval     _startTime;

    GLuint      _texture0;

    GLuint      _vbo_quad;
    GLuint      _program;
    GLint       _attribute_coord2d;
};


#endif
