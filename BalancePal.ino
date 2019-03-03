#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

#define susscess -1
#define WIFI_AP "LP"
#define WIFI_PASSWORD "happyyyy" 
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define SITE_URL "api.mediatek.com"

#define DEVICE_ID "DhEh1oQd"
#define DEVICE_KEY "JjR0mywAO2T6Oa6j"

//量到的data，與平衡狀態的差值允許範圍
//EX: Xmeasured - Xsteady > xerror   ======>判定為非平衡狀態
#define errorx 300
#define errory 50
#define errorz 300


//平衡狀態之x,y,z的平均值，以此值定義平衡狀態
float datax[1000];
float datay[1000];
float dataz[1000];
int state = 0; //state 0 = data measuring; state 1 = data transforming

float meanx = 0;
float meany = 0;
float meanz = 0;

float hx,hy,hz;
LWiFiClient content;
// Accelerometer ADXL345
#include <Wire.h> // I2C library, gyroscope
#define ACC (0xA7>>1)    //ADXL345 ACC address
const int Xcorrection = 0;    //Determine these by zeroing their values when they are not the axis measuring gravity
const int Ycorrection = 271;
const int Zcorrection = -906;
int n =1;
float sx,sy,sz; //量到data與平衡狀態之差值，即 hx-meanx / hy-meany / hz-meanz
boolean disconnectedMsg;

//記錄幾次、前二十筆資料
int num = 1;
int mx[20] = {0};
int my[20] = {0};
int mz[20] = {0};
int i;

void writeTo(int DEVICE, unsigned char address, unsigned char val) {
 Wire.beginTransmission(DEVICE); //start transmission to ACC 
 Wire.write(address);        // send register address
 Wire.write(val);        // send value to write
 Wire.endTransmission(); //end transmission
}

//initialize
void initAcc() {
//Turning on the ADXL345
writeTo(ACC, 0x2D, 0x08);  //Power-saving features control
writeTo(ACC, 0x31, 0x0B);  //Data format control
writeTo(ACC, 0x2C, 0x0A);  //Data rate and power mode control
writeTo(ACC, 0x1E, 0x28);  //X-Axis offset
writeTo(ACC, 0x1F, 0x00);  //Y-Axis offset
writeTo(ACC, 0x20, 0x00);  //Z-Axis offset
//by default the device is in +-2g range reading
}


void setup(){
Serial.begin(115200);
Wire.begin();
initAcc();
 Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    Serial.println("Connecting to AP...");
    ////delay(1000);
  }

  // keep retrying until connected to website
  Serial.println("Connecting to WebSite");
  while (0 == content.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    //////delay(1000);
  }

  // send HTTP request, ends with 2 CR/LF
  Serial.println("send HTTP GET request");


  /*
    // waiting for server response
    Serial.println("waiting HTTP response:");
    while (!content.available())
    {
      //delay(100);
    }
  */

disconnectedMsg = false;
}

void loop(){

if(Serial.available()<=0){
float acc[3];
getAccelerometerData(acc);
hx = acc[0]; //加速度計量到的data
hy = acc[1];
hz = acc[2];

Serial.print(" X="); Serial.print(hx);
Serial.print(" Y="); Serial.print(hy);
Serial.print(" Z="); Serial.print(hz);
Serial.println(" ");
//delay(50); 


if(num < 20){
    mx[num] = hx;
    my[num] = hy;
    mz[num] = hz;
}
else if(num==20){
    mx[num] = hx;
    my[num] = hy;
    mz[num] = hz;
    for(i=1; i<20;i++){
      meanx = meanx + mx[i];
    }
    meanx = meanx/19;
    for(i=1; i<20;i++){
      meany = meany + my[i];
    }
    meany = meany/19;
    for(i=1; i<20;i++){
      meanz = meanz + mz[i];
    }
    meanz = meanz/19;

Serial.print(" meanX="); Serial.print(meanx);
Serial.print(" meanY="); Serial.print(meany);
Serial.print(" meanZ="); Serial.print(meanz);
    
}


if(num>=20 && Serial.available()<=0){
sx = abs(hx - meanx);
sy = abs(hy - meany);
sz = abs(hz - meanz);

if(sx>=errorx || sy>=errory || sz>=errorz){
  Serial.println("Nonsteady State");
  Serial.print(" n= "); Serial.print(n);
  Serial.print(" sX="); Serial.print(sx);
  Serial.print(" sY="); Serial.print(sy);
  Serial.print(" sZ="); Serial.print(sz);

  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data2 = "n,,";
  data2 = data2 + n;
  //data += x;
  int dataLength2 = data2.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength2);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data2);

 content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  data2 = "x,,";
  data2 = data2 + hx;
  //data += x;
  dataLength2 = data2.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength2);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data2);
  // ---- ----
  // ---- copied form MCS Arduino code hint ----
  
  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data = "y,,";
  data = data + hy;
  //data += y;
  int dataLength = data.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data);
  // ---- ----


  ////delay(5000);
  
  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data1 = "z,,";
  data1 = data1 + hz;
  //data1 += z;
  int dataLength1 = data1.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength1);
  content.println("Content-Type: text/csv");
  // content.println("Connection: close");
  content.println();
  content.println(data1);

  // waiting for server response
  Serial.println("waiting HTTP response:");
  while (!content.available())
  {
    //delay(1000);
  }
}

else{
  Serial.println("Steady State ");
  Serial.print(" n="); Serial.print(n);
  Serial.print(" sX="); Serial.print(sx);
  Serial.print(" sY="); Serial.print(sy);
  Serial.print(" sZ="); Serial.print(sz);
 
  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data2 = "n,,";
  data2 = data2 + "-1";
  //data += x;
  int dataLength2 = data2.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength2);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data2);

 content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  data2 = "x,,";
  data2 = data2 + hx;
  //data += x;
  dataLength2 = data2.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength2);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data2);
  // ---- ----
  // ---- copied form MCS Arduino code hint ----
  
  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data = "y,,";
  data = data + hy;
  //data += y;
  int dataLength = data.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength);
  content.println("Content-Type: text/csv");
  //content.println("Connection: close");
  content.println();
  content.println(data);
  // ---- ----


  ////delay(5000);
  
  content.println("POST /mcs/v2/devices/DhEh1oQd/datapoints.csv HTTP/1.1");
  String data1 = "z,,";
  data1 = data1 + hz;
  //data1 += z;
  int dataLength1 = data1.length();
  content.println("Host: api.mediatek.com");
  content.println("deviceKey: JjR0mywAO2T6Oa6j");
  content.print("Content-Length: ");
  content.println(dataLength1);
  content.println("Content-Type: text/csv");
  // content.println("Connection: close");
  content.println();
  content.println(data1);

  // waiting for server response
  Serial.println("waiting HTTP response:");
  while (!content.available())
  {
    delay(500);
  }
}
}


   // Make sure we are connected, and dump the response content to Serial

  while (content)
  {
    int v = content.read();
    if (v != -1)
    {
      Serial.print((char)v);
    }
    else
    {
      //Serial.println("no more content, disconnect");
      // content.stop();
      break;
      /*    while (1)
          {
            //delay(1);
          }*/
    }

  }

  if (!disconnectedMsg)
  {
    Serial.println("disconnected by server");
    disconnectedMsg = true;
  }

  if(num >= 20){
    n++;
  }
  num++;

}
}



void getAccelerometerData(float * result) {
int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
byte buff[6];
readFrom(ACC, regAddress, 6, buff); //read the acceleration data from the ADXL345
//each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
//thus we are converting both bytes in to one int
result[0] = ((((int)buff[1]) << 8) | buff[0])+ Xcorrection;   
result[1] = ((((int)buff[3]) << 8) | buff[2])+ Ycorrection;
result[2] = ((((int)buff[5]) << 8) | buff[4])+ Zcorrection;
}





//reads num bytes starting from address register on ACC in to buff array
void readFrom(int DEVICE, unsigned char address, int num, unsigned char buff[]) {
Wire.beginTransmission(DEVICE); //start transmission to ACC 
Wire.write(address);        //sends address to read from
Wire.endTransmission(); //end transmission
Wire.beginTransmission(DEVICE); //start transmission to ACC
Wire.requestFrom(DEVICE, num);    // request 6 bytes from ACC
int i = 0;
while(Wire.available())    //ACC may send less than requested (abnormal)
  { 
  buff[i] = Wire.read(); // receive a byte
  i++;
  }
Wire.endTransmission(); //end transmission
}
