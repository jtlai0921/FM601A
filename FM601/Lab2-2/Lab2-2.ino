#include <Adafruit_NeoPixel.h>

// 建立燈條物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

void setup() {
  strip.begin();                  // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);        // 將整體亮度降為 1/8
  strip.show();                   // 讓燈條顯示出所設定的狀態
}

void loop() {
  colorWipe(255,   0,   0, 50);   // 依序亮紅燈
  colorWipe(  0, 255,   0, 50);   // 依序亮綠燈
  colorWipe(  0,   0, 255, 50);   // 依序亮藍燈
  colorWipe(255, 255, 255, 50);   // 依序亮白燈
  colorWipe(  0,   0,   0, 200);  // 依序熄燈 (慢速)
}

// 著色特效：依序顯示指定的rgb顏色, 每顯示一顆就暫停 waitMs 毫秒
void colorWipe(byte r, byte g, byte b, int waitMs) {
  for (unsigned i = 0; i < strip.numPixels(); i++) { // 由0開始依序點亮
    strip.setPixelColor(i, r, g, b);  // 點亮指定的顏色
    strip.show();                     // 將設定顯示出來
    delay(waitMs);                    // 暫停 waitMs 毫秒
  }
}

