#include <Adafruit_NeoPixel.h>

// 建立燈條(燈環)物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

const int thermistorPin = A0;     // 熱敏電阻接在 A0

void setup() {
  strip.begin();                  // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);        // 將整體亮度降為 1/8
  strip.show();                   // 讓燈條顯示出所設定的狀態
}

void loop() {
  coaster(520, 50); // 顯示熱感杯墊, 開始變化的熱度520, 熱度每多50速度加一倍
}

// 熱感杯墊, 參數：開始變化的熱度、熱度每加多少則速度加一倍
void coaster(int base, int speedBase) {
  float red, green, faster;
  while (true) {
    int vin = analogRead(thermistorPin);  // 讀取熱敏電阻的感測值
    red = constrain(vin - base, 0, 255);  // 計算紅色的亮度(必須介於 0~255)
    faster = red / speedBase;             // 計算呼吸加速率
    if (faster <= 1) faster = 1;          // 加速率不得小於 1
    green = (255 - red);                  // 綠色亮度 = 255 - 紅色亮度

    // 使用自訂的myMillis(速度)來計算呼吸燈的亮度, 
    //     再對映到紅、綠的亮度, 藍的亮度固定為0
    float bright = (exp(sin(myMillis(faster)/2000.0*PI))-0.36787944)*108.0;
    bright = bright / 255;     // 先除 255, 然後下一行再乘以指定的 3 原色值
    showAll(bright*red, bright*green, 0);
  }
}

// 依照指定的速度(faster) 調整 millis() 的傳回值, 並將調整值傳回
unsigned long myMillis(float faster) {
  static unsigned long lastMs = millis();  // 儲存上一次讀取的 millis() 值
  static unsigned long myMs = 0;           // 儲存要傳回的毫秒值
  static float sum = 0;                    // 儲存要傳回的增量值
  
  unsigned long ms = millis();  // 讀取 millis() 毫秒值
  if(ms != lastMs) {            // 如果距上次計算增量已超過1ms, 就重新計算
    sum += faster*(ms-lastMs);  // 計算增量值sum(=sum + faster*已過多少毫秒)
    lastMs = ms;                // 記錄最後一次計算增量的時間 (毫秒值)

    // 由於sum為float型別, 只有6位有效數字, 因此要避免加到太大, 否則會出錯
    if(sum > 1000) {  // 當 sum 大於 1000 時
      myMs += sum;    // 將 sum 轉為長整數加到 myMs 中
      sum = 0;        // 再將 sum 歸 0  (以避免 sum 加到太大)
    }
  }
  return myMs + sum;  // 傳回 (毫秒值+增量值)
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

