#include <SPI.h>
#include <RFID.h>
#include <SimpleSDAudio.h>
/*
Arduino Uno ve diğer modüller ile SPI bağlantısı için:
MOSI:D11       SCK:D13
MISO:D12       SDA(RC522):D8 ve CS:D4(Sd Kart Modül)
RST:D10(RC522) 3.3V(RC522)
5V (Sd Kart Modül)
 
Arduino Mega ve diğer modülleri ile SPI bağlantısı için:
MOSI:D51       SCK:D52
MISO:D50       SDA(RC522):D9 ve CS:D53(Sd Kart Modül)
RST:D8(RC522) 3.3V(RC522)
5V (Sd Kart Modül)
 
*/
byte emaomos[5] = {37,160,40,82,255};//tanımlı olan kartınızın id si
RFID rfid(8,10); 
byte serNum[5];
byte data[5];
void setup()
{ 
  Serial.begin(9600);//Seri haberleşmeyi başlatır
  SPI.begin(); 
  //Hoparlörü Arduino Uno da 9.pine
  //Arduino Megada ise 44. pine takınız
  SdPlay.setSDCSPin(4);// sd kart cs pini
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER))
 {
 while(1); 
 } 
  
 if(!SdPlay.setFile("music.wav")) // müzik dosya adı
 {
 while(1); //dosya bulunamassa durdur
 } 
 digitalWrite(4,HIGH);
 delay(100);
 
  rfid.init();
  
}
 
void loop()
{
    
  boolean emaomos_card = true;
    if (rfid.isCard()) {
      
      if (rfid.readCardSerial()){
         delay(1000);
         data[0] = rfid.serNum[0];
         data[1] = rfid.serNum[1];
         data[2] = rfid.serNum[2];
         data[3] = rfid.serNum[3];
         data[4] = rfid.serNum[4];
        }
  
      for(int i=0; i<5; i++){
        if(data[i]!= emaomos[i]) emaomos_card = false;
          }
        if (emaomos_card){ // kartın verileri eşleşiyor ise
          Serial.println("Kart eslesti"); 
          SdPlay.play();//Müzik çalma komutu
         delay(5000); //5 saniye boyunca çalar
         SdPlay.stop();//Müzik durdurma komutu
          /*
          while(!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
 ; 
 }
 */  
    }
    else{
      Serial.println("Kart eslesmedi");
      }
    
    rfid.halt();
}
}
