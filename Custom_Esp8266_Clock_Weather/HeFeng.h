
#pragma once
#include <ArduinoJson.h>

typedef struct HeFengCurrentData {

  String cond_txt;
  String fl;
  String tmp;
  String hum;
  String wind_sc;
  String iconMeteoCon;
  String sugg_1;
  String sugg_2;
  String loca_city;
}
HeFengCurrentData;
typedef struct HeFengForeData {
  String datestr;
  String tmp_min;
  String tmp_max;
  String iconMeteoCon;

}
HeFengForeData;
class HeFeng {
  private:
    String getMeteoconIcon(String cond_code);
  public:
    HeFeng();
    void doUpdateCurr(HeFengCurrentData *data, String key, String location);
    void doUpdateFore(HeFengForeData *data, String key, String location);
    void lifesugg(HeFengCurrentData *data, String key,String locationID);
    
};

class bmpRemote {
  public:
    bmpRemote();
    unsigned char bmp_rcv[833];
    bool disp_state;
    bool downloadBmp(String url);
};
unsigned char str16_to_hex(char inputstr16,char inputstr1);														   