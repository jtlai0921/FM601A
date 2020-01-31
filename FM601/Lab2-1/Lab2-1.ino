#include <Adafruit_NeoPixel.h>

// 建立燈條物件, 參數為：共15顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, 6);

void setup() {
  strip.begin();                     // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);           // 將整體亮度降為 1/8
  strip.setPixelColor(3, 255, 0, 0); // 點紅第3顆LED(由0算起)
  strip.show();                      // 讓燈條顯示出所設定的狀態
}

void loop() {
  // 留空
}
