#ifndef GLPANEL_HPP
#define GLPANEL_HPP

#include <wx/panel.h>
#include <anonymousglcanvas.hpp>
#include <anonymousglcontext.hpp>

class GLPanel: public wxPanel
{
public:
  GLPanel(wxPanel *parent);
  
  AnonymousGLContext& GetContext(wxGLCanvas *canvas);
  AnonymousGLCanvas *_canvas;
  
private:
  AnonymousGLContext *_context;
  void Close();
};

#endif // GLPANEL_HPP
