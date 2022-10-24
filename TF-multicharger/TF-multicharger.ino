/*
  Add addtionalboard manager url: http://arduino.esp8266.com/stable/package_esp8266com_index.json
  Board: ESPino ESP-12 Module
  install boardmanager: ESP8266
  Hardware: 16 channel relayboard with ESP-12
*/
//Shiftregister spi pins
#define    HC595_SI_PIN     14
#define    HC595_SCK_PIN    13
#define    HC595_RCK_PIN    12
#define    HC595_G_PIN    5

//Relay and time definitions
#define    RELAY_COUNT 16
#define    STARTUP_TIME 5*1000 //time before starting the show
#define    ON_TIME 24*60*60*1000 // Time the relay is on
#define    DELAY_TIME 2*60*1000 //time all relays off, before switching to next relay

//SPI Shiftregister pin initialization
void HC595Init(void)
{
 pinMode(HC595_SI_PIN,  OUTPUT);
 pinMode(HC595_SCK_PIN, OUTPUT);
 pinMode(HC595_RCK_PIN, OUTPUT);
 pinMode(HC595_G_PIN,   OUTPUT);
 digitalWrite(HC595_G_PIN, HIGH);
 digitalWrite(HC595_SI_PIN, LOW);
 digitalWrite(HC595_SCK_PIN,LOW);
 digitalWrite(HC595_RCK_PIN,LOW);
}

//SPI Send data to shiftregister
void HC595SendData(unsigned int OutData)
{
    unsigned char i;
    for (i = 0; i < RELAY_COUNT; i++)
    {
      digitalWrite(HC595_SCK_PIN,LOW);
      if ((OutData & 0x8000) == 0x8000)
      {
        digitalWrite(HC595_SI_PIN,HIGH);
      }
      else
      {
        digitalWrite(HC595_SI_PIN,LOW); 
      }
      OutData = OutData << 1;
      digitalWrite(HC595_SCK_PIN,HIGH);
    }
    
    digitalWrite(HC595_RCK_PIN,LOW);
    digitalWrite(HC595_RCK_PIN,HIGH);
    digitalWrite(HC595_G_PIN,LOW);
}

unsigned int relay_data = 0x0000;
//Turn on specified relay
void RelayON(unsigned int number)
{
  switch(number)
  {
    case 1  : relay_data = relay_data | 0x0001; break;
    case 2  : relay_data = relay_data | 0x0002; break;
    case 3  : relay_data = relay_data | 0x0004; break;
    case 4  : relay_data = relay_data | 0x0008; break;
    case 5  : relay_data = relay_data | 0x0010; break;
    case 6  : relay_data = relay_data | 0x0020; break;
    case 7  : relay_data = relay_data | 0x0040; break;
    case 8  : relay_data = relay_data | 0x0080; break;
    case 9  : relay_data = relay_data | 0x0100; break;
    case 10 : relay_data = relay_data | 0x0200; break;
    case 11 : relay_data = relay_data | 0x0400; break;
    case 12 : relay_data = relay_data | 0x0800; break;
    case 13 : relay_data = relay_data | 0x1000; break;
    case 14 : relay_data = relay_data | 0x2000; break;
    case 15 : relay_data = relay_data | 0x4000; break;
    case 16 : relay_data = relay_data | 0x8000; break;
    default : break;
  }
  HC595SendData(relay_data);
}

//Turn off specified relay
void RelayOFF(unsigned int number)
{
  switch(number)
  {
    case 1  : relay_data = relay_data & 0xFFFE; break;
    case 2  : relay_data = relay_data & 0xFFFD; break;
    case 3  : relay_data = relay_data & 0xFFFB; break;
    case 4  : relay_data = relay_data & 0xFFF7; break;
    case 5  : relay_data = relay_data & 0xFFEF; break;
    case 6  : relay_data = relay_data & 0xFFDF; break;
    case 7  : relay_data = relay_data & 0xFFBF; break;
    case 8  : relay_data = relay_data & 0xFF7F; break;
    case 9  : relay_data = relay_data & 0xFEFF; break;
    case 10 : relay_data = relay_data & 0xFDFF; break;
    case 11 : relay_data = relay_data & 0xFBFF; break;
    case 12 : relay_data = relay_data & 0xF7FF; break;
    case 13 : relay_data = relay_data & 0xEFFF; break;
    case 14 : relay_data = relay_data & 0xDFFF; break;
    case 15 : relay_data = relay_data & 0xBFFF; break;
    case 16 : relay_data = relay_data & 0x7FFF; break;
    default : break;
  }
  HC595SendData(relay_data);
}

void setup() 
{
  //Setup spi pins
  HC595Init();
  // Turnoff all relays
  HC595SendData(0x0000);
}

void loop() 
{
  delay(STARTUP_TIME);
  // turn on one relay at a time
  unsigned char i;
  for (i = 1; i <= RELAY_COUNT; i++)
  {
    RelayON(i); 
    delay(ON_TIME);
    RelayOFF(i); 
    delay(DELAY_TIME);
  }
}
