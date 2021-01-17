/**********
 * Zongyi Yang
 * Demo for PD3435
***********/

namespace PD3435
{
  const byte _WR  = A3; // write
  const byte _RST = A0; // reset
  const byte CE1  = A1; // chip enables
  const byte _CE0 = A2;
  const byte A2   = 10; // address inputs
  const byte A1   = 9;
  const byte A0   = 8;
  const byte D0   = 0;  // data bus I/O
  const byte D1   = 1;
  const byte D2   = 2;
  const byte D3   = 3;
  const byte D4   = 4;
  const byte D5   = 5;
  const byte D6   = 6;
  const byte D7   = 7;
  
  void writeAddress(int addr)
  {
    digitalWrite(A0, bitRead(addr, 0));
    digitalWrite(A1, bitRead(addr, 1));
    digitalWrite(A2, bitRead(addr, 2));
  }
  
  void writeData(int data)
  {
    digitalWrite(D0, bitRead(data, 0));
    digitalWrite(D1, bitRead(data, 1));
    digitalWrite(D2, bitRead(data, 2));
    digitalWrite(D3, bitRead(data, 3));
    digitalWrite(D4, bitRead(data, 4));
    digitalWrite(D5, bitRead(data, 5));
    digitalWrite(D6, bitRead(data, 6));
    digitalWrite(D7, bitRead(data, 7));
  }
  
  void doWrite(int addr, int data)
  {
    digitalWrite(_CE0, LOW);
    digitalWrite(CE1, HIGH);
    digitalWrite(_WR, HIGH);
  
    writeAddress(addr);
    writeData(data);
    
    digitalWrite(_WR, LOW);
    digitalWrite(_WR, HIGH);
  }
  
  // lamptest overwrites all other attributes
  // blink applies to all digits
  void setAttribute(bool doLampTest, bool doBlink, int attr, int brightness)
  {
    unsigned int data = 0x00;
    data |= (brightness & B11);
    data |= ((attr & B111) << 2);
    data |= (doBlink << 5);
    data |= (doLampTest << 6);
    doWrite(0x00, data);
  }
  
  // digit 0 is rightmost digit
  void setDigit(int digit, int data, bool useAttribute)
  {
    doWrite(digit+4, data|(useAttribute<<7));
  }

  void init()
  {
    digitalWrite(CE1, HIGH);
    digitalWrite(_CE0, HIGH);
    digitalWrite(_RST, HIGH);
    digitalWrite(_WR, HIGH);
    pinMode(CE1, OUTPUT);
    pinMode(_CE0, OUTPUT);
    pinMode(_RST, OUTPUT);
    pinMode(_WR, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
  }
}

void setup()
{
  PD3435::init();
  
  // do a lamp test for 1s to start
  PD3435::setAttribute(true, false, 0, 0);
  delay(1000);
  PD3435::setAttribute(false, false, B101, 1);
}

const int digits = 4;
unsigned int gData = 0x20-digits+1; // characters up to 0x20 are all blank
void loop() {
  PD3435::setDigit(3, gData, false);
  PD3435::setDigit(2, gData+1, false);
  PD3435::setDigit(1, gData+2, false);
  PD3435::setDigit(0, gData+3, false);
  gData++;
  
  if  (gData >= 0x80) // bit 7 enables attributes for that digit
    gData = 0x20-digits+1;
  delay(300);
}
