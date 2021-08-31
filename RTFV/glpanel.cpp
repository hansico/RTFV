#include <glpanel.hpp>

GLPanel::GLPanel(wxPanel *parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400,400), wxBORDER_SUNKEN)
{
  _canvas = new AnonymousGLCanvas(this,this);
  _context = NULL;
}

void GLPanel::Close()
{
  delete _canvas;
  delete _context;
  
  Destroy();
}

AnonymousGLContext& GLPanel::GetContext(wxGLCanvas *canvas){
  AnonymousGLContext *glcontext;
  if(!_context){
    _context = new AnonymousGLContext(canvas);
  }
  glcontext = _context;
  glcontext->SetCurrent(*canvas);
  return *glcontext;
}
