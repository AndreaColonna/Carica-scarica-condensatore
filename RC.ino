// C++ code
//
/*
  Carica e scarica di un condensatore
  creato 30/04/2022
  by Andrea Colonna
*/
#include <phyphoxBle.h>      // libreria per la comunicazione tramite bluetooth con phyphox
  char board_name[] = "rosmini6"; // per cambiare il nome della scheda che usa il BLE: non usare al cun carattere speciale e nessun spazio
  #define ROSSO 22     
  #define BLU 24     
  #define VERDE 23
  #define LED_PWR 25

  const int Sensor = A0;             //Pin di lettura del voltaggio
  float SenVal = 0;            //variabile per la lettura del voltaggio
  int Supply = 8;              //Pin di carica e scarica del condensatore

unsigned long initial_time, first_time, fourth_time;
float first_difference_float, fourth_difference_float;   // variabili di comodo per il tempo

void setup() {
  PhyphoxBLE::minConInterval = 6; //6 = 7.5ms       // impostazioni per il bluetooth
  PhyphoxBLE::maxConInterval = 6; //6 = 7.5ms
  PhyphoxBLE::slaveLatency = 0; //
  PhyphoxBLE::timeout = 10; //10 = 100ms 
  
  pinMode(ROSSO, OUTPUT);  // dichiarazione dei led della scheda
  pinMode(BLU, OUTPUT);
  pinMode(VERDE, OUTPUT);

  digitalWrite(ROSSO, LOW);    // 
  digitalWrite(VERDE, HIGH);
  digitalWrite(BLU, HIGH);
  
  Serial.begin(9600);  // inizializza il monitor seriale nel caso si utilizza phyphox il monitor seriale deve essere escluso
 
  pinMode(Supply, OUTPUT);      //Dichiarazione del pin 8 di carica e scarica
  digitalWrite(Supply, LOW);    //voltaggio 0 V sul pin di carica
  
  PhyphoxBLE::start(board_name); // Il nome della scheda può essere cambiato all'inizio del programma, attiva la conversazione bluetooth
  PhyphoxBLE::configHandler = &receivedData; // routine per mandare i comandi sulla scheda 
}

void loop() {
 if (PhyphoxBLE::currentConnections > 0) {
    digitalWrite(ROSSO, HIGH);
    digitalWrite(VERDE, LOW);
    digitalWrite(BLU, HIGH);
  } else {
    digitalWrite(ROSSO, LOW);
    digitalWrite(VERDE, HIGH);
    digitalWrite(BLU, HIGH);
  }
CaricaScarica();  
}

void receivedData() { // see Phyphox Arduino Library example
  if (PhyphoxBLE::currentConnections == 1) {
     initial_time = millis();  // se se la connessione è attiva prendi il tempo di inizio esperimento
  }
}

void CaricaScarica(){
   first_time = millis();    // leggi il tempo di misura
   first_difference_float = ((float)first_time - (float)initial_time) / 1000;  // tempo a cui viene la lettura
   SenVal = analogRead(Sensor);      //Lettura del valore di tensione del condensatoreSample 
   Serial.print(SenVal*3.3/1023.);   // stampa su monitor seriale latensione letta normalizzata a 3.3 V 
   Serial.print(" ");                // stampa spazio vuoto 
   Serial.println(first_difference_float);  // stampa il tempo normalizzato
   PhyphoxBLE::write(first_difference_float, SenVal);
   delay(2);
   if (SenVal == 1023){            // se il valore di tensione è al massimo metti al tensione a 0 V
    digitalWrite(Supply, LOW);
  }
   if(SenVal == 0){
   digitalWrite(Supply, HIGH);  // se il valore di tensione è al minimo metti al tensione a 3.3 V
   }
   }
