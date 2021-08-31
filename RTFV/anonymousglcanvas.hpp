#ifndef ANONYMOUSGLCANVAS_HPP
#define ANONYMOUSGLCANVAS_HPP

#include <sstream>
#include <wx/wx.h>
#include <wx/glcanvas.h>

class GLPanel;
class AnonymousGLContext;

class AnonymousGLCanvas : public wxGLCanvas
{
public:
  AnonymousGLCanvas(wxWindow *parent, GLPanel *container, int *attribList = NULL);
  ~AnonymousGLCanvas();
  void Rotate(wxCommandEvent& evt);
  
private:
  void OnPaint(wxPaintEvent& evt);
  std::vector<float> ConvertMsgTo3floats(std::string str,char delimiter='|');
  GLPanel *_parent;
  
  float _rx = 0.0;
  float _ry = 0.0;
  float _rz = 0.0;
};

#endif //ANONYMOUSGLCANVAS_HPP
