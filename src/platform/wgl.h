#pragma once

typedef void (APIENTRY* GLGENFRAMEBUFFERSPROC)(GLsizei, GLuint*);
typedef void (APIENTRY* GLGENRENDERBUFFERSPROC)(GLsizei, GLuint*);
typedef void (APIENTRY* GLBINDFRAMEBUFFERPROC)(GLenum, GLuint);
typedef void (APIENTRY* GLBINDRENDERBUFFERPROC)(GLenum, GLuint);
typedef void (APIENTRY* GLRENDERBUFFERSTORAGEMULTISAMPLE)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
typedef void (APIENTRY* GLFRAMEBUFFERRENDERBUFFER)(GLenum, GLenum, GLenum, GLuint);
typedef void (APIENTRY* GLBLITFRAMEBUFFER)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
typedef GLenum (APIENTRY* GLCHECKFRAMEBUFFERSTATUS)(GLenum);

#define glGenFramebuffers(n, ids) (((GLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers"))(n, ids))
#define glGenRenderbuffers(n, ids) (((GLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers"))(n, ids))

#define glBindFramebuffer(target, framebuffer) (((GLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer"))(target, framebuffer))
#define glBindRenderbuffer(target, framebuffer) (((GLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer"))(target, framebuffer))

#define glRenderbufferStorageMultisample(target, samples, internalformat, width, height) (((GLRENDERBUFFERSTORAGEMULTISAMPLE)wglGetProcAddress("glRenderbufferStorageMultisample"))(target, samples, internalformat, width, height))
#define glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) (((GLFRAMEBUFFERRENDERBUFFER)wglGetProcAddress("glFramebufferRenderbuffer"))(target, attachment, renderbuffertarget, renderbuffer))
#define glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter) (((GLBLITFRAMEBUFFER)wglGetProcAddress("glBlitFramebuffer"))(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter))

#define glCheckFramebufferStatus(target) (((GLCHECKFRAMEBUFFERSTATUS)wglGetProcAddress("glCheckFramebufferStatus"))(target))


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#ifndef GL_FRAMEBUFFER
#define GL_FRAMEBUFFER  0x8D40
#endif

#ifndef GL_RENDERBUFFER
#define GL_RENDERBUFFER  0x8D41
#endif

#ifndef GL_COLOR_ATTACHMENT0
#define GL_COLOR_ATTACHMENT0  0x8CE0
#endif

#ifndef GL_READ_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER  0x8CA8
#endif

#ifndef GL_DRAW_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER  0x8CA9
#endif

#ifndef GL_FRAMEBUFFER_COMPLETE
#define GL_FRAMEBUFFER_COMPLETE  0x8CD5
#endif