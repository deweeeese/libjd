#pragma once

#include <jd/base/build.h>
// HACK
//#include <qopengl.h>


#if BUILD_DEBUG

#define GL_LOG(...) {printf(__VA_ARGS__);printf("\n");}

// Wrap gl calls to check for errors:   GL_VERIFY( glEnable(GL_BLEND) );
// -- WARNING --
// - GL_VERIFY makes its own scope, so break up declarations and assignments
// - when assigning a variable, keep it inside the parens
// e.g.:
// const char * myStr;
// GL_VERIFY( myStr = glGetString(GL_VENDOR) );
//
#define GL_VERIFY(XXX) { XXX; GLenum glerr = glGetError(); \
    if (glerr) { \
        GL_LOG("OpenGL error -- %s -- in file %s, line %d:\n%s", GetGLErrorString(glerr), __FILE__, __LINE__, #XXX); \
        DEBUG_BREAK(); \
    } \
}

#else

#define GL_VERIFY(...) __VA_ARGS__
#endif


// returns number of bytes for a GL type
int SizeOfGLType( GLenum type );

const char * GetGLErrorString(GLenum err);
const char * GetGLCheckFramebufferStatusCodeString( GLenum status );
