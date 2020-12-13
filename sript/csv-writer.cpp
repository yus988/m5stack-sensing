// CSVへの書き込み、Wifi接続、日時の取得などが可能

#include <M5Stack.h>
#include <WiFi.h>
#include <time.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "F660A-TJVg-G";
char pass[] = "FzPDVsCh";

// Time
char ntpServer[] = "ntp.jst.mfeed.ad.jp";
const long gmtOffset_sec = 9 * 3600;
const int  daylightOffset_sec = 0;
struct tm timeinfo;
String dateStr;
String timeStr;

File file;

// 保存するファイル名
const char* fname = "/btnevent_log.csv";

// ボタン押下時に記録するイベント名
// CSVファイルは日本語が使えます。
char *eventName[] = { "Btn_A", "Btn_B", "Btn_C" };

void getTimeFromNTP(){
  // NTPサーバと時刻を同期
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!getLocalTime(&timeinfo)) {
    delay(1000);
  }
}

void getTime(){
  // 時刻の取得と表示
  getLocalTime(&timeinfo);
  dateStr = (String)(timeinfo.tm_year + 1900)
          + "/" + (String)(timeinfo.tm_mon + 1)
          + "/" + (String)timeinfo.tm_mday;
  timeStr = (String)timeinfo.tm_hour
          + "h" + (String)timeinfo.tm_min
          + "m" + (String)timeinfo.tm_sec
          + "s" ;

  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.setCursor(0, 50, 1);
  M5.Lcd.println(dateStr + "   ");
  M5.Lcd.println(timeStr + "   ");
}


void writeData(char *paramStr) {
  // SDカードへの書き込み処理（ファイル追加モード）
  // SD.beginはM5.begin内で処理されているので不要
  file = SD.open(fname, FILE_APPEND);
  file.println(dateStr + "," + timeStr + "," + paramStr);
  file.close();
}

void setup()
{
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.println("CSV Writer");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWiFi connected.");

  // timeSet
  getTimeFromNTP();

}

void loop()
{
  M5.update();
  // 時刻表示
  getTime();

  // カーソル位置の指定
  M5.Lcd.setCursor(0, 100, 1);

  // ボタンイベント処理
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.print("Button A Pressed");
    writeData(eventName[0]);
  }
  if (M5.BtnB.wasPressed()) {
    M5.Lcd.print("Button B Pressed");
    writeData(eventName[1]);
  }
  if (M5.BtnC.wasPressed()) {
    M5.Lcd.print("Button C Pressed");
    writeData(eventName[2]);
  }
}