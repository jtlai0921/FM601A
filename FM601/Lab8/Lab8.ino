#include <Adafruit_NeoPixel.h>
#include "Note.h"

// 【歌譜】結構： "音符串","節拍串","速度(BPM,拍/分鐘)"  (字串中的空白會忽略, BPM若不知道先可設為120再慢慢調整)
// 音符串： 1~7:Do~Si, -:連音(延長音), 0:休止符, #:升半音, ^/&:高/低八度(單音), a/d:高/低八度(永久)
// 節拍串： m表"1/m"拍(例如2表1/2拍), m<n>:表示連續n個都是m (在音符串中每個"0~7"及"-"字元,都必須有對應的拍子)
const char *demo[] =  { "1d556507a1",   // 音符串
                            "2 442222 2",   // 節拍串
                            "120" };        // 速度(BPM)
const char *bee[] =  {"533- 422- 1234 555- 533- 422- 1355 3--0 2222 234- 3333 345- 533- 422- 1355 1--0",
                            "1<99>",        // 拍子全部為 1
                            "150" };
const char *star[] = {"1155 665- 4433 221- 5544 332- 5544 332- 1155 665- 4433 221-", 
                            "1<99>",        // 拍子全部為 1
                            "130" };
const char *tiger[] =  { "1231 1231 345- 345- 565431 565431 1&51- 1&51-",
                            "1<16>"             "222211 22221<10>",
                            "150" };
const char *dunkey[] = { "1113 5555 666^1 5- 4446 3333 2222 5-5 1113 5555 666^1 5- 4446 3333 2223 1-",
                         "2222 2222 222 2 11 2222 2222 2222 122 2222 2222 222 2 11 2222 2222 2222 11", 
                         "100" };
const char *doll[] =   { 
  "11&5- 113- 5311 2--0 / 3321 d665- 65^17 a1--0 / 22&5- 113- 5313 5--- / 665- 315- 311&5&5 1--0",
  "1<56>"                                                                          "122 1 1 1111", 
  "180" }; 
const char *flower[] = { 
  "53235- 356^15- 55^1653 25321- 116553 21235- 55^1653 25321- 55321235 21&621- 5111&61 5111&61 5151 51511 &62111&61",
  "222211 222 211 21 2122 222211 212122 222211 21 2122 222211 14242222 22 2211 1222 21 1222 21 1111 12211  22222 21",
  "120" };
  
const int songNum = 7;            // 歌曲總數
const char **song[songNum] =      // 歌本陣列
            { demo, bee, star, tiger, dunkey, doll, flower };

// 建立燈條(燈環)物件, 參數為：共16顆LED, 資料線接在Pin6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 6);

const int buzzerPin = 8;      // 蜂鳴器接在 A1

void setup() {
  strip.begin();              // 將燈條物件初始化, 並設為全暗
  strip.setBrightness(32);    // 將整體亮度降為 1/8
  strip.show();               // 讓燈條顯示出所設定的狀態
}

void loop() {
  for(int i=0; i<songNum; i++) {
    playSong(i);      // 播放第 i 首歌曲
    delay(1000);      // 暫停 1 秒
  }
}

// 播放第 no 首歌曲
void playSong(int no) {
  int ms, nt;
  int lev = 5;                        // 預設音階 (預設為 Level 5)
  int addLev=0;                       // 音階增量 (預設為 0)
  const char *noteStr = song[no][0];  // 宣告變數指到音符字串 (以便後續取存音符字串中的字元)
  int noteLen = strlen(noteStr);      // 音符字串的長度
  int effect = getRandomLedEffect();  // 取得一個隨機的 LED 特效編號 (0~2)
  
  setNoteDuration(song[no][1], atoi(song[no][2]));  // 設定節拍,以便未來取得每個音符的節拍
  
  for (int i = 0; i < noteLen; i++) { // 依序解析音符字串中的每一個字元
    switch (noteStr[i]) {
      case 'a': lev++; break;         // 升一階(永久)
      case 'd': lev--; break;         // 降一階(永久)
      case '^': addLev = 1; break;    // 升一階(單音)
      case '&': addLev =-1; break;    // 降一階(單音)
      case '0':                       // 休止符
        noTone(buzzerPin);            // 靜音
        showAll(0, 0, 0);             // 清除 LED
        ms = getNoteDuration();       // 取得對應的節拍時間(毫秒)
        delay(ms);                    // 暫停節拍時間
        break;
    }
    if (noteStr[i] >= '1' && noteStr[i] <= '7') {  // 若為音符1~7,則播放之
      nt = noteStr[i] - '1';        // 儲存音符數值 (0~6)
      ms = getNoteDuration();       // 取得節拍長度(毫秒)
      
      while(noteStr[i+1] == '-' || noteStr[i+1] == ' ') { // 若有連音字元或空白(空白要忽略)
        if(noteStr[i+1] == '-') {   // 若為連音字元
          ms += getNoteDuration();  // 取得其節拍長度,並加到原來的節拍長度中
        }
        i++;                        // 指向下一個字元
      }
      
      if (i > 0 && noteStr[i - 1] == '#') { // 若前一字元為升半音 (#會加在音符字元的前面,例如：#5)
        tone(buzzerPin, noteShift[lev+addLev][nt], ms * 0.8);  // 以升半音播放音符,時間為節拍長度的8成
      }
      else {
        tone(buzzerPin, note[lev+addLev][nt], ms * 0.8);       // 以正常音播放音符,時間為節拍長度的8成
      }
      showLedEffect(effect, nt+1);          // 顯示特定編號的LED特效 (共有3種)
      delay(ms);                            // 暫停節拍長度的時間 (前8成有聲,後2成無聲(靜音))
      addLev = 0;                           // 將音階增量歸 0 (因為這個增量只對目前音符有效)
    }
  }
  showAll(0, 0, 0);   // 播完一首之後, 將 LED 設為全暗
}

// 設定與計算歌曲的節拍資訊
const char *beatStr;
int beatPos, beatLen, tempo;
int beatRepeat, beatRepeatCnt;
void setNoteDuration(const char *beat, int bpm) {
  beatStr = beat;
  beatLen = strlen(beatStr);
  beatPos = -1;
  beatRepeatCnt = 0;
  
  if(bpm <= 0) bpm = 120;   // bpm 為 1 分鐘(6000毫秒)有幾拍
  tempo = 60000 / bpm;      // 計算一拍有多少毫秒
}

// 取得節拍字串中的下一個節拍長度(毫秒)
int getNoteDuration() {
  if (beatRepeatCnt > 0) {  // 如果之前有有用m<n>指定n個m, 且n尚未讀完
    beatRepeatCnt--;        // 將計數值減 1
    return beatRepeat;      // 直接傳回 m
  }
  while (beatPos < beatLen) { // 找出、計算、並傳回下一個節拍長度(毫秒)
    beatPos++;                // 指向節拍字串中的下一個字元
    if (beatStr[beatPos] >= '0' && beatStr[beatPos] <= '9') { // 只處理0~9的節拍字元, 其他字元略過
      int ms = tempo/(beatStr[beatPos]-'0');  // 計算節拍長度(毫秒), 例如'2'則 ms = (一拍的毫秒數)/2 
      if (beatStr[beatPos+1] == '<') {        // 如果下一字元為<, 表示有使用m<n>格式指定n個m
        beatPos += 2;                         // 將節拍字元位置指到<的下一字元 (以便讀取n值)
        beatRepeatCnt = 0;                    // 先將 n 值歸 0
        while(beatStr[beatPos] >= '0' && beatStr[beatPos] <= '9') {     // 由於n可能有1~3個數字,要依序解析
          beatRepeatCnt = beatRepeatCnt*10 + (beatStr[beatPos] - '0');  // 將舊值乘10再加新值
          beatPos++;                                                    // 指向下一個字元
        }
        beatRepeatCnt--;  // 會先傳回第1個, 因此計數值(n)要減 1
        beatRepeat = ms;  // 記住要重複的節拍時間(毫秒)
      }
      return ms;          // 傳回目前音符的節拍時間(毫秒)
    }
  }
  return 0;   // 若超過節拍字串長度, 則傳回0毫秒
}

// 取得一個隨機的 LED 特效編號 (0~2)
int getRandomLedEffect() {
  static int oldEffect;         // 記錄前一次的LED特效編號 (以避免連續使用相同的特效)
  while(true) {
    int effect = random(0, 3);  // 取得介於 0~2 的隨機整數, 做為要顯示的LED特效編號
    if (effect != oldEffect) {  // 如果和前次不同才要, 否則就重取亂數
      oldEffect = effect;       // 儲存本次的特效編號 (以便下次呼叫本函式時會用它做比較, 避免重複)
      return effect;            // 傳回取得的亂數
    }    
  }
}

// 顯示 LED 特效 (每播一個音符就要呼叫本函式一次)
void showLedEffect(int effect, int note) {
  int r, g, b;
  do {
    r = random(0, 256);  // 取得隨機紅色值
    g = random(0, 256);  // 取得隨機綠色值
    b = random(0, 256);  // 取得隨機藍色值
  } while(r + g + b < 128);  // 若太暗則重取
  
  switch (effect) {             // 依編號顯示不同的特效
    case 0: // 由第 0 顆開始, 點亮指定數量的 LED
            showSome(r, g, b, note*2);  
            break;
    case 1: // 由隨機位置開始, 點亮指定數量的 LED
            showSeg (r, g, b, random(0, 16), note*2-1); 
            break;
    case 2: // 顯示療癒燈特效 (見第 7 章)
            showCure(r, g, b, 50); 
            break;
  }
}

// 點亮所有 LED, 並顯示出來
void showAll(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();    // 讓燈條顯示出來
}

// 由第 0 顆開始, 點亮指定數量的 LED
void showSome(int r, int g, int b, int pixelNum) {
  for (int i = 0; i < strip.numPixels(); i++) {
    if (i < pixelNum)   
      strip.setPixelColor(i, r, g, b);  // 在指定長度內就點亮
    else
      strip.setPixelColor(i, 0, 0, 0);  // 在指定長度外則熄滅
  }
  strip.show();
}

// 由指定位置開始, 點亮指定數量的 LED
void showSeg(int r, int g, int b, int pos, int pixelNum) {
  for (int i = 0; i < strip.numPixels(); i++) {
    if (pixelNum > 0)                     // 由 pos 位址開始
      strip.setPixelColor(pos, r, g, b);  // 在指定長度內就點亮
    else 
      strip.setPixelColor(pos, 0, 0, 0);  // 在指定長度外則熄滅
    pos++;                        // 位址加1
    pixelNum--;                   // 長度減1
    if (pos >= strip.numPixels()) // 如果位移到最後了, 則移到第0顆
      pos = 0;
  }
  strip.show();
}

// 顯示療癒燈特效 (見第 7 章)
void showCure(int r, int g, int b, int runDelay) {
  static int runPos = 0;
  static unsigned long runTimer;

  if (millis() - runTimer > runDelay) {           // 若延遲時間到了
    runTimer = millis();                          // 重設計時器
    runPos++;                                     // 移到下一顆 LED 開始顯示
    if (runPos >= strip.numPixels()) runPos = 0;  // 若移出最後顆則移回第1顆
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
