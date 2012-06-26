pishadertoy
===========
by dff180

Simple tool to test OpenGL ES 2.0 fragment shaders on the Raspberry Pi.
Fragment shader and texture can be specified on command-line.

Credits
-------

Included sample shaders are from [ShaderToy](http://www.iquilezles.org/apps/shadertoy/)


Installation
------------
Note: The following instructions are for [Raspbian](http://www.raspbian.org/) Raspberry Pi installs (Debian Wheezy).
They assume that you already have set up a working development environment where you can compile the hello\_pi examples.

1. Get sources with `git clone https://github.com/dff180/pishadertoy.git`
2. Install dependencies
2.1 `sudo apt-get install libsoil-dev`
3. make

Running
-------
* Start from Raspberry PI console

* Exit program with CTRL-C

* Using default fragment shader

    `./pishadertoy`
    
* Using fragment shader who doesn't need texture

    `./pishadertoy shaders/plasma.f.glsl`
    
* Using fragment shader with texture

    `./pishadertoy shaders/deform.f.glsl textures/tex1.jpg`
    
