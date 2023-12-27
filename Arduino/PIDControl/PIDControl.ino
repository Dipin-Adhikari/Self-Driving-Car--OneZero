// Motor A connections. A = (right)
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B connections. B = (left)
int enB = 3;
int in3 = 5;
int in4 = 4;

// declaring variables
int slope, error, pError, p, i, d, lSpeed, rSpeed;

// declaring constants
int baseSpeed = 255;
float kp = 3.242;
float kd = 2;
float ki = 0.005;


void setup() {
  Serial.begin(9600);
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  // Check anything available from serial python
  if (Serial.available()>0){
    int slope = Serial.read();

    // Stopping condition
    if (slope == 200){
      Stop();
    }
    else {
      // The slope needs to be 90 degrees (a straight line) every time. So, anything other than that is considered an error.
      error = 90 - slope;
      PID(error);
  }
  }
}

// Function to stop the car.
void Stop(){
  analogWrite(enA, baseSpeed);
  analogWrite(enB, baseSpeed);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


void PID(int error) {
  
  /* 
  * It uses the three control terms of proportional, integral, and derivative influence on the controller output to apply accurate and optimal control.

  * Proportional represents the error and controls how the system responds to curves.
  * Integral eliminates residual error by applying a control effect. Once the error is eliminated, the integral term stops growing.
  * Derivative calculates how fast the error is changing by differencing the current error from the previous error.
  
  * 'kp,' 'ki,' and 'kd' are constants.
  * A small 'kp' will result in a gentler response to curves, almost moving in a straight line. On the other hand, a large 'kp' will cause sudden responses, leading to oscillations.
  * A small 'ki' may have little impact, while a large 'ki' can induce oscillations.
  * Similarly, a small 'kd' may not have a significant impact, but a large 'kd' can cause oscillations and erratic movement.
  */
  
  p = error;
  i = i + error;
  d = error - pError;
  pError = error;

  float pid = kp * p + ki * i + kd * d;
  Serial.println(pid);

  /*
  * 'baseSpeed' represents the speed when the car moves in a straight line.
  * 'lSpeed' is the speed of motor A, and 'rSpeed' is the speed of motor B.

  * 'lSpeed' is calculated by subtracting the PID term from 'baseSpeed'. When turning left, the slope is less than 90, causing the error and PID to be positive. Consequently, 'lSpeed' decreases, while 'rSpeed' remains at 255.
  * 'rSpeed' is calculated by adding the PID term to 'baseSpeed'. When turning right, the slope is more than 90, resulting in a negative error and PID. As a result, 'lSpeed' stays at 255, and 'rSpeed' decreases.
  */
  
  lSpeed = baseSpeed - pid;
  rSpeed = baseSpeed + pid;
  
  // This will bound the values between 0 and 255.
  if (lSpeed < 0){
      lSpeed = 0;
  }
  if (lSpeed > 255){
      lSpeed = 255;
  }
  if (rSpeed < 0){
      rSpeed = 0;
  }
  if (rSpeed > 255){
      rSpeed = 255;
  }
  // Calling the function to drive the motors.
  drive(lSpeed, rSpeed);
}


void drive(int l, int r) {
  
  /*
  * To turn the car in a specified direction, the motor on that side should either move backward, move forward with a lower speed, or stop, while the motor on the opposite side moves forward with a higher speed. This combination creates a turning effect that rotates the car.
  
  * Here, the arguments 'l' and 'r' represent the speed of motors A and B, respectively.
  
  * If the value of 'r' is greater than 127, motor B will move forward with a speed of 'r'; otherwise, it will move backward with a speed of 'r'.
  
  * Similarly, if the value of 'l' is greater than 127, motor A will move forward with a speed of 'l'; otherwise, it will move backward with a speed of 'l'.
  
  * There are four possibilities:
     1. Both motors moving forward.
     2. Both motors moving backward.
     3. One motor moving forward and another moving backward, and vice versa.
  
  * If the turn is small, both motors will move forward with different speeds, producing a small turning effect. For a larger turn, they will move in opposite directions to create a more significant turning effect.
   */
   
  if (r > 127){
    analogWrite(enB, r); 
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else{
    analogWrite(enB, r);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  if (l > 127) {
    analogWrite(enA, l); 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else{
    analogWrite(enA, l); 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
}
