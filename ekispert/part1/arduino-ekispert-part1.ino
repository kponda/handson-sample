// ヘッダファイル指定　Including header files
#include <math.h>
#include <SakuraIO.h>

// 変数の定義　Definition of variables
SakuraIO_I2C sakuraio;
const int B = 4250;               // B value of the thermistor
const long R0 = 100000;           // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
uint32_t cnt = 0;

// 起動時に1回だけ実行　Run once at startup
void setup() {
  Serial.begin(9600);
  Serial.print("Waiting to come online");
  for (;;) {
    if ( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

// 以下ループ実行　Loop execution
void loop() {

  // cnt値のカウントアップ　Count up cnt value
  cnt++;
  Serial.println(cnt);

  // 温度情報の取得　Get temperature
    int a = analogRead(pinTempSensor);

    float R = 1023.0/a-1.0;
    R = R0*R;

    float temp = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet

    Serial.print("Temperature :");
    Serial.println(temp);

  // さくらの通信モジュールへの各値のキューイング　Queuing each value to module
  if (sakuraio.enqueueTx(0, cnt) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(1, temp) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }

  // キューイングされた値の送信　Sending queued values
  sakuraio.send();
  delay(100);

  // 利用可能な領域（Available）とデータが格納された領域（Queued）の取得　Get Available and Queued
  uint8_t available;
  uint8_t queued;
  if (sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE) {
    Serial.println("[ERR] get tx queue length error");
  }
  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);

  // 待機時間の調整　Adjust wait time
  delay(5000);
}