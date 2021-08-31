#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <serialpanel.hpp>
#include <glpanel.hpp>


class MainWindow : public wxFrame
{
public:
  MainWindow(const wxString& title="MYAPP");
  ~MainWindow();

  SerialPanel *_serialpanel;
  GLPanel *_glpanel;
  wxPanel *_parent;
};

#endif // MAINWINDOW_HPP
