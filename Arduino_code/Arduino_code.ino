#include <FileIO.h> // INCLUDING FILE INPUT OUTPUT LIBRARY //
#include <Wire.h>   // INCLUDE WIRE LIBRARY FOR I2C PROTOCOL //
#include <Servo.h>  // INCLUDE SERVO LIBRARY TO RUN SERVO MOTORS //


// Initializing Analog Read pins //
int AnalogPin1 = A0;   // POT 1 angle position of motor 1
int AnalogPin2 = A1;   // POT 2 angle position of motor 2
int AnalogPin3 = A2;   // POT 3 angle position of motor 3
int AnalogPin4 = A3;   // FOOTSWITCH 1
int AnalogPin5 = A4;   // FOOTSWITCH 2

// TO STORE VALUES OF FOOT SWITCHES //
int foot_switch_to;
int foot_switch_hs;


// INITIALIZING THE GAIT PATTERN HIP,KNEE,ANKLE FORMAT FOR 51 CYCLES //
int angles[] = {5, 3, 35, 5, 2, 34, 7, 2, 34, 10, 2, 33, 12, 2, 32, 14, 2, 31, 16, 3, 30, 16, 5, 28, 15, 6, 26, 15, 7, 24, 14, 7, 22, 12, 8, 20, 11, 9, 18, 10, 9, 16, 9, 9, 14, 8, 10, 12, 7, 10, 10, 6, 11, 8, 5, 11, 6, 5, 12, 5, 4, 12, 3, 4, 13, 1, 4, 13, -1, 4, 13, -3, 5, 13, -4, 6, 13, -5, 8, 12, -6, 11, 10, -7, 14, 8, -7, 18, 4, -7, 22, 0, -5, 27, -3, -3, 33, -7, -1, 38, -8, 3, 43, -9, 7, 48, -8, 11, 51, -6, 15, 52, -3, 18, 52, -1, 22, 50, 1, 24, 47, 3, 27, 44, 4, 29, 39, 5, 31, 34, 6, 33, 29, 6, 34, 24, 5, 35, 19, 5, 36, 14, 5, 36, 10, 4, 36, 7, 4, 36, 5, 3, 35
               };

// LOCAL VARIABLES DECLARATION //
int hip_angle, knee_angle, ankle_angle, val, angleData, t, q, p,angledata, m, z, n, i,l;
String dataString;
String send_data;

// BYTES TO FORM FINAL DATA, USED TO SEND TO MATLAB IN REAL TIME //
String start_byte = "9999";
String end_byte = "7777";
String final_byte;
String angle_delimiter = ",";
String motor_hip_id = "5001";
String motor_knee_id = "5002";
String motor_ankle_id = "5003";
String command_delimiter = ".";
String foot_switch_to_id = "5004";
String foot_switch_hs_id = "5005";


// DECLARATION OF DATA FILES WHICH IS USED TO ACCESS TXT FILES ON SD CARD //
File dataFile;
File dataFile2;



// INITIALZING MOTOR ANGLES TO START POSITION //
int h_servo_angle = 0;
int k_servo_angle = 0;
int a_servo_angle = 0;


// SERVO MOTOR DECLARATION //
Servo myservo1;
Servo myservo2;
Servo myservo3;


// SET MAX AND MIN VALUES FOR HIP, KNEE AND ANKLE //
int hip_max_angle = 45;
int hip_min_angle = -20;
int knee_max_angle = 60;
int knee_min_angle = -5;
int ankle_max_angle = 40;
int ankle_min_angle = -30;

void setup() {

  // INITIALIZE THE BRIDGE AND SERIAL //
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();
  while (!Serial); // WAIT UNTILL SERIAL COMMUNICATION IS SETUP //
  Serial.println("Filesystem datalogger\n");
  myservo1.attach(9);
  myservo2.attach(10);
  myservo3.attach(11);
  
  //Start I2C //
  for(l = 0;l<153;l++)
  {
    
      Wire.begin();
      Wire.beginTransmission(44);
      Wire.write(angles[l]);
      Wire.endTransmission();
  }

  //TIME STAMPING IN  "Realtime.txt file" //
  String Timestamp;
  Timestamp += getTimeStamp();
  Timestamp += " = ";
  File dataFile2 = FileSystem.open("/mnt/sd/realtime.txt", FILE_APPEND);
  if (dataFile2)
  {
    dataFile2.println(Timestamp);
    dataFile2.close();
  }
}

void loop()
{
  if (digitalRead(12) == HIGH)
  {
    for (i = 0; i < 153; i = i + 3)
    {
      k_servo_angle = angles[i];
      a_servo_angle = angles[i + 1];
      h_servo_angle = angles[i + 2];
      myservo1.write(h_servo_angle + 10); // WRITE ANGLE VALUES TO MOTORS WITH OFFSET 10 SINCE WE HAVE NEGATIVE VALUES AT HIP
      myservo2.write(k_servo_angle + 0); // WRITE ANGLE VALUES TO MOTORS WITH OFFSET 0 SINCE WE DO NOT HAVE NEGATIVE VALUES AT KNEE
      myservo3.write(a_servo_angle + 10); // WRITE ANGLE VALUES TO MOTORS WITH OFFSET 10 SINCE WE HAVE NEGATIVE VALUES AT ANKLE



      // myservo1.write(0);   // REQUIRED FOR CALIBRATION
      // myservo2.write(0);   // REQUIRED FOR CALIBRATION
      // myservo3.write(0);   // REQUIRED FOR CALIBRATION
      delay(65);

      // READ HIP MOTOR ANGLE FROM POT AT HIP //
      hip_angle = analogRead(AnalogPin1);
      hip_angle = map(hip_angle, 0, 1024, 0, 180); // MAPPING READ ANALOG VALUE TO ANGLE //
      hip_angle = 1000 + hip_angle;

      if (hip_angle > hip_max_angle || hip_angle < hip_min_angle) // SAFETY FEATURE IF POTS DETECT ABNORMAL ANGLE AT HIP //
      {
        exit(0);
      }

      // READ HIP MOTOR ANGLE FROM POT AT HIP //
      knee_angle = analogRead(AnalogPin2);
      knee_angle = map(knee_angle, 0, 1024, 0, 180); // MAPPING READ ANALOG VALUE TO ANGLE //
      knee_angle = 1000 + knee_angle;

      if (knee_angle > knee_max_angle || knee_angle < knee_min_angle) // SAFETY FEATURE IF POTS DETECT ABNORMAL ANGLE AT HIP //
      {
        exit(0);
      }

      // READ HIP MOTOR ANGLE FROM POT AT HIP //
      ankle_angle = analogRead(AnalogPin3);
      ankle_angle = map(ankle_angle, 0, 1024, 0, 180); // MAPPING READ ANALOG VALUE TO ANGLE //
      ankle_angle = 1000 + ankle_angle;

      if (ankle_angle > ankle_max_angle || ankle_angle < ankle_min_angle) // SAFETY FEATURE IF POTS DETECT ABNORMAL ANGLE AT HIP //
      {
        exit(0);
      }

      // READ FOOTSWITCH AT HEEL //
      foot_switch_hs = analogRead(AnalogPin4);
      if (foot_switch_hs > 10) // CALIBRATING PARAMETER WITH RESPECT TO WEIGHT OF PERSON //
      {
        foot_switch_hs = 1;
      }
      else
      {
        foot_switch_hs = 0;
      }

      // READ FOOTSWITCH AT HEEL //
      foot_switch_to = analogRead(AnalogPin5);
      if (foot_switch_to > 10)  // CALIBRATING PARAMETER WITH RESPECT TO WEIGHT OF PERSON //
      {
        foot_switch_to = 1;
      }
      else
      {
        foot_switch_to = 0;
      }


      // FINAL DATA BYTE TO SEND TO MATLAB //
      final_byte = start_byte + angle_delimiter +
                   motor_hip_id + angle_delimiter + hip_angle + angle_delimiter +
                   motor_knee_id + angle_delimiter + knee_angle + angle_delimiter +
                   motor_ankle_id + angle_delimiter + ankle_angle + angle_delimiter +
                   foot_switch_hs_id + angle_delimiter + foot_switch_hs + angle_delimiter +
                   foot_switch_to_id + angle_delimiter + foot_switch_to + angle_delimiter + end_byte + command_delimiter;


      Serial.println(final_byte);  // SENDING TO SERIAL PORT IN REAL TIME //


      // WRITING REALTIME READINGS TO A FILE "REALTIME.TXT" IN THE SD CARD //
      File dataFile = FileSystem.open("/mnt/sd/realtime.txt", FILE_APPEND);
      if (dataFile)
      {
        dataFile.println(final_byte);
        dataFile.close();
      }
      else
      {
        Serial.println("error opening realtime.txt");
      }
    }
  }
}


// FUNCTION  TO GET TIME STAMP //

String getTimeStamp() 
{
  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%D-%T");
  time.run(); 
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n')
      result += c;
  }

  return result;
}
// END OF FILE //


// EXTRAS TO READ FROM SD CARD //
/*
if (digitalRead(12) == HIGH)
  {
    File dataFile = FileSystem.open("/mnt/sd/motorangles.txt");
    while (digitalRead(12) == HIGH)
    {
      send_data = dataFile.read();
      if(send_data == -1)
      {
        break;
      }
      else if (send_data == 13 || send_data == 10)
      {
        continue;
      }
      
      else
      {
      switch (send_data)
      {
        case 48 : send_data = 0; break;
        case 49 : send_data = 1; break;
        case 50 : send_data = 2; break;
        case 51 : send_data = 3; break;
        case 52 : send_data = 4; break;
        case 53 : send_data = 5; break;
        case 54 : send_data = 6; break;
        case 55 : send_data = 7; break;
        case 56 : send_data = 8; break;
        case 57 : send_data = 9; break;
        case 44 : send_data = ','; break;
        case 46 : send_data = '.'; break;
        
      }
      //collecting_data += send_data;
      //z = z + 1;
      //if (z == 3)
      //{
      //  Serial.println(collecting_data);
      //  z = 0;
      //  collecting_data = "";
        delay(100);
      //}
      Serial.print(send_data);
    }
    }
    dataFile.close();
  }
  */
  
  
  
//EXTRAS TO WRITE TO SD CARD FROM SERIAL INPUT //
/*
  if (digitalRead(13) == HIGH)
  {
    File dataFile = FileSystem.open("/mnt/sd/motorangles.txt", FILE_WRITE);
    //Serial.println("its in the deleting pattern");
    FileSystem.remove("motorangles.txt");
  }

  while (digitalRead(13) == HIGH)
  {

    if (Serial.available() > 0)
    {
     
      // TO SEND DATA TO MASTER CONTROLLER //
     
      //Start I2C //
      Wire.begin();
      Wire.beginTransmission(44);
      Wire.write(angleData);
      Wire.endTransmission();
      // End I2C//

      // CONVERTING ASCII TO DECIMAL //
      
      angleData = Serial.read();
      switch (angleData)
      {
        case 48 : angleData = 0; break;
        case 49 : angleData = 1; break;
        case 50 : angleData = 2; break;
        case 51 : angleData = 3; break;
        case 52 : angleData = 4; break;
        case 53 : angleData = 5; break;
        case 54 : angleData = 6; break;
        case 55 : angleData = 7; break;
        case 56 : angleData = 8; break;
        case 57 : angleData = 9; break;
      }

      dataString += String(angleData);
      if (t == 2)
      {
        t = 0;
        if (p == 2)
        {
          dataString += ".";
          p = 0;
        }
        else
        {
          dataString += ",";
          p += 1;
        }
      }
      else
      {
        t += 1;
      }

      File dataFile = FileSystem.open("/mnt/sd/motorangles.txt", FILE_APPEND);
      if (dataFile)
      {
        if (q == 8)
        {
          Serial.println("in the writing loop");
          dataFile.println(dataString);
          dataFile.close();
          Serial.println(dataString);
          dataString = "";
          q = 0;
        }
        else
        {
          q = q + 1;
        }
      }
      // if the file isn't open, pop up an error: //
      else
      {
        Serial.println("error opening motorangles.txt");
      }
    }
  }
  */
