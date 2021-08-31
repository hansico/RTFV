#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <wx/wx.h>
#include <commserial.hpp>
#include <glpanel.hpp>

class MessageHandler
{
public:
  MessageHandler();
  ~MessageHandler();
  
  void msgpipe(uint8_t message[32]);

private:
  bool check_lrc(uint8_t msg[], int len);
  float extract_float(uint8_t message[], int beginindx);
};

union bfloat{
  uint8_t arr[4];
  float f;
};
wxDEFINE_EVENT(ROT_GL,wxCommandEvent);
#endif // MESSAGEHANDLER_HPP
