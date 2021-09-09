#include <serialpanel.hpp>
#include <main.hpp>
#include <bitset>

SerialPanel::SerialPanel(wxPanel * parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(240,250), wxBORDER_SUNKEN)
{
  _serial = new commSerial();
  wxArrayString baudoptions = getBaudOptions();
  _portName = new wxStaticText(this, -1, wxT("ttyUSB0"), wxPoint(5,10));
  _status = new wxStaticText(this, -1, wxT("Status: N/A"), wxPoint(5,30));
  _msg = new wxStaticText(this, -1, wxT("Message: "), wxPoint(5,50));
  _conn = new wxButton(this, ID_CONN, wxT("Connect"), wxPoint(5,80));
  _disc = new wxButton(this, ID_DISC, wxT("Disconnect"), wxPoint(100,80));
  _baudSelectBox = new wxChoice(this, ID_BAUDBOX, wxPoint(5,200), wxDefaultSize, baudoptions);
  _baudSelectBox->SetSelection(baudoptions.GetCount()-1);

  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnConnect, this, ID_CONN);
  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SerialPanel::OnDisconnect, this, ID_DISC);
}

SerialPanel::~SerialPanel()
{
  delete _serial;
}

void SerialPanel::OnConnect(wxCommandEvent & WXUNUSED(event))
{
  int st = -128;

  if(_serial==NULL){
    _serial = new commSerial();
    st = _serial->connect(_baudSelectBox->GetString(_baudSelectBox->GetSelection()).ToStdString());
  }else{
    st = _serial->connect(_baudSelectBox->GetString(_baudSelectBox->GetSelection()).ToStdString());
  }

  if(st==1){ // Successful connection
    _conn->Enable(false);
    _baudSelectBox->Enable(false);
  }
  UpdateStatus(_serial->statusconvert(st));
}

void SerialPanel::OnDisconnect(wxCommandEvent & WXUNUSED(event))
{
  int st = _serial->disconnect();
  UpdateStatus(_serial->statusconvert(st));
  _conn->Enable(true);
  _baudSelectBox->Enable(true);
}

void SerialPanel::UpdateStatus(const char* newstatus)
{
  std::string a = "Status: ";
  a+=newstatus;
  _status->SetLabel(a);
}

void SerialPanel::UpdateMessage(wxCommandEvent & event)
{
  _msg->SetLabel("Message: " + event.GetString());
}

wxArrayString SerialPanel::getBaudOptions()
{
  std::vector<std::string> bauds = _serial->availableBauds;

  wxArrayString baudoptions;
  for(const std::string &baud : bauds)
  {
    baudoptions.Add(baud);
  }
  return baudoptions;
}
