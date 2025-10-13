//James Dong

const int ledPin1 = 8;
const int ledPin2 = 7;
const int ledPin3 = 6;

const int buttonPin = 2;
int buttonState = 0;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    digitalWrite(ledPin1, HIGH);  
    delay(1000);
    digitalWrite(ledPin1, LOW);
    delay(1000);

    digitalWrite(ledPin2, HIGH); 
    delay(1000);
    digitalWrite(ledPin2, LOW);
    delay(1000); 

    digitalWrite(ledPin3, HIGH);  
    delay(1000);
    digitalWrite(ledPin3, LOW);
    delay(1000); 

  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
  }
}
