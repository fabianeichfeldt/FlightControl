#include "RemoteControl.h"
#include "Arduino.h"

#define BUFFERSIZE 1000

RemoteControl::RemoteControl() {
  config = ConfigData::GetDefault();
  control = ControlData::GetDefault();
}

void RemoteControl::Init()
{
  Serial1.begin(115200);
  delay(300);
  Serial1.write("AT\r\n");
  delay(400);
  Serial1.write("AT+CWMODE=2\r\n");
  delay(400);
  Serial1.write("AT+CIFSR\r\n");
  delay(400);
  Serial1.write("AT+CIPMUX=1\r\n");
  delay(400);
  Serial1.write("AT+CIPSERVER=1,443\r\n");
  delay(400);
}

void RemoteControl::ReadFromSerial()
{
   if(Serial.available())
   {
     int input = Serial.parseInt();
     while(Serial.read() != '\n')
       ;
     if(input == 1)
       control.SetSpeed(control.GetSpeed() + 51);
     else if (input == 8)
     {
       control.SetRotation(Rotation(control.GetRotation().GetX() - 3, control.GetRotation().GetY() + 3, 0));
     }
     else if (input == 5)
     {
       control.SetRotation(Rotation(control.GetRotation().GetX() + 3, control.GetRotation().GetY() - 3, 0));
     }
     else
       control.SetSpeed(control.GetSpeed() - 51);
   }
}

void RemoteControl::ReadData()
{
  byte i;
  byte buffer[BUFFERSIZE];
  byte availableBytes = 0, firstRelevantByte=0;

  //normal data: +IPD,xx:<3Bytes StartData><12BytesData>
  availableBytes = Serial1.available();
  if(availableBytes > 22)
  {
    if(availableBytes > BUFFERSIZE)
    {
      availableBytes = BUFFERSIZE;
    }
    Serial1.readBytes(buffer, availableBytes);

    for(i=0; i<availableBytes-14;i++)
    {
      if((buffer[i] == 0xf0) && (buffer[i+1] == 0xf0) && (buffer[i+2] == 0xf0))
      {
        firstRelevantByte = i+3;
        int motorSpeed = (buffer[firstRelevantByte] << 16) | (buffer[firstRelevantByte+1] << 8) | (buffer[firstRelevantByte+2]);
        int sollX = (buffer[firstRelevantByte+3] << 16) | (buffer[firstRelevantByte+4] << 8) | (buffer[firstRelevantByte+5]);
        int sollY = (buffer[firstRelevantByte+6] << 16) | (buffer[firstRelevantByte+7] << 8) | (buffer[firstRelevantByte+8]);
        int sollZ = (buffer[firstRelevantByte+9] << 16) | (buffer[firstRelevantByte+10] << 8) | (buffer[firstRelevantByte+11]);

        control.SetRotation(Rotation((sollX - 1500)/50, (sollY - 1500)/-50, 0));
        control.SetSpeed(motorSpeed);

        break;
      }
      /*   else if((buffer[i] == 0xf1) && (buffer[i+1] == 0xf1) && (buffer[i+2] == 0xf1) && motorSpeed == MOTOR_MIN && (i+23)<availableBytes)
        {
          firstRelevantByte = i+3;

          KP_X = GetFloatFromSerial(&buffer[firstRelevantByte]);
          KI_X = GetFloatFromSerial(&buffer[firstRelevantByte+4]);
          KD_X = GetFloatFromSerial(&buffer[firstRelevantByte+8]);
          KP_Z = GetFloatFromSerial(&buffer[firstRelevantByte+12]);
          KI_Z = GetFloatFromSerial(&buffer[firstRelevantByte+16]);
          KD_Z = GetFloatFromSerial(&buffer[firstRelevantByte+20]);
          Serial.print("KPX: ");Serial.println(KP_X);
          Serial.print("KIX: ");Serial.println(KI_X);
          Serial.print("KDX: ");Serial.println(KD_X);
          Serial.print("KPZ: ");Serial.println(KP_Z);
          Serial.print("KIZ: ");Serial.println(KI_Z);
          Serial.print("KDZ: ");Serial.println(KD_Z);
          break;
        }*/
    }
  }
}

float RemoteControl::GetFloatFromSerial(byte* buffer)
{
  long tmp =  ((long)buffer[0] << 24);
  tmp |= ((long)buffer[1] << 16);
  tmp |= ((long)buffer[2] << 8);
  tmp |= ((long)buffer[3]);
  tmp &= 0x7fffffff;
  if(buffer[0] & 0x80)
    tmp *= -1;

  return tmp/1000000.0;
}

byte RemoteControl::HasConfigurationData() {
  return false;
}

byte RemoteControl::HasControlData() {
  return false;
}

ConfigData RemoteControl::GetConfigData() {
  return config;
}

ControlData RemoteControl::GetControlData() {
  return control;
}


