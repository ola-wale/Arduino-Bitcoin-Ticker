/**
 * Arduino bitcoin ticker - get's bitcoin's price from various exchanges and outputs it to the 16x2 display shield.
 * Author/s:
 * Malik Morenigbade : Indeks #363038
 * Tawanda Chikore : Indeks #362061
 * 
 * How it works.
 * Modes/Exchanges/Pairs:
 * There are five 'modes'
 * Mode 0 - Pulls the price from Kraken.com on the BTC/EUR Pair : https://api.kraken.com/0/public/Ticker?pair=XXBTZEUR
 * Mode 1 - Pulls the price from Kraken.com on the BTC/USD Pair : https://api.kraken.com/0/public/Ticker?pair=XXBTZUSD
 * Mode 2 - Pulls the price from Bitstamp.com on the BTC/EUR Pair : https://www.bitstamp.net/api/v2/ticker/btceur/
 * Mode 3 - Pulls the price from Bitstamp.com on the BTC/USD Pair : https://www.bitstamp.net/api/v2/ticker/btcusd/
 * Mode 4 - Pulls the price from Bitbay.net on the BTC/PLN Pair : https://bitbay.net/API/Public/BTCPLN/ticker.json
 * 
 * I'm using my server as a conduit to poll these apis : It simplifies the data into a simple array of [2] elements & Less RAM usage on the arduino.
 * I've attached the .php file running on my server along with this file.
 */


#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <ArduinoJson.h>
LiquidCrystal lcd(3,4,7,8,9,10);

char msgs[5][16] = {"Right Key OK ",
                    "Up Key OK    ",               
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK" };
int adc_key_val[5] ={50, 200, 400, 600, 800 };
int modes[5] = {0,1,2,3,4};
int current_mode = 0;
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

//my dorm's internet requires a registered mac
byte mac[] = {  0xF8, 0x46, 0x1C, 0x47, 0xFD, 0x34 };
int mode = 0;
/**
 * Function declarations
 */
void getprice(int mode=0);
boolean in_array(int array[], int element);
void cprint(const char *toprint="",bool clear=true,int x=0,int y=0);
/**
 * Reserve memory for the json parser
 */
StaticJsonBuffer<200> jsonBuffer;
/**
 * Ethernet Client
 */
EthernetClient client;
 
void setup()
{
  /**
   * connect to serial port 9600
   */
  Serial.begin(9600);
  /**
   * Init 16x2 LCD
   */
  lcd.begin(16, 2);
  cprint("Fetching Price...");
  /**
   * initialize ethernet with the MAC address
   */
  Ethernet.begin(mac);
  /**
   * DHCP - print the current IP of the shield on the network
   */
  Serial.println(Ethernet.localIP());
  /**
   * Connect to my server on port 80 - redundant ?
   */
  client.connect("dev.owale.co",80);
  /**
   * while !connected keep trying.
   */
  while(!client.connected()) {
   Serial.print("reconnecting...");
   client.connect("dev.owale.co",80);
  }
  /**
   * on init - get price with current_mode which is 0 at init
   */
  getprice(current_mode);
}

void loop(){
  /**
   * Code to read key from 16x2 LCD shield - http://linksprite.com/wiki/index.php5?title=16_X_2_LCD_Keypad_Shield_for_Arduino
   */
  adc_key_in = analogRead(0);    // read the value from the sensor 
  key = get_key(adc_key_in);  // convert into key press
  if (key != oldkey)   // if keypress is detected
   {
    delay(50);  // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor 
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey)    
    {   
      oldkey = key;
      if (key >=0){
           Serial.print(msgs[key]);
           Serial.print("key pressed");
           current_mode++; //increment the current mode
           getprice(current_mode);          
      }
    }
  }
 delay(100);
}
void getprice(int mode){
    /**
     * If the current mode isn't in modes[]
     * reset current_mode back to the first mode in modes[0]
     */
    if(!in_array(modes,mode)){
      current_mode = modes[0];  
    }
    bool done = false;
    String query = "GET /microcontroller_api/?mode=";
    query.concat(mode);
    query.concat(" HTTP/1.0");
    client.println(query);
    client.println("Host: dev.owale.co");
    client.println();
    while(!client.available() && done == false) {
      String data = client.readString();
      data = getBody(data);
      Serial.println(data);
      JsonObject& body = parseBody(data);
      String pair = body["pair"];
      String value = body["price"];
      Serial.print("pair is ");
      Serial.print(pair);
      Serial.println();
      Serial.print("value is ");
      Serial.print(value);
      /**
       * Display bitcoin price data
       */
       /**
        * clear the LCD
        */
       lcd.clear();
       /**
        * set the cursor to the first block on the first line
        */
        lcd.setCursor(0,0);
        /**
         * Print the current Pair name on this line
         */
         lcd.print(pair);
         /**
          * set the cursor to the first block on the second line
          */
          lcd.setCursor(0,1);
          /**
           * Print the current Pair price on this line
           */
           lcd.print(value);
      done = true;
    }
}

/**
 * Clear the LCD (?) & Print a value to it
 * char {toprint} - the string to print to the LCD.
 * bool {clear} - to clear the LCD of previous values or not
 * int {x} - position of the cursor on the x axis
 * int {y} - position of the cursor on the y axis
 */
void cprint(const char *toprint,bool clear,int x,int y){
  if(clear){
      lcd.clear();
  }
  lcd.setCursor(x,y);
  lcd.print(toprint);
  //debug
  Serial.println();
  Serial.println(toprint);
}

/**
 * Because client.readString() returns data along with HTTP headers
 * we'll need to separate the header from the response body to utilize the JSON data.
 * I made my server echo a "~" which signifies the beginning of json data. so arduino knows where to start from see: [2]
 */
String getBody(String data){
    String result;
    int index = data.indexOf("~")+1; //[2]
    for(int i = index;i< data.length();i++){
      result += data[i];
    }
    return result;
}

/**
 * Parse JSON data to array.
 * String {body} response body to parse to JSON
 */
JsonObject& parseBody(String body){
    return jsonBuffer.parseObject(body);
}

// Convert ADC value to key number
int get_key(unsigned int input) {
    int k;
    for (k = 0; k < NUM_KEYS; k++) {
        if (input < adc_key_val[k]) {
            return k;
        }
    }
    if (k >= NUM_KEYS) k = -1; // No valid key pressed
    return k;
}

/**
 * Check if an element is in an array
 */
boolean in_array(int array[], int element) {
  for (int i = 0; i < sizeof(array) / sizeof(int); i++) {
      if (array[i] == element) {
          return true;
      }
    }
  return false;
}
