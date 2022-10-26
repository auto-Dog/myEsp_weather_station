/**************************************************************/
/*   Powerd by DAYU and customized by AlphaDu,2021/08/24      */
/*   Ver 2.03 without info                                    */
/*   Add dim mode;                                            */
/*   Add energy saving mode                                   */
/**************************************************************/
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <SH1106.h>
#include <SH1106Brzo.h>
#include <SH1106Spi.h>
#include <SH1106Wire.h>
#include <SSD1306.h>
#include <SSD1306Brzo.h>
#include <SSD1306I2C.h>
#include <SSD1306Spi.h>
#include <SSD1306Wire.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <DNSServer.h>//密码直连将其三个库注释
#include <ESP8266WebServer.h>

#include <CustomWiFiManager.h>

#include <time.h>
#include <sys/time.h>
#include <coredecls.h>


//#include "SH1106Wire.h"   //1.3寸用这个
#include "SSD1306Wire.h"    //0.96寸用这个
#include "OLEDDisplayUi.h"
#include "HeFeng.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"

/***************************
   Begin Settings
 **************************/
#define BJ_haidian "101010200"
#define mySSID "WeatherTV-IOT"		//更改这个字符串，可以为模块SSID改名

const char* WIFI_SSID = "TPLINK11";  //填写你的WIFI名称及密码（可以不用管）
const char* WIFI_PWD = "8888888888";

//const char* BILIBILIID = "488684813";  //填写你的B站账号（已经屏蔽显示粉丝数功能，改为生活建议）

//由于太多人使用我的秘钥，导致获取次数超额，所以不提供秘钥了，大家可以到https://dev.heweather.com/获取免费的
const char* HEFENG_KEY = "";//填写你的和风天气秘钥
//const char* HEFENG_LOCATION = "CN101010100";//填写你的城市ID,可到https://where.heweather.com/index.html查询
//const char* HEFENG_LOCATION = "auto_ip";//自动IP定位
const char* HEFENG_LOCATION = BJ_haidian;//根据已知地址ID定位
const char* mypicurl=""; //填写你的Json地址

#define TZ              8      // 中国时区为8
#define DST_MN          0      // 默认为0

const int UPDATE_INTERVAL_SECS = 30 * 60; // 30分钟更新一次天气
const int UPDATE_CURR_INTERVAL_SECS = 30 * 59; // 30分钟更新一次生活建议
const unsigned int RESET_SECS = 12*60*60; //12小时后触发重启

const int I2C_DISPLAY_ADDRESS = 0x3c;  //I2c地址默认
const int SDA_PIN = 0;  //引脚连接
const int SDC_PIN = 2;  //


const String WDAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  //星期
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};  //月份

// web配网页面自定义我的图标请随便使用一个图片转base64工具转换https://tool.css-js.com/base64.html, 64*64
const char Icon[] PROGMEM = "data:image/jpeg;base64,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";

/***************************
   End Settings
 **************************/

//SH1106Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 1.3寸用这个
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 0.96寸用这个
OLEDDisplayUi   ui( &display );

HeFengCurrentData currentWeather; //实例化对象
HeFengForeData foreWeather[3];
HeFeng HeFengClient;
bmpRemote mypic;


#define TZ_MN           ((TZ)*60)   //时间换算
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)

time_t now; //实例化时间

bool readyForWeatherUpdate = false; // 天气更新标志
bool first = true;  //首次更新标志
long timeSinceLastWUpdate = 0;    //上次更新后的时间
long timeSinceLastCurrUpdate = 0;   //上次天气更新后的时间

String curr_sugg1 = "-1"; //生活建议-户外运动
String curr_sugg2 = "-1"; //生活建议-晾衣服

bool webconnect(bool is_running);
void drawProgress(OLEDDisplay *display, int percentage, String label);   //提前声明函数
void updateData(OLEDDisplay *display);
void updateDatas(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void setReadyForWeatherUpdate();
void drawBkg(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

//添加框架
//此数组保留指向所有帧的函数指针
//框架是从右向左滑动的单个视图
FrameCallback frames[] = { drawDateTime, drawCurrentWeather, drawForecast,drawBkg };
//页面数量
int numberOfFrames = 4;

OverlayCallback overlays[] = { drawHeaderOverlay }; //覆盖回调函数
int numberOfOverlays = 1;  //覆盖数

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // 屏幕初始化
  display.init();
  display.clear();
  display.display();
  //display.flipScreenVertically(); //屏幕翻转
  display.setContrast(255); //屏幕亮度
  //初始化界面
  display.clear();
  display.drawXbm(0, 0, 128, 64, bilibili); //显示哔哩哔哩
  display.display();
  //Web配网，密码直连请注释
  webconnect(0);

  // 用固定密码连接，Web配网请注释
  //wificonnect();

  ui.setTargetFPS(30);  //刷新频率

  ui.setActiveSymbol(activeSymbole); //设置活动符号
  ui.setInactiveSymbol(inactiveSymbole); //设置非活动符号

  // 符号位置
  // 你可以把这个改成TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // 定义第一帧在栏中的位置
  ui.setIndicatorDirection(LEFT_RIGHT);

  // 屏幕切换方向
  // 您可以更改使用的屏幕切换方向 SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  ui.setFrames(frames, numberOfFrames); // 设置框架
  ui.setTimePerFrame(5000); //设置切换时间

  ui.setOverlays(overlays, numberOfOverlays); //设置覆盖

  // UI负责初始化显示
  ui.init();

  //display.flipScreenVertically(); //屏幕反转
  //display.test_debug(); //屏幕反转

  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com"); //ntp获取时间，你也可用其他"pool.ntp.org","0.cn.pool.ntp.org","1.cn.pool.ntp.org","ntp1.aliyun.com"
  delay(200);

}

void loop() {
  if (millis() - timeSinceLastWUpdate > (1000L * RESET_SECS)){
    ESP.restart();
    delay(1000);
  }
  if (first) {  //首次加载
    updateDatas(&display);
    first = false;
  }
  if (millis() - timeSinceLastWUpdate > (1000L * UPDATE_INTERVAL_SECS)) { //屏幕刷新
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }
  /*
  if (millis() - timeSinceLastCurrUpdate > (1000L * UPDATE_CURR_INTERVAL_SECS)) { //粉丝数更新
    HeFengClient.fans(&currentWeather, BILIBILIID);
    fans = String(currentWeather.follower);
    timeSinceLastCurrUpdate = millis();
  }
  
  if (millis() - timeSinceLastCurrUpdate > (1000L * UPDATE_CURR_INTERVAL_SECS)) { //生活建议更新
    HeFengClient.lifesugg(&currentWeather, HEFENG_KEY,DL_ganjingzi);
    curr_sugg1 = String(currentWeather.sugg_1);
    curr_sugg2 = String(currentWeather.sugg_2);
    timeSinceLastCurrUpdate = millis();
  }
  */
  if (readyForWeatherUpdate && ui.getUiState()->frameState == FIXED) { //天气更新
    updateData(&display);
  }

  int remainingTimeBudget = ui.update(); //剩余时间预算

  if (remainingTimeBudget > 0) {
    //你可以在这里工作如果你低于你的时间预算。
    delay(remainingTimeBudget);
  }
}

bool webconnect(bool is_running) {  ////Web配网，密码直连将其注释
  // display.clear();
  // display.drawXbm(0, 0, 128, 64, bilibili); //显示哔哩哔哩
  // display.display();
  WiFiManager wifiManager;  //实例化WiFiManager
  if(is_running){
    wifiManager.setConnectTimeout(20); //设置超时
	return wifiManager.autoConnect(mySSID);
  }
  else{
    wifiManager.setDebugOutput(false); //关闭Debug
    //wifiManager.setConnectTimeout(10); //设置超时
    wifiManager.setHeadImgBase64(FPSTR(Icon)); //设置图标
    wifiManager.setPageTitle("Alphadu的WiFi配置页");  //设置页标题

    if (!wifiManager.autoConnect(mySSID)) {  //AP模式
      Serial.println("连接失败并超时");
      //重新设置并再试一次，或者让它进入深度睡眠状态
      ESP.restart();
      delay(1000);
    }
    Serial.println("connected...^_^");
    yield();
  }
  return 1;
}

void drawProgress(OLEDDisplay *display, int percentage, String label) {    //绘制进度
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display) {  //天气更新
  //更新时wifi唤醒
  WiFi.forceSleepWake();
  Serial.println("WAKEUP now");
  delay(1);
  if(webconnect(1)){
    HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);
    mypic.downloadBmp(mypicurl);
    HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);
    HeFengClient.lifesugg(&currentWeather, HEFENG_KEY,DL_ganjingzi);
    curr_sugg1 = String(currentWeather.sugg_1);
    curr_sugg2 = String(currentWeather.sugg_2);
  }
  //更新后，进入wifi睡眠模式
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("SLEEP now");
  //disp_bmp_state=HeFengClient.downloadBmp(&currentWeather);  
  readyForWeatherUpdate = false;
}

void updateDatas(OLEDDisplay *display) {  //首次天气更新
  drawProgress(display, 0, "Updating data...");
  HeFengClient.lifesugg(&currentWeather, HEFENG_KEY,DL_ganjingzi);
  curr_sugg1 = String(currentWeather.sugg_1);
  curr_sugg2 = String(currentWeather.sugg_2);

  drawProgress(display, 25, "Updating weather...");
  HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);

  drawProgress(display, 50, "Updating bkg...");
  mypic.downloadBmp(mypicurl);
  //disp_bmp_state=HeFengClient.downloadBmp(&currentWeather);

  drawProgress(display, 75, "Updating forecasts...");
  HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);

  readyForWeatherUpdate = false;
  drawProgress(display, 100, "Done...");
  delay(200);
  //更新后，进入wifi睡眠模式
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("SLEEP now");
}

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //显示时间
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  String date = WDAY_NAMES[timeInfo->tm_wday];

  sprintf_P(buff, PSTR("%04d-%02d-%02d  %s"), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, WDAY_NAMES[timeInfo->tm_wday].c_str());
  display->drawString(64 + x, 5 + y, String(buff));
  display->setFont(ArialMT_Plain_24);

  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  //调整亮度，夜间模式
  if(timeInfo->tm_hour>0&&timeInfo->tm_hour<7){
    display->setBrightness(50);
  }
  else{
    display->setBrightness(255);
  }
  display->drawString(64 + x, 22 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}


void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //显示天气
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 40 + y, currentWeather.cond_txt + "    |   Wind: " + currentWeather.wind_sc + "  ");
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(62 + x, 0 + y, currentWeather.loca_city);
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = currentWeather.tmp + "°C" ;
  display->drawString(60 + x, 9 + y, temp);
  display->setFont(ArialMT_Plain_10);
  display->drawString(62 + x, 30 + y, currentWeather.fl + "°C | " + currentWeather.hum + "%");
  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 0 + y, currentWeather.iconMeteoCon);
}

void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //天气预报
  drawForecastDetails(display, x, y, 0);
  drawForecastDetails(display, x + 44, y, 1);
  drawForecastDetails(display, x + 88, y, 2);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex) {  //天气预报

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y, foreWeather[dayIndex].datestr);
  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, foreWeather[dayIndex].iconMeteoCon);

  String temp = foreWeather[dayIndex].tmp_min + " | " + foreWeather[dayIndex].tmp_max;
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

//DEBUG HERE
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {   //绘图页眉覆盖
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);

  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(6, 54, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  String temp1 = curr_sugg1;
  String temp2 = curr_sugg2;
//根据天气建议来显示不同建议图标
//户外运动建议
  if(curr_sugg1=="1"||curr_sugg1=="2"){
    display->drawXbm(92, 54, 15, 10, logo_bmp_2);
  }
  else if(curr_sugg1=="3"){
      display->drawXbm(92, 54, 15, 10, logo_bmp_3);
    }
  else{
    display->drawString(98, 54, "SE");  //无法获取数据、错误时显示
  }
  
//晾衣服建议
  if(curr_sugg2=="1"||curr_sugg2=="2"){
    display->drawXbm(112, 54, 15, 10, logo_bmp_0);
  }
  else if(curr_sugg2=="5"||curr_sugg2=="6"){
      display->drawXbm(112, 54, 15, 10, logo_bmp_1);
  }
  else if(curr_sugg2=="3"||curr_sugg2=="4"){
    display->drawString(112, 54, ".");
  }
  else{
    display->drawString(112, 54, "CE");
  }
  
  //display->drawXbm(92, 55, 15, 10, logo_bmp_0);
  //display->drawXbm(112, 55, 15, 10, logo_bmp_1);
  display->drawHorizontalLine(0, 52, 128);
}

void setReadyForWeatherUpdate() {  //为天气更新做好准备
  Serial.println("Setting readyForUpdate to true");
  readyForWeatherUpdate = true;
}

void drawBkg(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y){
  
  if(mypic.disp_state)
    display->drawXbm(x+32,y+10,64,32,mypic.bmp_rcv);
    //display->drawXbm(x+16,y+10,96,32,mypic.bmp_rcv);
  else
    display->drawXbm(x+32,y+10,64,32,map3264);
}
