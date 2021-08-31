#ifndef ANONYMOUSGLCONTEXT_HPP
#define ANONYMOUSGLCONTEXT_HPP

#include <wx/glcanvas.h>
#include <wx/wx.h>

class AnonymousGLContext : public wxGLContext
{
public:
  AnonymousGLContext(wxGLCanvas *canvas);
  void DrawModel(float rx,float ry, float rz);
  void DrawModelTris(float rx, float ry, float rz);
  
private:
  bool loadOBJ();
  std::vector<std::vector<std::vector<float>>> _faces;
  std::vector<std::vector<std::vector<float>>> _normals;
  std::vector<std::vector<std::vector<float>>> _uvs;
};

#endif //ANONYMOUSGLCONTEXT_HPP
