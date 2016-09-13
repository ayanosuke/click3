void setup() {
  pinMode(A2,INPUT_PULLUP);
  Serial.begin(57600);
  Serial.println("ok."); 
}

void loop() {
  ButtonState2();
}
void ButtonState2(){
  static int state = 0;
  static char bufp = 0;
  static uint32_t Ti;
  static char cKeybuf[20];
  
  char pc = 0;
  char ps = 0;
  char i;
  char di;
  char Max = 10;

  switch(state){
    case 0:
            Ti = millis();
            state = 1;
            break;
    case 1:                                       // 50msec周期の監視
            if( millis()-Ti >50){
              di = digitalRead(A2);               // ボタン読み込み
              if(di == 0 || bufp > 0) {           // 最初の検出か？ bufpが0以上
                cKeybuf[bufp++] = di;
                if(bufp >= Max){            
                  state = 2;
                  bufp = 0;
                  break;
                }
              }
              Ti = millis();
            }
            break;
    case 2:                                     // シングルクリック・ダブルクリック・長押し判定
            Serial.print("Keybuf:");  
            for(i=0;i<=Max-1;i++)
              Serial.print(cKeybuf[i],DEC);
            Serial.println("");
                  
            for( i = 0 ; i <= Max - 2 ; i++){   // 変化点を変数pcでカウント
              if(cKeybuf[i] != cKeybuf[i+1])
                pc = pc + 1;
              ps = ps + cKeybuf[i];             // 0が多いと長押し判定に使える
            }

            Serial.print(pc,DEC);
            Serial.print(":");   
            Serial.println(ps,DEC);

            switch(pc){
              case 0:Serial.println("Long press");break;
              case 1:
              case 2:if(ps<=5)
                        Serial.println("Long press");
                     else
                        Serial.println("single click");
                     break;
              case 3:
              case 4:Serial.println("Double click");break;
            }
            Serial.println("");
            state = 0;
            break;
    default:
            break;
  }  
}
