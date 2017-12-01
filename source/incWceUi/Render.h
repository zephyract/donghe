// $Change: 395037 $

#ifndef __RENDER_H__
#define __RENDER_H__

// #include <GLES/egl.h>
// #include <GLES/gl.h>
#include "egl.h"
#include "gl2.h"
// #include "glues.h"
// #include "glutes.h"

#define NO_GDI

static const unsigned int PRECISION = 16;
static const GLfixed ONE  = 1 << PRECISION;
static const GLfixed ZERO = 0;

inline GLfixed FixedFromInt(int value) {return value << PRECISION;};
inline GLfixed FixedFromFloat(float value) {return static_cast<GLfixed>(value * static_cast<float>(ONE));};
inline GLfixed MultiplyFixed(GLfixed op1, GLfixed op2) {return static_cast<GLfixed>(((__int64)op1 * (__int64)op2) >> PRECISION);};

class GLRender
{
public:
  GLRender();
  virtual ~GLRender();

  BOOL InitializeEGL(HWND hWnd, HDC hDC);
  BOOL TerminateEGL();
  void SwapBuffer();
  BOOL CopyBuffers(HBITMAP hbmp);

protected:
  EGLDisplay m_eglDisplay;
  EGLConfig m_eglConfig;
  EGLSurface m_eglSurface;
  EGLContext m_eglContext;
  NativeWindowType m_eglWindow;

  EGLSurface m_eglPixmapSurface;
public:
  void CreateBitmap();
  HBITMAP m_hbmp;
};



#endif /* __RENDER_H__ */

