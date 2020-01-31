#include <Adafruit_NeoPixel.h>

// 建立燈條(燈環)物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

void setup() {
  strip.begin();             // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(64);   // 將整體亮度降為 1/4
  strip.show();              // 讓燈條顯示出所設定的狀態
}

void loop() {
  breathing(0, 255, 0);    // 顯示綠色的呼吸燈
}

// 曲線型變化的呼吸特效
void breathing(int r, int g, int b) {
  float bright = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
  bright = bright / 255;     // 先除 255, 然後下一行再乘以指定的 3 原色值
  showAll(bright * r, bright * g, bright * b);
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

