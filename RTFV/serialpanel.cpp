#include <serialpanel.hpp>
#include <main.hpp>
#include <bitset>

SerialPanel::SerialPanel(wxPanel * parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(240,250), wxBORDER_SUNKEN)
{
  _portName = new wxStaticText(this, -1, wxT("ttyUSB0"), wxPoint(5,10));
  _status = new wxStaticText(this, -1, wxT("Status: N/A"), wxPoint(5,30));
  _msg = new wxStaticText(this, -1, wxT("Message: "), wxPoint(5,50));
  _conn = new wxButton(this, ID_CONN, wxT("Connect"), wxPoint(5,80));
  _disc = new wxButton(this, ID_DISC, wxT("Disconnect"), wxPoint(100,80));
  _serial = NULL;
  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnConnect, this, ID_CONN);
  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnDisconnect, this, ID_DISC);
}

SerialPanel::~SerialPanel()
{
  delete _serial;
}

void SerialPanel::OnConnect(wxCommandEvent & WXUNUSED(event))
{
  if(_serial==NULL){
    _serial = new commSerial(this);
    _serial->connect();
  }else{
    //
  }
}

void SerialPanel::OnDisconnect(wxCommandEvent & WXUNUSED(event))
{
  delete _serial;
  _serial = NULL; 
}

void SerialPanel::UpdateStatus(wxCommandEvent & event)
{
  _status->SetLabel("Status: " + event.GetString());
}

void SerialPanel::UpdateMessage(wxCommandEvent & event)
{
  _msg->SetLabel("Message: " + event.GetString());
}
