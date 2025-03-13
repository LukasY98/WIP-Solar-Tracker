// Variables
int sensorReadingFront = 0;
int sensorReadingL1 = 0;
int sensorReadingR1 = 0;
int sensorReadingL2 = 0;
int sensorReadingR2 = 0;
int sensorReadingBack = 0;

int highestIndex = 0;
int highestValue = 0;

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
    // Front sensor has the highest value
    if (sensorReadings[1] > sensorReadings[2]) // Light source is slightly left of the front sensor
    {
      int diff = 0;
      diff = sensorReadings[1] - sensorReadings[2];
      int error = 0;
      error = map(diff, 0, 1023, 0, 100);
      return error;
    }
    if (sensorReadings[2] > sensorReadings[1]) // Light source is slightly right of the front sensor
    {
      int diff = 0;
      diff = sensorReadings[2] - sensorReadings[1];
      int error = 0;
      error = map(diff, 0, 1023, 0, -100);
      return error;
    }
    else
    {
      return 0;
    }
  case 1:
    // Left sensor 1 has the highest value
    return 100;
  case 2:
    // Right sensor 1 has the highest value
    return -100;
  case 3:
    // Left sensor 2 has the highest value
    return 200;
  case 4:
    // Right sensor 2 has the highest value
    return -200;
  case 5:
    // Back sensor has the highest value
    if (sensorReadings[3] > sensorReadings[4])
    {
      return 300;
    }
    else
    {
      return -300;
    }
  default:
    Serial.println("Unknown sensor index");
    return 0;
}
}

void printSensorData()
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
}



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
  
  printSensorData();
  
  int error = calcError(sensorReadings);
  Serial.println(error);

  delay(1000);
}
