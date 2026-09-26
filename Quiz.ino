/* ============================================================
   MindMatrix — ATL Lab Quiz  ·  Firmware v2
   ESP8266 D1 mini · 4x4 keypad · Buzzer · Wi-Fi AP + Web
   ------------------------------------------------------------
   Keypad logic
     #        start  /  confirm  /  next  /  play again
     A B C D  select MCQ option
     0-9      type numeric answer
     *        clear  /  backspace

   Buzzer
     correct  ->  1 high-pitch beep
     wrong    ->  3 short low-pitch beeps

   HTML page is embedded in page.h (PROGMEM).
   No LittleFS upload is required.
   ============================================================ */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Keypad.h>
#include <ArduinoJson.h>

#include "page.h"

/* ============================================================
   PIN MAP
   ------------------------------------------------------------
   KEYPAD                    D1 mini       GPIO
   ------------------------------------------------------------
     Pin 1 (Row 1)  -------> D0            GPIO16
     Pin 2 (Row 2)  -------> D1            GPIO5
     Pin 3 (Row 3)  -------> D2            GPIO4
     Pin 4 (Row 4)  -------> D5            GPIO14
     Pin 5 (Col 1)  -------> D6            GPIO12
     Pin 6 (Col 2)  -------> D7            GPIO13
     Pin 7 (Col 3)  -------> D3            GPIO0    [!] boot pin
     Pin 8 (Col 4)  -------> D4            GPIO2    [!] boot pin + LED

   BUZZER
     (+)            -------> D8            GPIO15
     (-)            -------> GND
   ============================================================ */

#define BUZZER_PIN   D8

// ---------------- AP config ----------------
const char* AP_SSID = "MindMatrix";
const char* AP_PASS = "quiz1234";   // must be >= 8 chars

// ---------------- Keypad ----------------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {D0, D1, D2, D5};
byte colPins[COLS] = {D6, D7, D3, D4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------------- Question bank ----------------
struct Question {
  uint8_t type;      // 0 = MCQ, 1 = numeric
  const char* q;
  const char* a;
  const char* b;
  const char* c;
  const char* d;
  int16_t ans;       // MCQ: 0-3 (A-D) · NUM: correct number
};

const Question QUESTIONS[] = {
  // ---------- 30 MCQ ----------
  {0, "What is the chemical symbol of water?",              "CO2",  "H2O",  "O2",   "NaCl", 1},
  {0, "Which is the largest planet in our solar system?",   "Earth","Mars", "Jupiter","Saturn", 2},
  {0, "What is the approximate speed of light?",            "3x10^6 m/s","3x10^8 m/s","3x10^10 m/s","3x10^5 m/s", 1},
  {0, "What is the SI unit of force?",                      "Joule","Watt", "Newton","Pascal", 2},
  {0, "Who is commonly credited with inventing the electric bulb?",
                                                            "Newton","Edison","Einstein","Faraday", 1},
  {0, "What is the square root of 144?",                    "10",   "11",   "12",   "14",   2},
  {0, "What is decimal 5 in binary?",                       "100",  "101",  "110",  "111",  1},
  {0, "Which of these is an input device?",                 "Monitor","Printer","Keyboard","Speaker", 2},
  {0, "Which formula represents Ohm's law?",                "V = IR","P = VI","F = ma","E = mc^2", 0},
  {0, "Which planet is called the Red Planet?",             "Venus","Mars", "Jupiter","Mercury", 1},
  {0, "What is the largest organ in the human body?",       "Heart","Liver","Skin", "Lungs", 2},
  {0, "Which gas do plants absorb during photosynthesis?",  "Oxygen","Nitrogen","Carbon dioxide","Hydrogen", 2},
  {0, "What is the 7th prime number?",                      "13",   "15",   "17",   "19",   2},
  {0, "In Boolean logic, what is 1 OR 0?",                  "0",    "1",    "2",    "10",   1},
  {0, "What is the SI unit of electric current?",           "Volt", "Ohm",  "Ampere","Watt", 2},
  {0, "Which of these is a renewable energy source?",       "Coal", "Diesel","Solar","Petrol", 2},
  {0, "What is the chemical symbol of gold?",               "Ag",   "Au",   "Gd",   "Go",   1},
  {0, "How many bones are there in an adult human body?",   "106",  "206",  "306",  "406",  1},
  {0, "What does HTML stand for?",
     "HyperText Markup Language","HighText Machine Language","HyperTool Multi Language","Home Tool Markup Language", 0},
  {0, "Which device converts electrical energy into mechanical energy?",
                                                            "Generator","Motor","Transformer","Battery", 1},
  {0, "What is the LCM of 6 and 8?",                        "12",   "18",   "24",   "48",   2},
  {0, "What is each angle of an equilateral triangle?",     "30 deg","45 deg","60 deg","90 deg", 2},
  {0, "What is the pH of pure water?",                      "5",    "6",    "7",    "8",    2},
  {0, "Which planet is famous for its bright rings?",       "Mars", "Saturn","Venus","Mercury", 1},
  {0, "RAM is a volatile memory. True or false?",           "True", "False","Only when off","Cannot say", 0},
  {0, "1 byte is equal to how many bits?",                  "4 bits","8 bits","16 bits","32 bits", 1},
  {0, "Which force pulls objects towards the Earth?",       "Magnetism","Gravity","Friction","Pressure", 1},
  {0, "Which of these is a programming language?",          "Python","HTTP", "HTML", "USB",  0},
  {0, "What is the area of a circle?",                      "2 pi r","pi r^2","pi d","r^2", 1},
  {0, "What was India's first satellite?",                  "Chandrayaan","Aryabhata","INSAT","Rohini", 1},

  // ---------- 20 numeric ----------
  {1, "15 + 27 = ?",                                       nullptr,nullptr,nullptr,nullptr, 42},
  {1, "9 x 8 = ?",                                         nullptr,nullptr,nullptr,nullptr, 72},
  {1, "144 / 12 = ?",                                      nullptr,nullptr,nullptr,nullptr, 12},
  {1, "What is 25% of 200?",                               nullptr,nullptr,nullptr,nullptr, 50},
  {1, "What is the next prime number after 19?",           nullptr,nullptr,nullptr,nullptr, 23},
  {1, "What is the perimeter of a square of side 7 cm?",   nullptr,nullptr,nullptr,nullptr, 28},
  {1, "3^2 + 4^2 = ?",                                     nullptr,nullptr,nullptr,nullptr, 25},
  {1, "100 - 37 = ?",                                      nullptr,nullptr,nullptr,nullptr, 63},
  {1, "How many minutes are there in 2.5 hours?",          nullptr,nullptr,nullptr,nullptr, 150},
  {1, "2^5 = ?",                                           nullptr,nullptr,nullptr,nullptr, 32},
  {1, "What is the average of 10, 20, 30 and 40?",         nullptr,nullptr,nullptr,nullptr, 25},
  {1, "Solve for x: x + 15 = 40",                          nullptr,nullptr,nullptr,nullptr, 25},
  {1, "What is the area of a rectangle 12 cm x 5 cm?",     nullptr,nullptr,nullptr,nullptr, 60},
  {1, "A car travels 120 km in 2 hours. Speed in km/h?",   nullptr,nullptr,nullptr,nullptr, 60},
  {1, "7! / 5! = ?",                                       nullptr,nullptr,nullptr,nullptr, 42},
  {1, "What is binary 1010 in decimal?",                   nullptr,nullptr,nullptr,nullptr, 10},
  {1, "What is the LCM of 4 and 5?",                       nullptr,nullptr,nullptr,nullptr, 20},
  {1, "What is the HCF of 18 and 24?",                     nullptr,nullptr,nullptr,nullptr, 6},
  {1, "What is the sum of all angles of a triangle?",      nullptr,nullptr,nullptr,nullptr, 180},
  {1, "11^2 = ?",                                          nullptr,nullptr,nullptr,nullptr, 121},
};

const int TOTAL_Q = sizeof(QUESTIONS) / sizeof(QUESTIONS[0]);

// ---------------- State machine ----------------
enum QuizState { QS_IDLE, QS_QUESTION, QS_FEEDBACK, QS_FINISHED };
QuizState state = QS_IDLE;

int    qIndex        = 0;
int    score         = 0;
int    selectedIndex = -1;
String numBuffer     = "";
bool   lastCorrect   = false;

// ---------------- Server ----------------
ESP8266WebServer server(80);
DNSServer        dns;

// ============================================================
// BUZZER  (non-blocking)
// ============================================================
uint8_t  pendingBeeps   = 0;
uint32_t nextBeepTime   = 0;
uint16_t beepFreq       = 2000;
uint16_t beepDur        = 180;
uint16_t beepGap        = 140;

void beepCorrect() {
  noTone(BUZZER_PIN);
  pendingBeeps = 1;
  beepFreq     = 2200;   // high pitch
  beepDur      = 220;
  beepGap      = 180;
  nextBeepTime = millis();   // fire immediately
}

void beepWrong() {
  noTone(BUZZER_PIN);
  pendingBeeps = 3;
  beepFreq     = 380;    // low pitch
  beepDur      = 150;
  beepGap      = 130;
  nextBeepTime = millis();   // fire immediately
}

void beepBoot() {
  noTone(BUZZER_PIN);
  pendingBeeps = 2;
  beepFreq     = 1500;
  beepDur      = 100;
  beepGap      = 90;
  nextBeepTime = millis() + 300;
}

void updateBuzzer() {
  if (pendingBeeps == 0) return;
  if ((int32_t)(millis() - nextBeepTime) < 0) return;

  tone(BUZZER_PIN, beepFreq, beepDur);
  pendingBeeps--;
  nextBeepTime = millis() + beepDur + beepGap;
}

// ============================================================
// SERIAL HELPERS
// ============================================================
void stamp() {
  uint32_t t = millis();
  Serial.printf("[%02lu:%02lu.%03lu] ",
                (unsigned long)(t / 60000),
                (unsigned long)((t / 1000) % 60),
                (unsigned long)(t % 1000));
}

const char* stateName(QuizState s) {
  switch (s) {
    case QS_IDLE:     return "QS_IDLE";
    case QS_QUESTION: return "QS_QUESTION";
    case QS_FEEDBACK: return "QS_FEEDBACK";
    case QS_FINISHED: return "QS_FINISHED";
  }
  return "?";
}

void logKey(char key, const char* action) {
  stamp();
  Serial.printf("[KEY  ] '%c'  ->  %s\n", key, action);
}

void logStateChange(QuizState from, QuizState to) {
  stamp();
  Serial.printf("[STATE ] %s  ->  %s\n", stateName(from), stateName(to));
}

void logQuestionHeader() {
  const Question& q = QUESTIONS[qIndex];
  stamp();
  Serial.printf("[Q %03d/%03d] [%s]\n",
                qIndex + 1, TOTAL_Q,
                q.type == 0 ? "MCQ" : "NUM");
  stamp();
  Serial.printf("           Q: %s\n", q.q);

  if (q.type == 0) {
    stamp(); Serial.printf("           A) %s\n", q.a);
    stamp(); Serial.printf("           B) %s\n", q.b);
    stamp(); Serial.printf("           C) %s\n", q.c);
    stamp(); Serial.printf("           D) %s\n", q.d);
    stamp(); Serial.println("           >> Press A/B/C/D to select, # to confirm");
  } else {
    stamp(); Serial.println("           >> Type digits 0-9, * to backspace, # to submit");
  }
}

void logAnswer(bool correct, const char* given, const char* expected) {
  stamp();
  Serial.printf("[ANSWER] %s  |  given: %s  |  expected: %s  |  score: %d\n",
                correct ? "CORRECT" : "WRONG ",
                given, expected, score);
}

// ============================================================
// BOOT BANNER
// ============================================================
void printBootBanner() {
  Serial.println();
  Serial.println(F("============================================================"));
  Serial.println(F("   __  __ _         _  __  __      _        _"));
  Serial.println(F("  |  \\/  (_)_ _  __| | \\ \\/ /__ _ | |_ _ _ (_)_ ____"));
  Serial.println(F("  | |\\/| | | ' \\/ _` |  >  <___| || | ' \\| | '_ \\/ -_)"));
  Serial.println(F("  |_|  |_|_|_||_\\__,_| /_/\\_\\   \\_,_|_||_|_| .__/\\___|"));
  Serial.println(F("                                            |_|"));
  Serial.println(F("============================================================"));
  Serial.println(F("   MindMatrix  ·  ATL Lab Quiz  ·  Firmware v2.0"));
  Serial.println(F("   ESP8266 D1 mini  ·  4x4 Keypad  ·  Buzzer"));
  Serial.println(F("============================================================"));
  Serial.println();

  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  HARDWARE  ·  PIN MAP                                    |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  KEYPAD                                                  |"));
  Serial.println(F("|    Pin 1  (Row 1)  ----->  D0   (GPIO16)                 |"));
  Serial.println(F("|    Pin 2  (Row 2)  ----->  D1   (GPIO5)                  |"));
  Serial.println(F("|    Pin 3  (Row 3)  ----->  D2   (GPIO4)                  |"));
  Serial.println(F("|    Pin 4  (Row 4)  ----->  D5   (GPIO14)                 |"));
  Serial.println(F("|    Pin 5  (Col 1)  ----->  D6   (GPIO12)                 |"));
  Serial.println(F("|    Pin 6  (Col 2)  ----->  D7   (GPIO13)                 |"));
  Serial.println(F("|    Pin 7  (Col 3)  ----->  D3   (GPIO0)   [BOOT PIN]     |"));
  Serial.println(F("|    Pin 8  (Col 4)  ----->  D4   (GPIO2)   [BOOT + LED]   |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  BUZZER                                                  |"));
  Serial.println(F("|    (+)             ----->  D8   (GPIO15)                 |"));
  Serial.println(F("|    (-)             ----->  GND                           |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println();

  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  KEYPAD LAYOUT                                           |"));
  Serial.println(F("|      +-----+-----+-----+-----+                           |"));
  Serial.println(F("|      |  1  |  2  |  3  |  A  |                           |"));
  Serial.println(F("|      +-----+-----+-----+-----+                           |"));
  Serial.println(F("|      |  4  |  5  |  6  |  B  |                           |"));
  Serial.println(F("|      +-----+-----+-----+-----+                           |"));
  Serial.println(F("|      |  7  |  8  |  9  |  C  |                           |"));
  Serial.println(F("|      +-----+-----+-----+-----+                           |"));
  Serial.println(F("|      |  *  |  0  |  #  |  D  |                           |"));
  Serial.println(F("|      +-----+-----+-----+-----+                           |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  KEYS                                                    |"));
  Serial.println(F("|    #        start / confirm / next / play again          |"));
  Serial.println(F("|    A B C D  select MCQ option                            |"));
  Serial.println(F("|    0 - 9    type numeric answer                          |"));
  Serial.println(F("|    *        clear / backspace                            |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println();
}

void printWifiBanner(IPAddress ip) {
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println(F("|  WIRELESS ACCESS POINT                                   |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.printf ( "|  SSID      : %-42s |\n", AP_SSID);
  Serial.printf ( "|  Password  : %-42s |\n", AP_PASS);
  Serial.printf ( "|  Gateway   : http://%-33s |\n", ip.toString().c_str());
  Serial.println(F("|  Portal    : connect -> browser opens quiz automatically |"));
  Serial.println(F("+----------------------------------------------------------+"));
  Serial.println();
}

// ============================================================
// Helpers
// ============================================================
void resetQuiz() {
  qIndex        = 0;
  score         = 0;
  selectedIndex = -1;
  numBuffer     = "";
  lastCorrect   = false;
}

const char* correctAnswerString(const Question& q) {
  if (q.type == 0) {
    switch (q.ans) {
      case 0: return q.a;
      case 1: return q.b;
      case 2: return q.c;
      default: return q.d;
    }
  }
  static char buf[10];
  snprintf(buf, sizeof(buf), "%d", q.ans);
  return buf;
}

// ============================================================
// Keypad handler — fully logged
// ============================================================
void handleKey(char key) {
  // ---------- IDLE ----------
  if (state == QS_IDLE) {
    if (key == '#') {
      logKey(key, "Start quiz");
      QuizState prev = state;
      resetQuiz();
      state = QS_QUESTION;
      logStateChange(prev, state);
      logQuestionHeader();
    } else {
      logKey(key, "Ignored (idle — press # to start)");
    }
    return;
  }

  // ---------- FINISHED ----------
  if (state == QS_FINISHED) {
    if (key == '#') {
      logKey(key, "Play again");
      QuizState prev = state;
      resetQuiz();
      state = QS_QUESTION;
      logStateChange(prev, state);
      logQuestionHeader();
    } else {
      logKey(key, "Ignored (quiz finished — press # to restart)");
    }
    return;
  }

  // ---------- QUESTION ----------
  if (state == QS_QUESTION) {
    const Question& q = QUESTIONS[qIndex];

    if (q.type == 0) {              // ---------- MCQ ----------
      if (key >= 'A' && key <= 'D') {
        selectedIndex = key - 'A';
        char msg[64];
        snprintf(msg, sizeof(msg),
                 "MCQ select %c (index %d)", key, selectedIndex);
        logKey(key, msg);

      } else if (key == '*') {
        if (selectedIndex >= 0) {
          logKey(key, "Clear MCQ selection");
          selectedIndex = -1;
        } else {
          logKey(key, "Ignored (* — nothing selected)");
        }

      } else if (key == '#') {
        if (selectedIndex < 0) {
          logKey(key, "Ignored (# — no option selected yet)");
        } else {
          char msg[64];
          snprintf(msg, sizeof(msg),
                   "Confirm MCQ (selected=%c index=%d)",
                   'A' + selectedIndex, selectedIndex);
          logKey(key, msg);

          lastCorrect = (selectedIndex == q.ans);
          if (lastCorrect) score++;

          QuizState prev = state;
          state = QS_FEEDBACK;
          logStateChange(prev, state);

          char given[8], expected[8];
          snprintf(given,    sizeof(given),    "%c", 'A' + selectedIndex);
          snprintf(expected, sizeof(expected), "%c", 'A' + q.ans);
          logAnswer(lastCorrect, given, expected);

          if (lastCorrect) {
            beepCorrect();
            stamp(); Serial.println(F("[BUZZ  ] BEEP (1x, 2200 Hz)  — correct"));
          } else {
            beepWrong();
            stamp(); Serial.println(F("[BUZZ  ] BEEP BEEP BEEP (3x, 380 Hz)  — wrong"));
          }
        }
      } else {
        logKey(key, "Ignored (not a valid MCQ key)");
      }
    }
    else {                          // ---------- NUMERIC ----------
      if (key >= '0' && key <= '9') {
        if (numBuffer.length() < 6) {
          numBuffer += key;
          char msg[64];
          snprintf(msg, sizeof(msg),
                   "Append digit (buffer=\"%s\")", numBuffer.c_str());
          logKey(key, msg);
        } else {
          logKey(key, "Ignored (buffer full, max 6 digits)");
        }

      } else if (key == '*') {
        if (numBuffer.length() > 0) {
          numBuffer.remove(numBuffer.length() - 1);
          char msg[64];
          snprintf(msg, sizeof(msg),
                   "Backspace (buffer=\"%s\")", numBuffer.c_str());
          logKey(key, msg);
        } else {
          logKey(key, "Ignored (* — buffer already empty)");
        }

      } else if (key == '#') {
        if (numBuffer.length() == 0) {
          logKey(key, "Ignored (# — no digits typed yet)");
        } else {
          char msg[64];
          snprintf(msg, sizeof(msg),
                   "Submit numeric (value=%s)", numBuffer.c_str());
          logKey(key, msg);

          int val = numBuffer.toInt();
          lastCorrect = (val == q.ans);
          if (lastCorrect) score++;

          QuizState prev = state;
          state = QS_FEEDBACK;
          logStateChange(prev, state);

          char expected[10];
          snprintf(expected, sizeof(expected), "%d", q.ans);
          logAnswer(lastCorrect, numBuffer.c_str(), expected);

          if (lastCorrect) {
            beepCorrect();
            stamp(); Serial.println(F("[BUZZ  ] BEEP (1x, 2200 Hz)  — correct"));
          } else {
            beepWrong();
            stamp(); Serial.println(F("[BUZZ  ] BEEP BEEP BEEP (3x, 380 Hz)  — wrong"));
          }
        }
      } else {
        logKey(key, "Ignored (not a valid numeric key)");
      }
    }
    return;
  }

  // ---------- FEEDBACK ----------
  if (state == QS_FEEDBACK) {
    if (key == '#') {
      logKey(key, "Advance to next");
      qIndex++;

      if (qIndex >= TOTAL_Q) {
        QuizState prev = state;
        state = QS_FINISHED;
        logStateChange(prev, state);

        stamp();
        Serial.println(F("+----------------------------------------------------------+"));
        stamp();
        Serial.printf ( "[RESULT] FINAL SCORE: %d / %d\n", score, TOTAL_Q);
        stamp();
        Serial.println(F("+----------------------------------------------------------+"));
        stamp();
        Serial.println(F("[SYSTEM] Press # to play again"));
      } else {
        selectedIndex = -1;
        numBuffer     = "";
        QuizState prev = state;
        state = QS_QUESTION;
        logStateChange(prev, state);
        logQuestionHeader();
      }
    } else {
      logKey(key, "Ignored (in feedback — press # to continue)");
    }
    return;
  }
}

// ============================================================
// Web handlers
// ============================================================
void handleRoot() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.send_P(200, "text/html", PAGE_HTML);
}

void handleState() {
  StaticJsonDocument<2048> doc;

  doc["total"] = TOTAL_Q;
  doc["score"] = score;
  doc["index"] = qIndex;

  if (state == QS_IDLE) {
    doc["state"] = "idle";
  }
  else if (state == QS_FINISHED) {
    doc["state"] = "finished";
    doc["index"] = TOTAL_Q;
  }
  else {
    const Question& q = QUESTIONS[qIndex];
    doc["state"] = (state == QS_FEEDBACK) ? "feedback" : "question";

    if (q.type == 0) {
      doc["type"]     = "mcq";
      doc["question"] = q.q;
      JsonArray opts  = doc.createNestedArray("options");
      opts.add(q.a);  opts.add(q.b);
      opts.add(q.c);  opts.add(q.d);
      doc["selectedIndex"] = selectedIndex;

      if (state == QS_FEEDBACK) {
        doc["correctIndex"]  = q.ans;
        doc["correct"]       = lastCorrect;
        doc["correctAnswer"] = correctAnswerString(q);
      }
    } else {
      doc["type"]     = "num";
      doc["question"] = q.q;
      doc["buffer"]   = numBuffer;

      if (state == QS_FEEDBACK) {
        doc["correct"]       = lastCorrect;
        doc["correctAnswer"] = correctAnswerString(q);
      }
    }
  }

  String out;
  serializeJson(doc, out);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.send(200, "application/json", out);
}

void handleNotFound() {
  server.sendHeader("Location",
    String("http://") + WiFi.softAPIP().toString() + "/", true);
  server.send(302, "text/plain", "");
}

// ============================================================
// Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  delay(200);

  printBootBanner();

  // Buzzer boot chime
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  beepBoot();
  stamp(); Serial.println(F("[BUZZ  ] Boot chime (2x, 1500 Hz)"));

  // Wi-Fi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress ip = WiFi.softAPIP();

  printWifiBanner(ip);

  dns.start(53, "*", ip);

  server.on("/",          handleRoot);
  server.on("/api/state", handleState);
  server.onNotFound(handleNotFound);
  server.begin();

  stamp(); Serial.println(F("[SYSTEM] HTTP server started on port 80"));
  stamp(); Serial.println(F("[SYSTEM] Quiz reset · state = QS_IDLE"));
  stamp(); Serial.println(F("[SYSTEM] Ready — press # on the keypad to begin"));
  stamp(); Serial.println(F("[SYSTEM] --------------------------------------------------"));
  Serial.println();

  resetQuiz();
  state = QS_IDLE;
}

// ============================================================
// Loop
// ============================================================
void loop() {
  dns.processNextRequest();
  server.handleClient();

  updateBuzzer();

  char key = keypad.getKey();
  if (key) handleKey(key);
}