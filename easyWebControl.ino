#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "pitches.h"

#define ssid      "xxx"       // WiFi SSID
#define password  "xxx"  // WiFi password
#define PIN_MUSIC D8
String  etatmusic = "OFF";

// Création des objets 
ESP8266WebServer server ( 80 );

void playMusic(const int noteNumber, const int speedNote, const int noteDurations[], const int melody[]){
    for (int thisNote = 0; thisNote < 11; thisNote++) {
      int noteDuration = speedNote / noteDurations[thisNote];
      tone(PIN_MUSIC, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(PIN_MUSIC);
    }
}

void playMusicVictory(){
  const int notesVictory[] = {  NOTE_ZERO, NOTE_C6 ,NOTE_C6,NOTE_C6,NOTE_C6, NOTE_GS5 , NOTE_AS5 , NOTE_C6, NOTE_ZERO , NOTE_AS5 , NOTE_C6};
  const int rythmeVictory[] = { 2,12,12,12,4,4,4,12,16,12,1 };
  const int speedNoteVictory = 1500;

  playMusic(sizeof(notesVictory)/sizeof(int), speedNoteVictory, rythmeVictory,  notesVictory);
}

void playMusicDefeat(){
  const int notesDefeat[] = { NOTE_B4, NOTE_F5,NOTE_ZERO ,NOTE_F5,NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5,NOTE_E4,NOTE_ZERO, NOTE_E4 , NOTE_C4 };
  const int noteDurations[] = { 4,4,4,4,3,3,3,4,4,4,4,4 };
  const int speedNote = 500;

  playMusic(sizeof(notesDefeat)/sizeof(int), speedNote, noteDurations,  notesDefeat);
}

String getPage(){
  String page = "<html lang=fr-FR><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>Music Demo</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>Music Demo</h1>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>Music (etat: ";
  page += etatmusic;
  page += ")";
  page += "<INPUT type='radio' name='MUSIC' value='1'>Victory";
  page += "<INPUT type='radio' name='MUSIC' value='0'>Defeat</li></ul>";
  page += "<INPUT type='submit' value='Actualiser'>";
  page += "</body></html>";
  return page;
}
void handleRoot(){ 
  if ( server.hasArg("MUSIC") ) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}

void handleSubmit() {
  String MUSICValue;
  MUSICValue = server.arg("MUSIC");
  Serial.print(MUSICValue);
  if ( MUSICValue == "1" ) {
    playMusicVictory();
    etatmusic = "Winner";
    server.send ( 200, "text/html", getPage() );
  } else if ( MUSICValue == "0" ) {
    playMusicDefeat();
    etatmusic = "Loser";
    server.send ( 200, "text/html", getPage() );
  } else {
    Serial.println("Err");
  }
}

void setup() {
  Serial.begin ( 115200 );  
  WiFi.begin ( ssid, password );
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" ); 
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  // On branche la fonction qui gère la premiere page / link to the function that manage launch page 
  server.on ( "/", handleRoot );

  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
  delay(1000);
}
