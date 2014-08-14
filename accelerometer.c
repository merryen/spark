int Zout = A6;
int Xout = A7;
int Yout = A1;

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.print("X = ");
    Serial.print(analogRead(Xout));
    Serial.print(" ");
    Serial.print("Y = ");
    Serial.print(analogRead(Yout));
    Serial.print(" ");
    Serial.print("Z = ");
    Serial.println(analogRead(Zout));
    Delay(1000);
}