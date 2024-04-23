#define pin_A 2 // encoder channel pins
#define pin_B 4
#define pin_rwd 8 // reward pin
#define pin_pho 3 // photosensor pin
#define enc_pls 1000.0 //number of pulses per revolution of encoder
#define rad 5.0 //radius of wheel
#define DELAY 50 //reward valve open time in ms
#define min_d 30.0 // minimum distance traveled for reward to avoid cheating
#define neg_d -30.0 // reset negative distance for animals walking backwards

int laps = 0;
int steps;
float d;

unsigned long rwd_t = 0xffffffff;
bool A, B;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(pin_A), stepper, CHANGE);
  pinMode(pin_B, INPUT);

  pinMode(pin_rwd, OUTPUT);
  digitalWrite(pin_rwd, LOW);

  attachInterrupt(digitalPinToInterrupt(pin_pho), reward, RISING);

  reset();
  d = min_d + 1;
  reward();
  reset();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(d);

  if ((millis() - rwd_t) > DELAY)
    digitalWrite(pin_rwd, LOW);
    

  if (d < neg_d)
    reset();

    delay(1);
}

void stepper() {
  A = digitalRead(pin_A);
  B = digitalRead(pin_B);
  A == B ? steps++ : steps--;
  d = (float) steps / enc_pls * 2.0 * PI * rad;
  

//  if (d > 30) {
//    reward();
//    steps = 0;
//    d = 0.0;
//  }
}

void reward() {
  if (d > min_d) {
    digitalWrite(pin_rwd, HIGH);
    rwd_t = millis();
    Serial.print("lap no. ");
    Serial.print(laps);
    Serial.print("\n");
    Serial.print("dist. ");
    Serial.print(d);
    Serial.print("\n");
    reset();
    laps++;
  }
}

void reset() {
  d = 0.0;
  steps = 0;
}
