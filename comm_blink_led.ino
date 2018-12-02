/*
 * The purpose of this example is to test UART communication to Arduino.
 * If Arduino receives an 'h' it turn on the built-in LED.
 * If other character is received, LED is turned off.
 * 
 * Tested on Arduino Mega by Jan Robles
 * 
 * Hardware connection(TTL & Arduino Mega)
 * TTL on PC USB port
 * TTL (Rx) to Mega (Tx)
 * TTL (Tx) to Mega (Rx)
 * TTL (Vin, GND) to Mega (3.3V, GND)
 * 
*/

byte byteRead;

void setup() {                
// Turn the Serial Protocol ON
  Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);


}

void loop() {
   /*  check if data has been sent from the computer: */
  if (Serial.available()) {
    /* read the most recent byte */
    byteRead = Serial.read();

    if(byteRead=='h'){
    
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(1000);

  Serial.write('h'); //just to make sure 'h' was sent

    }else{
      
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(1000);
  
  }
  }
}
