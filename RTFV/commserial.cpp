#include <commserial.hpp>
#include <serialpanel.hpp>
#include <main.hpp>

commSerial::commSerial()
{
  _workerThr = NULL;
}

commSerial::~commSerial()
{ 
  disconnect();
}

int commSerial::connect(std::string baudstr)
{
  bool p = open_port();
  if(!p){
    return ST_NOT_AVLBL;
  }
  
  if(!config(baudstr)){
    return ST_UNRECOG_BAUD;
  };
  _exit_signal = std::promise<void>();
  std::future<void> future = _exit_signal.get_future();
  _workerThr = new std::thread(&commSerial::readserial,this,std::move(future));
  connected = true;
  return ST_CONN;
}

int commSerial::disconnect(){
  std::cout << "Disconnecting\n";
  
  if(connected){_exit_signal.set_value();std::cout << "Exit signaled\n";}

  if(_workerThr!=NULL){_workerThr->join();}
  delete _workerThr;
  _workerThr = NULL;

  close(_dev);
  std::cout << "_dev closed\n";

  connected = false;
  return ST_DISCONN;
}

void commSerial::readserial(std::future<void> future){
  while(future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout){
    memset(_buff, 0x00 , sizeof(_buff));
    int n = read(_dev, &_buff, sizeof(_buff));
    
    if(n<0){
      std::cout << "ERROR" << n << " " << strerror(n) << "\n";
      break;
    }
    
    if(n>0){
      wxGetApp()._msgHandler->msgpipe(_buff);
      std::cout << n << " : " << _buff << "\n";
    }
  }
}

bool commSerial::open_port(){
  
  int dev;
  char port[] = "/dev/ttyUSB0";
  
  dev = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  
  if (dev < 0){
    std::cout << "commSerial: "<< port << " " << strerror(dev) << "\n";
    return false;
  }
  
  _dev = dev;
  fcntl(dev,F_SETFL, 0);
  return true;
}

bool commSerial::config(std::string baudstr){
  tcgetattr(_dev, &_settings);

  if(!setBaud(baudstr)){
    return false;
  }

  _settings.c_cflag |= ( CLOCAL | CREAD );
  _settings.c_oflag &= ~OPOST;
  _settings.c_oflag &= ~ONLCR;
  _settings.c_cflag &= ~PARENB;
  _settings.c_cflag &= ~CSTOPB;
  _settings.c_cflag &= ~CRTSCTS;
  _settings.c_lflag &= ~ICANON;
  _settings.c_lflag &= ~ISIG;
  _settings.c_iflag &= ~( IXON | IXOFF | IXANY );
  _settings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
  _settings.c_cc[VTIME] = 10;
  _settings.c_cc[VMIN] = 0;
  
  tcsetattr(_dev, TCSANOW, &_settings);
  return true;
}

bool commSerial::setBaud(std::string baudstr)
{
  if(baudstr=="9600"){
    cfsetispeed(&_settings, B9600);
    cfsetospeed(&_settings, B9600);
    return true;
  }

  if(baudstr=="38400"){
    cfsetispeed(&_settings, B38400);
    cfsetospeed(&_settings, B38400);
    return true;
  }

  if(baudstr=="57600"){
    cfsetispeed(&_settings, B57600);
    cfsetospeed(&_settings, B57600);
    return true;
  }

  if(baudstr=="115200"){
    cfsetispeed(&_settings, B115200);
    cfsetospeed(&_settings, B115200);
    return true;
  }

  return false;
}

const char * commSerial::statusconvert(int stcode)
{
  switch(_statusCodes(stcode))
  {
    case ST_UNRECOG_BAUD: return "Unrecognized baud";
    case ST_NOT_AVLBL:    return "Device not available";
    case ST_DISCONN:      return "Disconnected";
    case ST_CONN:         return "Connected";
    default:              return "Unexpected status code";
  }
}
