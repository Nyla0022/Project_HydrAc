
#include<stdio.h>
#include<string.h>
#include <math.h>

char bytes_received[20];
int i=0;

float distance, angle;

char angle_result[10] = {'0','0','0','0','0',
                        '0','0','0','0','0'};

char dist_result[10] = {'0','0','0','0','0',
                        '0','0','0','0','0'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Let's start receiving...");

}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available()){
    //while(Serial.available()>0){
   // if(i<10){
    Serial.println("I received something");
    for(int j=0; j<20; j++){
       bytes_received[j] = Serial.read();
       delay(0.8);
    }
i++;
    //Serial.print(num_of_bytes_received);
    //}
int m=0;
    for(;m<10; m++)
    angle_result[m]=bytes_received[m];

    m=0;
    for(;m<10; m++)
    dist_result[m] = bytes_received[m+10];

    angle = atof(angle_result);
    distance = atof(dist_result);

    Serial.println(angle);
    Serial.println(distance);

    
  }
}// end of loop
