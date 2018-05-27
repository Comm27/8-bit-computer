#define REG_CLK 11
#define REG_OUTPUT 12
#define DATA_IN 13

#define DATA_FIRST 2
#define DATA_LAST  9
#define EEPROM_WRITE 10

   

void setAddress(int address, bool outputEnable)
{
    shiftOut(DATA_IN, REG_CLK, MSBFIRST, address >> 8 | (outputEnable ? 0x00 : 0x80));
    shiftOut(DATA_IN, REG_CLK, MSBFIRST, address);

    digitalWrite(REG_OUTPUT, LOW );
    digitalWrite(REG_OUTPUT, HIGH);
    digitalWrite(REG_OUTPUT, LOW );
}

byte outputEEPROM(int address)
{
    for(int i = DATA_FIRST; i <= DATA_LAST; ++i)
    {
        pinMode(i, INPUT);
    }

    setAddress(address, true);

    byte output = 0;

    for(int pin = DATA_LAST; pin >= DATA_FIRST; --pin)
    {
        output = (output << 1) + digitalRead(pin);
    }

    return output;
}

void outputContent()
{
  for(int currentAddress = 0; currentAddress < 2048; ++currentAddress)
  {
    byte data = outputEEPROM(currentAddress);
    char buffer[255];
    sprintf(buffer, "%d : %d", currentAddress, data);
    Serial.println(buffer);
    delay(1);
  }
}

void inputEEPROM(int address, byte data)
{
    setAddress(address, false);
    for(int pin = DATA_FIRST; pin <= DATA_LAST; ++pin)
    {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, data & 0x01);
      data = data >> 1;
    }

    digitalWrite(EEPROM_WRITE, LOW );
    delayMicroseconds(1);
    digitalWrite(EEPROM_WRITE, HIGH);

    delay(50);
}

byte bcdData[] = {0x01, 0x4f, 0x12, 0x06, 0x4c, 0x24, 0x20, 0x0f, 0x00, 0x04, 0x08, 0xc2, 0x31, 0x42, 0x30, 0x38};

void setup()
{
    pinMode(REG_CLK,        OUTPUT);
    pinMode(REG_OUTPUT,     OUTPUT);
    pinMode(DATA_IN,        OUTPUT);
    pinMode(EEPROM_WRITE,   OUTPUT);

    
    digitalWrite(EEPROM_WRITE, HIGH);

    Serial.begin(9600);

    // inputEEPROM(0, 0x32);
    // inputEEPROM(100, 0x03);
    
    for(int i = 0; i < 16; ++i)
    {
        inputEEPROM(i, bcdData[i]);
    }
    
    outputContent();

}

void loop()
{
 
}


