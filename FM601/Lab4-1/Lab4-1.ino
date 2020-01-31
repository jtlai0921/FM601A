#include <Adafruit_NeoPixel.h>

// 建立燈條(燈環)物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

void setup() {
  strip.begin();                  // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);        // 將整體亮度降為 1/8
  strip.show();                   // 讓燈條顯示出所設定的狀態
}

void loop() {
  fadeInOut(255, 255, 255, 5);    // 顯示由暗到亮再由亮到暗,每次變化間隔5ms
}

// 由暗到亮再由亮到暗,每次變化間隔 delayMs 毫秒
void fadeInOut(int r, int g, int b, int delayMs) {
  for (long k = 0; k <= 255; k++) {     // 由最暗到最亮
    showAll(k*r/255, k*g/255, k*b/255); // 將亮度對映到指定的顏色
    delay(delayMs);                     // 暫停指定的時間
  }
  for (long k = 255; k >= 0; k--) {     // 由最亮到最暗
    showAll(k*r/255, k*g/255, k*b/255); // 同上個迴圈
    delay(delayMs);                     // 同上個迴圈
  }
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

