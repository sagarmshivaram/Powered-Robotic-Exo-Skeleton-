int incomingByte = 0;   // for incoming serial data

void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
analogWrite(9,30);
digitalWrite(11,0);
}

void loop() {

        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
                if(incomingByte==48) // Check for ASCII Value for 0
               {
               digitalWrite(10,LOW); // Setting Pin 10 to 0
               }
               else if(incomingByte==49) //Check for ASCII value for 1
               {
               digitalWrite(10,HIGH); // Setting Pin 10 to 0
               }
               else if(incomingByte==50) //Check for ASCII Value for 2
               {
               analogWrite(9,50);  // Writing Analog value of 50 to Pin 9
               }
               else if(incomingByte==51) //Check for ASCII Value for 3
               {
               analogWrite(9,150);  // Writing Analog value of 150 to Pin 9
               }
               else if(incomingByte==52) //Check for ASCII Value for 4
               {
               analogWrite(9,250); // Writing Analog value of 250 to Pin 9
               }
               else if(incomingByte==53) //Check for ASCII Value for 5
               digitalWrite(11,LOW); // Change Direction to clock wise when Pin 11 is set to 0
               }
               else if(incomingByte==54)  //Check for ASCII Value for 6
               {
               digitalWrite(11,HIGH); // Change Direction to anticlock wise when Pin 11 is set to 1
               }
        }
}
