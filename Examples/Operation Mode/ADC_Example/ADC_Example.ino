#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <avr/sleep.h>

Adafruit_ADS1015 ads1015;

#define interruptPin 2

byte volatile toggle=0;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  Serial.println("Single-ended readings from AIN0 with >3.0V comparator");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
  Serial.println("Comparator Threshold: 1000 (3.000V)");
  ads1015.begin();
  
  pinMode(interruptPin, INPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  
  // Setup 3V comparator on channel 0
  ads1015.startComparator_windowed(0, 1400, 700);
  
}

void loop(void)
{
  int16_t adc0;

  // Comparator will only de-assert after a read
  adc0 = ads1015.getLastConversionResults();
  Serial.print("AIN0: "); Serial.println(adc0);
  
  sleepNow();

}

void indicate(){
  // execute code here after wake-up before returning to the loop() function  
  // timers and code using timers (serial.print and more...) will not work here.  
  // we don't really need to execute any special functions here, since we  
  // just want the thing to wake up  
}

void sleepNow()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
 
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
    Serial.println("Going to sleep");     
    delay(100);
    // Clear the comparator right before attaching interrupt   
    ads1015.getLastConversionResults();
    attachInterrupt(digitalPinToInterrupt(interruptPin), indicate , LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW
 
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
                        
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep... 
    
    detachInterrupt(digitalPinToInterrupt(interruptPin));      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
    Serial.println("Woke up!");
                             
} 
