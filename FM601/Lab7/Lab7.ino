#include <Adafruit_NeoPixel.h>

// 建立燈環物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

const int micPin = A2;  // 音量感測器接在 A2 腳位

void setup() {
  strip.begin();            // 將燈環物件初始化, 並設為全暗
  strip.setBrightness(32);  // 將整體亮度降為 1/8
  strip.show();             // 讓燈環顯示出所設定的狀態
}

void loop() {
  // 顯示療癒燈特效, 最低喚醒音量變化值120, 繞圈速度間隔50ms
  cureLamp(120, 50);
}

// 療癒燈特效, 參數為：最低喚醒音量變化值, 繞圈速度間隔
// 間隔越短, 繞圈速度越快
void cureLamp(int activeBase, int runDelay) {
  char state = 'a';  // 儲存目前狀態, 預設為對話狀態

  int red = 0;    // 紅色的亮度
  int green = 0;  // 綠色的亮度
  int gRest = 0;  // 休息狀態時的綠色亮度

  int timeRest = 0;  // 儲存休息狀態的時間
  int runDelayRest;  // 休息狀態的繞圈速度間隔

  unsigned long ms;          // 儲存目前時間
  unsigned long restTimer;   // 進入休息狀態的計時器
  unsigned long sleepTimer;  // 進入睡眠呼吸狀態的計時器

  int cnt = 0;       // 讀取音量的計數器
  int vin;           // 目前的音量值
  int vmin = 10000;  // 音量最小值
  int vmax = 0;      // 音量最大值
  int diff;          // 音量差異值

  sleepTimer = restTimer = millis();
  while (true) {

    // ●讀100次, 取(最大值-最小值)做為音量變化值
    vin = analogRead(micPin);  // 讀取目前的音量值
    // 與之前最大最小值比對, 若超過則取代之
    if (vin > vmax) vmax = vin;
    if (vin < vmin) vmin = vin;

    cnt ++;
    if (cnt >= 100) {    // 讀100次之後進行處理
      diff = vmax - vmin;
      if (diff > 255) diff = 255;

      // 紅色亮度會一直遞減10, 但是不得小於diff
      red -= 10;
      if (red <= diff)
        red = diff; 
      green = 255 - red;

      // 音量變化大的話便進入對話狀態
      if (diff >= activeBase) {
        state = 'a';
        restTimer = millis();  // 重設計時器
      }

      vmax = 0; vmin = 10000;
      cnt = 0;
    }

    ms = millis();  // 紀錄目前時間
    
    // ●對話狀態
    if (state == 'a') {      
      // red越大showCureDelay越少(繞圈速度越快)
      int showCureDelay = map(red, 0, 255, runDelay, 20);
      showCure(red, green, 0, showCureDelay); 

      // 未偵測到大的音量變化超過一段時間便將進入休息狀態
      if (ms - restTimer > 3000) {
        state = 'b';
        gRest = green;
        timeRest = 1;
        runDelayRest = map(red, 0, 255, runDelay, 20);
        sleepTimer = ms;
      }
    }

    // ●休息狀態
    else if (state == 'b') { 
      if (ms - sleepTimer > timeRest) {
        sleepTimer = ms;
        gRest--;  // 綠光逐漸減弱
        timeRest++;  //休息時間逐漸拉長
        // 綠光減弱到消失後便進入等待進入睡眠呼吸狀態
        if (gRest == 0)
          state = 'c';
      }
      showCure(0, gRest, 0, runDelayRest);
    }

    // ●等待進入睡眠呼吸狀態
    else if (state == 'c') {  
      // 超過一段時間後將進入睡眠呼吸狀態
      if (ms - sleepTimer > 3000) {
        if ((exp(sin(ms / 2000.0 * PI)) - 0.36787944) * 108.0 < 5)
          state = 'd';
      }
    }

    // ●睡眠呼吸狀態
    else {
      int val = (exp(sin(ms / 2000.0 * PI)) - 0.36787944) * 108.0;
      showAll(0, 0, val);
    }
  }
}

// 療癒燈用來顯示繞圈燈光的函式
void showCure(int r, int g, int b, int runDelay) {
  static int runPos = 0;  // 儲存繞圈燈光的起始位置
  static unsigned long runTimer;

  if (millis() - runTimer > runDelay) {  // 若延遲間隔時間到了
    // 移到下一顆 LED 開始顯示, 以便產生繞圈跑動的動態效果
    runPos++;                            
    if (runPos >= strip.numPixels())  // 若超出最後顆則移回第1顆
      runPos = 0;
    runTimer = millis();
  }

  strip.clear();
  int pos = runPos;
  for (int i = 0; i < 1 + (r / 20); i++) {  // 越紅則點亮越多顆 LED
    strip.setPixelColor(pos, r, g, b);
    pos++;
    if (pos >= strip.numPixels()) pos = 0;
  }
  strip.show();
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈環顯示出來
}
