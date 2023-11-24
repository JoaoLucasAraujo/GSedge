//Joao Lucas Araujo
#define PULSE_PER_BEAT    1   
#define INTERRUPT_PIN     5          
#define SAMPLING_INTERVAL 1000    

volatile uint16_t pulse;              
uint16_t count;                      

float heartRate;            

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  

void IRAM_ATTR HeartRateInterrupt() {
  portENTER_CRITICAL_ISR(&mux);  
  pulse++;
  portEXIT_CRITICAL_ISR(&mux); 
}

void setup() {
  Serial.begin(115200);

  pinMode(INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), HeartRateInterrupt, RISING);  
}

void loop() {
  HeartRate();
}

void HeartRate() {
  static unsigned long startTime;
  if (millis() - startTime < SAMPLING_INTERVAL) return;
  startTime = millis();

  portENTER_CRITICAL(&mux); 
  count = pulse;  
  pulse = 0;
  portEXIT_CRITICAL(&mux); 


  heartRate = map(count, 0, 220, 0, 220);

  Serial.println("Heart Rate: " + String(heartRate, 2) + " BPM");
}
