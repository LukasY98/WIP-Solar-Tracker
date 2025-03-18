// Constants
const int MAX_SENSOR_READING = 690; //TODO: Adjust for real world values


// Variables
int sensorReadingFront = 0;
int sensorReadingL1 = 0;
int sensorReadingR1 = 0;
int sensorReadingL2 = 0;
int sensorReadingR2 = 0;
int sensorReadingBack = 0;

int highestIndex = 0;
int highestValue = 0;

int integral = 0;
int lastError = 0;

float previousTime = 0.0;

float Kp = 1.0;
float Ki = 0.5;
float Kd = 0.2;


// Helper Functions
void readSensorData()
{
  sensorReadingFront = analogRead(A0);
  sensorReadingL1 = analogRead(A1);
  sensorReadingR1 = analogRead(A2);
  sensorReadingL2 = analogRead(A3);
  sensorReadingR2 = analogRead(A4);
  sensorReadingBack = analogRead(A5);
}

void findHighestValue(int sensorReadings[])
{
  highestValue = sensorReadings[0];
  highestIndex = 0;
  for (int i = 1; i < 6; i++) {
    if (sensorReadings[i] > highestValue) {
      highestValue = sensorReadings[i];
      highestIndex = i;
    }
  }
}

int calcError(int sensorReadings[])
{
  switch (highestIndex) {
  case 0:
    {
    // Front sensor has the highest value
    int diff = sensorReadings[1] - sensorReadings[2];
    int maxDiff = MAX_SENSOR_READING;
    int error = map(diff, -maxDiff, maxDiff, -100, 100);
    return error;
    }
  case 1:
    {
    // Left sensor 1 has the highest value
    int diff = sensorReadings[3] - sensorReadings[0];
    int maxDiff = MAX_SENSOR_READING;
    int error = map(diff, -maxDiff, maxDiff, 100, 300);
    return error;
    }
  case 2:
    {
    // Right sensor 1 has the highest value
    int diff = sensorReadings[4] - sensorReadings[0];
    int maxDiff = MAX_SENSOR_READING;
    int error = map(diff, -maxDiff, maxDiff, -100, -300);
    return error;
    }
  case 3:
    {
    // Left sensor 2 has the highest value
    int diff = sensorReadings[5] - sensorReadings[1];
    int maxDiff = MAX_SENSOR_READING;
    int error = map(diff, -maxDiff, maxDiff, 300, 500);
    return error;
    }
  case 4:
    {
    // Right sensor 2 has the highest value
    int diff = sensorReadings[5] - sensorReadings[2];
    int maxDiff = MAX_SENSOR_READING;
    int error = map(diff, -maxDiff, maxDiff, -300, -500);
    return error;
    }
  case 5:
    {
    // Back sensor has the highest value
    if (sensorReadings[3] > sensorReadings[4]) //Light source is slightly left of the back sensor
    {
      int diff = sensorReadings[3] - sensorReadings[4];
      int maxDiff = MAX_SENSOR_READING;
      int error = map(diff, 0, maxDiff, 600, 500);
      return error;
    }
    else //Light source is slightly right of the back sensor
    {
      int diff = sensorReadings[4] - sensorReadings[3];
      int maxDiff = MAX_SENSOR_READING;
      int error = map(diff, 0, maxDiff, -600, -500);
      return error;
    }
    }
  default:
    {
    Serial.println("Unknown sensor index");
    return 0;
    }
  }
}

void calcIntegral(int error)
{
  // If the sign (+/-) of the error changes or the error is 0, reset the integral
  if ((error > 0) & (integral < 0) | (error < 0) & (integral > 0) | error == 0)
  {
    integral = 0;
  }
  integral += error;
}

float calcDerivative(int error)
{
  float currentTime = millis();
  float deltaTime = (currentTime - previousTime) / 1000.0; // Convert to seconds
  previousTime = currentTime;
  if (deltaTime > 0)
  {
    return (error - lastError) / deltaTime;
  }
}

void printData(int error, float derivative, float output)
{
  Serial.print("Sensor Front: ");
  Serial.print(sensorReadingFront);
  Serial.print(", Sensor L1: ");
  Serial.print(sensorReadingL1);
  Serial.print(", Sensor R1: ");
  Serial.print(sensorReadingR1);
  Serial.print(", Sensor L2: ");
  Serial.print(sensorReadingL2);
  Serial.print(", Sensor R2: ");
  Serial.print(sensorReadingR2);
  Serial.print(", Sensor Back: ");
  Serial.println(sensorReadingBack);
  Serial.print("Highest Sensor Value: ");
  Serial.print(highestValue);
  Serial.print(" (Sensor: ");
  Serial.print(highestIndex);
  Serial.println(")");
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" Integral: ");
  Serial.print(integral);
  Serial.print(" Derivative: ");
  Serial.println(derivative);
  Serial.print("PID: ");
  Serial.println(output);
}



// Main Functions
void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
}

void loop()
{
  readSensorData();
  int sensorReadings[] = {sensorReadingFront, sensorReadingL1, sensorReadingR1, sensorReadingL2, sensorReadingR2, sensorReadingBack};
  
  findHighestValue(sensorReadings);
  
  int error = calcError(sensorReadings);
  calcIntegral(error);
  float derivative = calcDerivative(error);

  float output = (Kp * error) + (Ki * integral) + (Kd * derivative);
  
  printData(error, derivative, output);
  
  lastError = error;
  delay(10); //increase for testing
}
