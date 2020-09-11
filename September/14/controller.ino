void setup(){
  // initialize light pins
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  
  // initialize RNG (assumes pin 0 is empty)
  randomSeed(analogRead(0));
}

void loop(){
  // get button presses
  int b1 = digitalRead(5);
  int b2 = digitalRead(4);
  
  // button 1 (pin 5) is pressed
  if(b1){
    // alternates lights at regular intervals
    // of 100sec as long as button 1 is held
    if(digitalRead(13)){
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
    }
    else {
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
    }
    delay(100);
  }
  
  // button 2 (pin 4) is pressed
  else if(b2){
    // alternates lights at random intervals
    // switching between 100sec and a
    // random delay of 100-500sec as long as
    // button 1 is held
    if(digitalRead(13)){
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);
      delay(random(100, 1000));
    }
    else {
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
      delay(100);
    }
  }
  
  // no buttons are pressed
  else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
  }
}
