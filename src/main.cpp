//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12, /* reset=*/ 8);
#include <main.h>
static CAN_message_t msg;
boolean coueleurChoie=false,premieerEcrant0=true,premieerEcrant1=true,canEcrant0=false;
byte transion,encodeurPos=0,encodeurPosP=1,Strategie=0,strategieBicolor,sensRotationEncodeur;
byte ecrant=0,nb=0;
int timerRotationEncodeur=0,timerBpEncodeur,bpEncodeur=0;
boolean messageRecalage=false,passeEcrant3=false,passeEcrant31=true,passeEcrant4=true,passeEcrant2=true;
char couleurCanRecu='n';
String st="ez";
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12);
void setup() {
  Can0.begin(1000000);
  u8g2.begin();
  Serial.begin(9600);
  MsTimer2::set(1,IntrerrupTimer);
  MsTimer2::start();
  pinMode(BOUTON_1,INPUT_PULLUP);
  pinMode(ENCODEUR_BP,INPUT_PULLUP);
  pinMode(ENCODEUR_A,INPUT_PULLUP);
  pinMode(ENCODEUR_B,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_BP),bpInterupEncodeur,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_A),tournInterupEncodeurRisingA,RISING);
  delay(100);
  msg.id=0x006;
  msg.len=3;
  msg.buf[0]='d';
  msg.buf[1]='s';
  msg.buf[2]=0;
  Can0.write(msg);
}
void loop() {
   while(Can0.read(msg)){
     interpretationCan();
  }
  switch (ecrant) {
    case 0:
      ecrant=ecrant_0();
    break;
    case 1:
      ecrant=ecrant_1();
    break;
    case 2:
    ecrant=ecrant_2();
    break;
    case 3:
    ecrant=ecrant_3();
    break;
    case 4:
    ecrant=ecrant_4();
    break;
  }
}


byte ecrant_0(){
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant0||canEcrant0){
    if (encodeurPos>2) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=2;
    encodeurPosP=encodeurPos;
    if(encodeurPosP==0 && bpEncodeur==1){
      Strategie++;
      if(Strategie >= 10)Strategie=0;
      msg.id=0x006;
      msg.len=3;
      msg.buf[0]='d';
      msg.buf[1]='s';
      msg.buf[2]=Strategie;
      Can0.write(msg);
    }
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0, encodeurPos*10, 128, 11);
    u8g2.setDrawColor(2);
    u8g2.setCursor(64,9);
    u8g2.print(Strategie);
    u8g2.drawStr(0,9,"Strategie");
    if(couleurCanRecu=='j')u8g2.drawStr(64,19,"jaune");
    else if(couleurCanRecu=='b')u8g2.drawStr(64,19,"bleu");
    else if(couleurCanRecu=='c'){
      if (bpEncodeur==1&&encodeurPos==1)coueleurChoie=!coueleurChoie;
      if (coueleurChoie)u8g2.drawStr(64,19,"jaune");
      else u8g2.drawStr(64,19,"bleu");
    }
    u8g2.drawStr(0,19,"couleur");
    u8g2.drawStr(0,29,"Valider");
    u8g2.sendBuffer();					// transfer internal memory to the display
    strategieBicolor=coueleurChoie;
    if(couleurCanRecu!='c')strategieBicolor=2;
    msg.id=0x006;
    msg.len=4;
    msg.buf[0]='R';//retour
    msg.buf[1]='c';//courluer
    msg.buf[2]='s';//strat
    msg.buf[3]=strategieBicolor;
    Can0.write(msg);
    if(encodeurPos==2&&bpEncodeur){
      transion=1;
      bpEncodeur=1;
    }
    else transion=0;
    canEcrant0=false;
    bpEncodeur=0;
    premieerEcrant0=false;
    premieerEcrant1=true;
  }

  return transion;
}

byte ecrant_1(){
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant1){
    if (encodeurPos>1) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=1;
    encodeurPosP=encodeurPos;
    if(encodeurPos==0 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=3;
      msg.buf[0]='S';//starte
      msg.buf[1]='r';//recalage
      msg.buf[2]='d';//debue
      Can0.write(msg);
    }
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    if(encodeurPos==0)u8g2.drawBox(0, 40, 64, 11);
    else if(encodeurPos==1)u8g2.drawBox(64,40, 64, 11);
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Vous avez choisie ");
    u8g2.drawStr(0,19,"  la strategie : ");
    u8g2.drawStr(0,29,"Et vous ete sur le cote : ");
    u8g2.drawStr(0,49,"    valider     changer");
    u8g2.setCursor(100,19);
    u8g2.print(Strategie);
    if(couleurCanRecu=='j')u8g2.drawStr(100,39,"jaune");
    else if(couleurCanRecu=='b')u8g2.drawStr(100,39,"bleu");
    else if(couleurCanRecu=='c'){
      if (coueleurChoie)u8g2.drawStr(100,39,"jaune");
      else u8g2.drawStr(100,39,"bleu");
    }
    u8g2.sendBuffer();					// transfer internal memory to the display

    if(encodeurPos==1&&bpEncodeur)transion=0;
    else if(encodeurPos==0&&bpEncodeur)transion=2;
    else transion=1;
    bpEncodeur=0;
    premieerEcrant1=false;
    premieerEcrant0=true;
  }
  return transion;
}

byte ecrant_2(){
  if(passeEcrant2){
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Recalage ");
    u8g2.sendBuffer();					// transfer internal memory to the display
    passeEcrant2=!passeEcrant2;
  }
  if(messageRecalage)transion=3;
  else transion=2;
  return transion;

}

byte ecrant_3(){
//  Serial.println("test");
  if(bpEncodeur&& !digitalRead(BOUTON_1)){
    passeEcrant3=true;

    Can0.write(msg);
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Recalage fin");
    u8g2.drawStr(0,19, "tirete valider");
    Serial.println("truc");
    u8g2.sendBuffer();					// transfer internal memory to the display
    bpEncodeur=0;
  }
  if(passeEcrant3&&digitalRead(BOUTON_1)){
    msg.id=0x006;
    msg.len=3;
    msg.buf[0]='G';//starte
    msg.buf[1]='d';//recalage
    msg.buf[2]='m';//debue
    Can0.write(msg);
    bpEncodeur=0;
    Serial.println("azazz");
    return 4;
  }
  if(passeEcrant31){
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Recalage fin");
    u8g2.sendBuffer();					// transfer internal memory to the display
    bpEncodeur=0;
    passeEcrant31=!passeEcrant31;
  }
  return 3;
}

byte ecrant_4(){
  if(passeEcrant4){

    Can0.write(msg);
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "en match");
    u8g2.sendBuffer();					// transfer internal memory to the display
    passeEcrant4=!passeEcrant4;
  }
  return transion=4;
}

void bpInterupEncodeur(){
  if(timerBpEncodeur > 100){
    if(transion==0||transion==1||transion==3)bpEncodeur=1;
    timerBpEncodeur=0;
  }
}
void tournInterupEncodeurRisingA(){
  if(timerRotationEncodeur > 100){
    encodeurPos ++;
      timerRotationEncodeur=0;
  }
}
void interpretationCan(){
  if (comparChenCan("Rds")){
    couleurCanRecu=msg.buf[3];
    canEcrant0=true;
  }
  if(comparChenCan("Drf")){
    messageRecalage=!messageRecalage;
  }
  if(comparChenCan("Ptf")){
    Serial.println("tesetests");
    transion=90;
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.print(msg.buf[3]);
    u8g2.sendBuffer();					// transfer internal memory to the display
  }
}
int comparChenCan(String comp){
  for(int i=0;i<9;i++){
    if(comp[i]=='\0')return 1;
    if(msg.buf[i]!=comp[i]) return 0;
  }
  return 0;
}
void IntrerrupTimer(){
  timerBpEncodeur++;
  timerRotationEncodeur++;
}
