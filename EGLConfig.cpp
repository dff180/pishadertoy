#include "EGLConfig.h"
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdlib>


/// @brief ctor will create a default attrib set
EGLconfig::EGLconfig()
{
	m_attributes[EGL_RED_SIZE]=8;
	m_attributes[EGL_GREEN_SIZE]=8;
	m_attributes[EGL_BLUE_SIZE]=8;
	m_attributes[EGL_ALPHA_SIZE]=8;
	m_attributes[EGL_SURFACE_TYPE]=EGL_WINDOW_BIT;
	std::cout <<"make new config\n";
}
/// @brief dtor (doesn't do anything
EGLconfig::~EGLconfig()
{

}

void EGLconfig::setAttribute(EGLint _attrib,EGLint _value)
{
	assert(_attrib >= 0x3020 && _attrib <=0x3042);
	m_attributes[_attrib]=_value;
}
void EGLconfig::setRGBA(EGLint _r,EGLint _g, EGLint _b, EGLint _a)
{
	m_attributes[EGL_RED_SIZE]=_r;
	m_attributes[EGL_GREEN_SIZE]=_g;
	m_attributes[EGL_BLUE_SIZE]=_b;
	m_attributes[EGL_ALPHA_SIZE]=_a;

}
void EGLconfig::setDepth(EGLint _d)
{
	m_attributes[EGL_DEPTH_SIZE]=_d;

}

void EGLconfig::setSurface(EGLint _s)
{
	m_attributes[EGL_SURFACE_TYPE]=_s;
}

void EGLconfig::chooseConfig(EGLDisplay _display)
{
	std::map<EGLint,EGLint>::const_iterator it;

	std::vector <EGLint> attribs;
	for ( it=m_attributes.begin() ; it != m_attributes.end(); it++ )
	{
		attribs.push_back((*it).first);
		attribs.push_back((*it).second);
	}
	attribs.push_back(EGL_NONE);

	EGLBoolean result;
	EGLint numConfig;

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(_display, &attribs[0], &m_config, 1, &numConfig);
	std::cout<<"got numCofig ="<<numConfig<<"\n";
	if( result==EGL_FALSE )
	{
		std::cerr<<"error setting config check your setting or if you have a valid display\n";
		exit(EXIT_FAILURE);
	}
	std::cout<<"choosing config\n";

}
