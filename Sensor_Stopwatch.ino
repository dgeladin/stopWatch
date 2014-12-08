#include <StopWatch.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
 
 StopWatch mySW;   // MILLIS (default)


/*-----( Declare Constants )-----*/
#define SWITCHPIN 2
#define LEDPIN    13

/*-----( Declare Variables )-----*/
int  detector_state;  /* Holds the last state of the switch */
int  stopwatch_state;
int  change_state;
int  elapsed_millis;
float display_time;
long elapsedTime ;                  // elapsed time for stop watch
int fractional;                     // variable used to store fractional part of time
String LcdString;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  detector_state = 0;
  stopwatch_state = 0;
  change_state = LOW;
  
  Serial.begin(9600);
  pinMode (SWITCHPIN, INPUT );
  pinMode (LEDPIN, OUTPUT );
  lcd.print("Start...");
}/*--(end setup)---*/

void  loop ()  /*----( LOOP: RUNS CONSTANTLY )----*/
{
  detector_state = digitalRead (SWITCHPIN);
  if ( HIGH == detector_state) 
  {
    digitalWrite (LEDPIN, HIGH );
    change_state = HIGH;
  } 
  else 
  {
    digitalWrite (LEDPIN, LOW );
    if ( change_state == HIGH ) {
       Serial.println("Call StopWatch Function");
       stop_watch();
    } 
    change_state = LOW;

  }
  delay (50);
}/* --(end main loop )-- */

void stop_watch () 
{
  if ( stopwatch_state == 0 ) 
  {
    Serial.println("Starting Stop Watch..");
    mySW.start();
    stopwatch_state = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Awaiting Finish.");
    delay(2000);
  } else {
    Serial.println("Stopping Stop Watch..");
    mySW.stop();
    elapsedTime = mySW.elapsed();
    Serial.print("Stopwatch: ");
    LcdString = String((int)(elapsedTime / 1000L));
    Serial.print( (int)(elapsedTime / 1000L));         // divide by 1000 to convert to seconds - then cast to an int to print
    Serial.print(".");                             // print decimal point
    LcdString = LcdString + ".";
    // use modulo operator to get fractional part of time 
    fractional = (int)(elapsedTime % 1000L);
    // pad in leading zeros - wouldn't it be nice if 
    // Arduino language had a flag for this? :)
    if (fractional == 0) {
          Serial.print("000");      // add three zero's
          LcdString = LcdString + "000";
    } else if (fractional < 10) {   // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
          Serial.print("00");       // add two zeros
          LcdString = LcdString + "00";
    }  else if (fractional < 100) {
          Serial.print("0");        // add one zero
          LcdString = LcdString + "0";
    }    
    Serial.println(fractional);  // print fractional part of time
    LcdString = LcdString + fractional;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time: " + LcdString);
    delay(2000);
    mySW.reset();
    stopwatch_state = 0;
  }
}

/* ( THE END ) */
