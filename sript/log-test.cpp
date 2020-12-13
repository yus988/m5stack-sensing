#include <M5Stack.h>
        File f;
bool fOK = false;
int i = 0;

void setup()
{
    M5.begin();

    // Start SD card
    if (!SD.begin())
    {
        M5.Lcd.println("ERROR
                       : SD CARD.");
        while (1)
            ;
    }

    // Open log file
    f = SD.open("/ testLog.txt", FILE_WRITE);
    if (!f)
    {
        M5.Lcd.println("ERROR
                       : OPEN FILE.");
        while (1)
            ;
    }

    // Greeting Message
    M5.Lcd.println("testLogger");
}

void loop()
{
    String dataString = "";

    M5.update();

    if (fOK)
    {
        dataString += String(i++);
        dataString += String(",");
        dataString += String(millis());

        f.println(dataString);
        //Serial.println(dataString);
    }

    if (M5.BtnA.wasPressed())
    {
        M5.Lcd.println("Start >");
        fOK = true;
    }
    if (M5.BtnB.wasPressed())
    {
        M5.Lcd.println("End");
        fOK = false;
    }
    if (M5.BtnC.wasPressed())
    {
        f.close();
        M5.powerOFF();
    }
    delay(20);
}