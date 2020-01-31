#include <Adafruit_NeoPixel.h>

// 建立燈條(燈環)物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

const int photocellPin = A1;      // 光敏電阻接在 A1

void setup() {
  strip.begin();                  // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);        // 將整體亮度降為 1/8
  strip.show();                   // 讓燈條顯示出所設定的狀態
}

void loop() {
  autoLamp(255,255,255, 200,750);  // 安全燈：白色,光敏範圍200~750
}

// 自動安全燈函式, 參數為：rgb色彩, 燈由最亮到關掉的光敏範圍(min~max)
void autoLamp(int r, int g, int b, int minVin, int maxVin) {
  int bright;
  unsigned rb = 0;                  // rb 為安全燈實際輸出的亮度

  while (true) {
    int vin = analogRead(photocellPin);         // 讀取光敏電阻的感測值
    bright = map(vin, minVin, maxVin, 255, 0);  // 將光敏值對映到LED亮度值
    bright = constrain(bright, 0, 255);         // 限制亮度介於0~255

    if (rb < bright) rb++;          // rb每次只加/減1, 以產生漸變的效果
      else if (rb > bright) rb--;     
    delay(3);                       // 漸變的延遲時間(ms), 值越大漸變越慢
    showAll(rb*r/255, rb*g/255, rb*b/255);
  }
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

