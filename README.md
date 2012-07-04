pishadertoy
===========
by dff180

Simple tool to test OpenGL ES 2.0 fragment shaders on the Raspberry Pi.

Fragment shader and texture can be specified on command-line.

Credits
-------

* EGL class code by [Jon Macey](http://jonmacey.blogspot.de/2012/06/opengl-es-on-raspberry-pi-pt-3-creating.html)

* Sample shaders and from [ShaderToy](http://www.iquilezles.org/apps/shadertoy/). Explore the ShaderToy website for additional shader and texture usage examples.


Installation
------------
Note: The following instructions are for [Raspbian](http://www.raspbian.org/) Raspberry Pi installs (Pisces R1 image, Debian Wheezy).
They assume that you already have set up a working development environment where you can compile the hello\_pi examples.

1. Get sources with `git clone https://github.com/dff180/pishadertoy.git`
2. Install dependencies with `sudo apt-get install libsoil-dev`
3. `cd pishadertoy`
4. `make`

Running
-------
* Start from Raspberry Pi console or via SSH client 

* Exit program with CTRL-C

### Examples

* Using default fragment shader

    `./pishadertoy`
    
* Using fragment shader which doesn't need texture

    `./pishadertoy shaders/plasma.f.glsl`
    
* Using fragment shader with texture

    `./pishadertoy shaders/deform.f.glsl textures/mytexture.jpg`
    
