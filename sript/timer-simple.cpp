#define OnB_LED 2
boolean togLED = false;

volatile int timeCounter1;
hw_timer_t *timer1 = NULL; 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer1(){

  portENTER_CRITICAL_ISR(&timerMux);
  timeCounter1++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  pinMode(OnB_LED, OUTPUT);
  Serial.begin(115200);

  timer1 = timerBegin(0, 80, true);

  timerAttachInterrupt(timer1, &onTimer1, true);

  timerAlarmWrite(timer1, 1000000, true);

  timerAlarmEnable(timer1);

  Serial.println("Start!");
}

void loop() {

  if (timeCounter1 > 0) {
    portENTER_CRITICAL(&timerMux);
    timeCounter1--;
    portEXIT_CRITICAL(&timerMux);
    RoutineWork();
  }
}

/****************** Interrupt handler *******************/

void RoutineWork()
{
  Serial.print("interrupted   ");
  Serial.print("togLED = ");
  Serial.println(togLED);

  if (togLED == true) {
    digitalWrite(OnB_LED, LOW);
    togLED = false;
  }else{
    digitalWrite(OnB_LED, HIGH);
    togLED = true;
  }
}