#include <mainwindow.hpp>

MainWindow::MainWindow(const wxString& title)
   : wxFrame(NULL, wxID_ANY, title, wxPoint(10,10), wxSize(640,400))
{
  _parent = new wxPanel(this, wxID_ANY);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
  
  _serialpanel = new SerialPanel(_parent);
  _glpanel = new GLPanel(_parent);
  
  hbox->Add(_glpanel, 0, wxEXPAND | wxALL, 5);
  hbox->Add(_serialpanel, 0, wxEXPAND | wxALL, 5);
  _parent->SetSizer(hbox);
  this->Centre();
}

MainWindow::~MainWindow()
{
  delete _glpanel;
  delete _serialpanel;
  delete _parent;
}

