#include <stdio.h>
#include <string.h>
#include <anonymousglcanvas.hpp>
#include <glpanel.hpp>

AnonymousGLCanvas::AnonymousGLCanvas(wxWindow *parent, GLPanel *container, int *attribList)
  :wxGLCanvas(parent, wxID_ANY,attribList,wxDefaultPosition,
              wxSize(400,400),wxFULL_REPAINT_ON_RESIZE)
{
  _parent = container;
  Bind(wxEVT_PAINT,&AnonymousGLCanvas::OnPaint,this);
}

AnonymousGLCanvas::~AnonymousGLCanvas(){
  _parent = NULL;
}

void AnonymousGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  
  const wxSize ClientSize = GetClientSize();
  AnonymousGLContext& canvas = _parent->GetContext(this);
  glViewport(0, 0, ClientSize.x, ClientSize.y);

  canvas.DrawModel(_rx,_ry,_rz);
  SwapBuffers();
}

void AnonymousGLCanvas::Rotate(wxCommandEvent& event){
  std::vector<float> rots = ConvertMsgTo3floats(event.GetString().ToStdString());

  _rx = rots[0];
  _ry = rots[1];
  _rz = rots[2];

  Refresh(false);
}

std::vector<float> AnonymousGLCanvas::ConvertMsgTo3floats(std::string str,char delimiter){
  std::vector<float> splitted;
  std::stringstream ss(str);
  std::string item;
  while(std::getline(ss,item,delimiter)){
    splitted.push_back(std::stof(item));
  }
  return splitted;
}
