#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

const int BUTTON_PIN = A3;

const unsigned long timestamps[] PROGMEM = {
  23790, 28780, 34980, 39980, 46480, 51620, 55080, 58750, 62810,
  68060, 70860, 73780, 80800, 85210, 87730, 92420, 94260, 103660,
  108720, 125120, 128040, 130850, 138100, 142370, 144800, 149260,
  155190, 160990, 166620, 172210, 178100, 183910, 189400, 195260
};

const int totalLines = 34;
unsigned long startTime = 0;
bool isPlaying = false;
int currentLineIndex = 0;
int snowX, snowY;

void printLyric(int index) {
  switch (index) {
    case 0: display.print(F("That snowflake once\nfell on my nose")); break;
    case 1: display.print(F("Now flying backwards\nto last winter")); break;
    case 2: display.print(F("I realize that\nhellos are goodbyes")); break;
    case 3: display.print(F("The moment I softly\nlet go of your hand")); break;
    case 4: display.print(F("Winds hum through the\ntrembling branches")); break;
    case 5: display.print(F("A song for a soul\nbound to soar to")); break;
    case 6: display.print(F("The very edge of\nthe starry ocean")); break;
    case 7: display.print(F("Even if we're meant\nto fall apart")); break;
    case 8: display.print(F("I will gaze at where\nyou are for all time")); break;
    case 9: display.print(F("Bon voyage")); break;
    case 10: display.print(F("May your path be\nclear")); break;
    case 11: display.print(F("May you get to where\ndreams are all\ncrystalline & sweet")); break;
    case 12: display.print(F("Please crack a\nlittle smile")); break;
    case 13: display.print(F("You won't be alone")); break;
    case 14: display.print(F("In this world you've\nchosen as your own")); break;
    case 15: display.print(F("Don't worry")); break;
    case 16: display.print(F("I'm well aware that\nall snowflakes melt\nin the end")); break;
    case 17: display.print(F("Which is sign of\nwarmth and hope\nof spring")); break;
    case 18: display.print(F("So I'll dance\nflamboyantly into\nthat good night")); break;
    case 19: display.print(F("Bon voyage")); break;
    case 20: display.print(F("May your path be\nclear")); break;
    case 21: display.print(F("May you get to where\ndreams are all\ncrystalline & sweet")); break;
    case 22: display.print(F("Please crack a\nlittle smile")); break;
    case 23: display.print(F("You won't be alone")); break;
    case 24: display.print(F("In this world you've\nchosen as your own")); break;
    case 25: display.print(F("Timing is such a\ndazzling riddle")); break;
    case 26: display.print(F("Guess I'll spend my\nwhole life\nunraveling it")); break;
    case 27: display.print(F("Though it might cost\nme everything I have")); break;
    case 28: display.print(F("I never regret it;\nwouldn't have it any\nother way 'cause")); break;
    case 29: display.print(F("Those who trace\nstarlight on their\nlonely roads")); break;
    case 30: display.print(F("Will find their\ndestinations in\nthe cosmos")); break;
    case 31: display.print(F("Meeting you in that\nswirling snowfall's")); break;
    case 32: display.print(F("The most beautiful\nthing ever\nhappened to me")); break;
    case 33: display.print(F("...")); break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin();
  Wire.setClock(100000);
  delay(500);

  Serial.println(F("Booting OLED..."));
  
  if (!display.begin(0x3C, true)) {
    Serial.println(F("CRITICAL ERROR: OLED failed to allocate RAM."));
    for (;;); 
  }
  Serial.println(F("OLED OK!"));

  display.clearDisplay();
  display.display();

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(F("  Booting audio..."));
  display.display();

  Serial.println(F("Booting DFPlayer..."));
  mySoftwareSerial.begin(9600);
  delay(2000);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("DFPlayer FAILED!"));
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println(F("  Audio failed :("));
    display.display();
  } else {
    Serial.println(F("DFPlayer OK!"));
    myDFPlayer.volume(50); 
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println(F("  Ready for"));
  display.println(F("  tears T_T"));
  display.println(F(""));
  display.println(F("  Press button"));
  display.println(F("  to play!"));
  display.display();

  snowX = random(2, 126);
  snowY = 34;
}

void loop() {
  while (mySoftwareSerial.available()) mySoftwareSerial.read();

  if (!isPlaying && digitalRead(BUTTON_PIN) == LOW) {
    isPlaying = true;
    currentLineIndex = 0;
    myDFPlayer.play(1);
    delay(500);
    startTime = millis();
  }

  if (isPlaying) {
    unsigned long elapsedTime = millis() - startTime;

    unsigned long nextTimestamp = pgm_read_dword(&timestamps[currentLineIndex + 1]);
    unsigned long firstTimestamp = pgm_read_dword(&timestamps[0]);

    if (currentLineIndex < totalLines - 1 && elapsedTime >= nextTimestamp) {
      currentLineIndex++;
    }

    display.clearDisplay();

    display.setCursor(0, 0);
    if (elapsedTime >= firstTimestamp && currentLineIndex < totalLines) {
      printLyric(currentLineIndex);
    } else if (elapsedTime < firstTimestamp) {
      display.print(F("Song starting...\nGet ready!"));
    } else {
      display.print(F("Song over...\n*cries softly*"));
    }

    display.drawLine(0, 32, 128, 32, SH110X_WHITE);

    display.drawLine(snowX, snowY - 2, snowX, snowY + 2, SH110X_WHITE);
    display.drawLine(snowX - 2, snowY, snowX + 2, snowY, SH110X_WHITE);

    snowY += 1;
    snowX += random(-1, 2);
    snowX = constrain(snowX, 2, 126);

    if (snowY > 64) {
      snowY = 34;
      snowX = random(2, 126);
    }

    display.display();
  }
}
