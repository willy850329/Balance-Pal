# Balance-Pal
An Arduino based system in order to test and enhance people's balance.
This system contains an Arduino board (MTK　linkit ONE), MPU6050, WIFI kit.
In order to test the user's balance, the user will be asked to stand on single foot, with this system being set on his knee.
The system will keep recording the accelerometer's(MPU6050) data and send to the MCS cloud database with WIFI kit.
We also develop an Android app which downloads the sensor data from the MCS and analyzes it with the embedded algorithm.
Eventually, the user will get rated of his balance by the app. 

# Flow Chart
<img width="500" height="500" src="https://github.com/willy850329/Balance-Pal/blob/master/BalancePal/flow.png"/>

# Demo
<img width="500" height="500" src="https://github.com/willy850329/Balance-Pal/blob/master/BalancePal/demo_1.gif"/>
<img width="500" height="500" src="https://github.com/willy850329/Balance-Pal/blob/master/BalancePal/demo_2.gif"/>
<img width="500" height="500" src="https://github.com/willy850329/Balance-Pal/blob/master/BalancePal/app.png"/>
*** Only the Arduino code is provided here but not the Android app's code.

*** For more detail, please refer to the PPT file: https://drive.google.com/file/d/1Iv8G8rbaXyf-Jm7FIQbnkgYAD3jlQKVi/view?usp=sharing
