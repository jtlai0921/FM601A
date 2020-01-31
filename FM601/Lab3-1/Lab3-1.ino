#include <Adafruit_NeoPixel.h>

// 建立燈條物件, 參數為：共15顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, 6);

void setup() {
  strip.begin();                  // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);        // 將整體亮度降為 1/8
  strip.show();                   // 讓燈條顯示出所設定的狀態
}

void loop() {
  meteor(255, 255, 255, 0, 6);  // 由位置 0 開始, 顯示長度為 6 的白色流星
  delay(5000);                      // 暫停 5 秒
  meteor(255, 128,   0, 3, 9);  // 由位置 3 開始, 顯示長度為 9 的橘色流星
  delay(5000);                      // 暫停 5 秒
}

void meteor(int r, int g, int b, int pos, int len) {
  int n     = 255 / ((1+len)*len/2);      // 算出梯形的 n
  int bright= 255 % ((1+len)*len/2);      // 算出餘數做為初始亮度
  int cnt = 1;                            // 計數 (由第 1*n 開始計算增量)
  strip.clear();                          // 先清為全暗
  for (int i = pos; i < pos+len; i++) {   // 由最暗到最亮
    bright += cnt*n;                      // 計算新亮度 (= 原亮度+增量)
    cnt++;                                // 將計數加 1
    if(i>=0 && i<(int)strip.numPixels()) {// 位置在燈條的範圍內才點亮
      strip.setPixelColor(i,              // 點亮目前位置的 LED
            map(bright, 0, 255, 0, r),    // 利用 map() 做紅色的範圍對映
            map(bright, 0, 255, 0, g),    // 利用 map() 做綠色的範圍對映 
            map(bright, 0, 255, 0, b));   // 利用 map() 做藍色的範圍對映
    }
  }
  strip.show();  // 將設定實際顯示出來
}


