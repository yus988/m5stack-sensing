

#include <M5Stack.h>
#include <Wire.h>

//アドレス指定
#define ADXL345_ADDR 0x53
#define DATA_FORMAT 0x31
#define POWER_CTL 0x2D
#define DATAX0 0x32
#define FULL_RES_16G 0x0B
#define BIT10_16G 0x03
#define MEASURE 0x08

void setup() {
  Serial.begin(115200);//シリアル通信を115200bpsで初期化
  Wire.begin();//I2Cを初期化
 
  Wire.beginTransmission(ADXL345_ADDR);//I2Cスレーブ「Arduino Uno」のデータ送信開始
  Wire.write(DATA_FORMAT);//データ・フォーマット・コントロール
  Wire.write(FULL_RES_16G);//「最大分解能モード」、「±16g」
  //Wire.write(BIT10_16G);//「10bit固定分解能モード」、「±16g」
  Wire.endTransmission();//I2Cスレーブ「Arduino Uno」のデータ送信終了
 
  Wire.beginTransmission(ADXL345_ADDR);//I2Cスレーブ「Arduino Uno」のデータ送信開始
  Wire.write(POWER_CTL);//省電力機能コントロール
  Wire.write(MEASURE);//測定モード
  Wire.endTransmission();//I2Cスレーブ「Arduino Uno」のデータ送信終了
}
 
void loop() {
  unsigned int dac[6];
  unsigned int i, x, y, z;
  float X_Axis, Y_Axis, Z_Axis;

  Wire.beginTransmission(ADXL345_ADDR);//I2Cスレーブ「Arduino Uno」のデータ送信開始
  Wire.write(DATAX0);//出力データバイトを「X軸データ0」のアドレスに指定
  Wire.endTransmission();//I2Cスレーブ「Arduino Uno」のデータ送信終了
  
  Wire.requestFrom(ADXL345_ADDR, 6);//I2Cデバイス「ADXL345」に6Byteのデータ要求
  for (i=0; i<6; i++){
    while (Wire.available() == 0 ){}
    dac[i] = Wire.read();//dacにI2Cデバイス「ADXL345」のデータ読み込み
  }

  x = (dac[1] << 8) | dac[0];//2Byte目のデータを8bit左にシフト、OR演算子で1Byte目のデータを結合して、xに代入
  X_Axis = float(x) * 0.0392266;//加速度の計算、X_Axisに代入 ※0.0392266=(4/1000*9.80665)
  y = (dac[3] << 8) | dac[2];//2Byte目のデータを8bit左にシフト、OR演算子で1Byte目のデータを結合して、xに代入
  Y_Axis = float(y) * 0.0392266;//加速度の計算、Y_Axisに代入
  z = (dac[5] << 8) | dac[4];//2Byte目のデータを8bit左にシフト、OR演算子で1Byte目のデータを結合して、xに代入
  Z_Axis = float(z) * 0.0392266;//加速度の計算、Z_Axisに代入
  
  Serial.print("X-Axis:");//文字列「X-Axis:」をシリアルモニタに送信
  Serial.print(X_Axis);//「X_Axis」をシリアルモニタに送信
  Serial.print("m/s^2 ");//文字列「m/s^2」をシリアルモニタに送信
  Serial.print("Y-Axis:");//文字列「Y-Axis:」をシリアルモニタに送信
  Serial.print(Y_Axis);//「Y_Axis」をシリアルモニタに送信
  Serial.print("m/s^2 ");//文字列「m/s^2」をシリアルモニタに送信
  Serial.print("Z-Axis:");//文字列「Z-Axis:」をシリアルモニタに送信
  Serial.print(Z_Axis);//「Z_Axis」をシリアルモニタに送信
  Serial.println("m/s^2");//文字列「m/s^2」をシリアルモニタに送信、改行
   
  delay(1000);//1000msec待機(1秒待機)
}
