#include <jd/gfx/glutil.h>

const char * GetGLErrorString(GLenum err)
{
    switch (err)
    {
        case GL_NO_ERROR:       return "No Error";  break;
        case GL_INVALID_ENUM:   return "Invalid Enum";  break;
        case GL_INVALID_VALUE:  return "Invalid Value"; break;
        case GL_INVALID_OPERATION:  return "Invalid Operation"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "Invalid Framebuffer Operation"; break;
        case GL_OUT_OF_MEMORY:  return "Out of Memory";  break;
    }
    return "(unknown)";
}

const char * GetGLCheckFramebufferStatusCodeString( GLenum status )
{
    switch (status)
    {
        case GL_FRAMEBUFFER_COMPLETE:   return "GL_FRAMEBUFFER_COMPLETE"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:  return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
        //case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:  return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"; break;
        case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED"; break;
    }
    return "(unknown)";
}

