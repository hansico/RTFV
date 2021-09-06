#include <messagehandler.hpp>
#include <main.hpp> // NEEDED FOR wxGetApp();
#include <bitset>

MessageHandler::MessageHandler()
{
  auto target = wxGetApp()._mainwindow->_serialpanel;
  target->Bind(UPD_MSG,&SerialPanel::UpdateMessage,target);
  
  auto target_canv = wxGetApp()._mainwindow->_glpanel->_canvas;
  target_canv->Bind(ROT_GL,&AnonymousGLCanvas::Rotate,target_canv);
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::msgpipe(uint8_t message[32]){
  if(!check_lrc(message, 14)){
    // TODO
    std::cout << "LRC failed.\n";
    return;
  }
  float rx = extract_float(message,1);
  float ry = extract_float(message,5);
  float rz = extract_float(message,9);

  std::cout << message[0] << '|' << rx << '|' << ry << '|' << rz << '\n';

  std::string msg = "";
  //msg += std::to_string(message[0])+'|';
  msg += std::to_string(rx)+'|';
  msg += std::to_string(ry)+'|';
  msg += std::to_string(rz)+'|';

  auto target = wxGetApp()._mainwindow->_serialpanel;
  wxCommandEvent event_new(UPD_MSG);
  event_new.SetString(msg);
  wxQueueEvent(target,event_new.Clone());

  auto target_canv = wxGetApp()._mainwindow->_glpanel->_canvas;
  wxCommandEvent event_canv(ROT_GL);
  event_canv.SetString(msg);
  wxQueueEvent(target_canv,event_canv.Clone());
}

bool MessageHandler::check_lrc(uint8_t msg[], int len)
{
    uint8_t lrc = 0x00;
    for(int i=0;i<len-1;++i){
      lrc ^= msg[i];
    }
    return msg[len-1]==lrc;
}

float MessageHandler::extract_float(uint8_t message[], int beginindx){
  bfloat fl;
  fl.arr[0] = message[beginindx];
  fl.arr[1] = message[beginindx+1];
  fl.arr[2] = message[beginindx+2];
  fl.arr[3] = message[beginindx+3];
  return fl.f;
}
