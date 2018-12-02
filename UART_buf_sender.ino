
#include<stdio.h>
#include<string.h>
#include <math.h>

    float distance=23.44,angle=45;
    int i =0;
void ftoa(float,char*);
//char arr[3] = {'1', '2', '3'};

void setup() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  Serial.print("setup done");

}
void loop() {
  // put your setup code here, to run once:

  

    char dist_c[10] = {'0','0','0','0','0',
                        '0','0','0','0','0'};

    char angle_c[10] = {'0','0','0','0','0',
                        '0','0','0','0','0'};

    char buf[20] = {'0','0','0','0','0',
                    '0','0','0','0','0',
                    '0','0','0','0','0',
                    '0','0','0','0','0'};




    ftoa(angle,angle_c);
    ftoa(distance,dist_c);

    //Serial.println("\nanglechars:%s\tsize:%d\n",angle_c,sizeof(angle_c));
    //Serial.println("\ndistancechars:%s\tsize:%d\n",dist_c,sizeof(dist_c));

    for(i=0; i<sizeof(angle_c);i++)
        buf[i]=angle_c[i];

    //Serial.println("\nbuffera:%s\tsize:%d\n",buf,sizeof(buf));

    for(i=0; i<sizeof(dist_c);i++)
        buf[i+10]=dist_c[i];

   
    //Serial.println("\nbufferb:%s\tsize:%d\n",buf,sizeof(buf));
    
    /*
    Serial.println("buf is ");
    for(int j=0; j<sizeof(buf); j++)
    Serial.print(buf[j]);
   
    delay(1000);
    */
    //Serial.println("Test.... ");
    Serial.write(buf, sizeof(buf));
}









/*

int main()
{


    //***send via UART


    //-------------receiving end--------------------
    //*** Receive via UART

    i=0;
    for(; i<(sizeof(buf)/2);i++){
        angle_result[i] = buf[i];
        printf(" char %c\n",angle_result[i]);
    }

    i=0;
    for(; i<10;i++){
       dist_result[i] = buf[i+10];
        printf(" char %c\n",dist_result[i]);
    }


    printf("\nreceived angle:%s\tsize:%d\n",angle_result,sizeof(angle_result));
    printf("\nreceived distance:%s\tsize:%d\n",dist_result,sizeof(dist_result));

    double angle_rec = atof(angle_result);
    double dist_rec = atof(dist_result);

    printf("angle result is %f\n", angle_rec);
    printf("distance result is %f\n", dist_rec);


    return 0;
}

*/




void ftoa(float f,char *buf)
{
    int pos=0,ix,dp,num;
    if (f<0)
    {
        buf[pos++]='-';
        f = -f;
    }
    dp=0;
    while (f>=10.0)
    {
        f=f/10.0;
        dp++;
    }
    for (ix=1;ix<8;ix++)
    {
            num = f;
            f=f-num;
            if (num>9)
                buf[pos++]='#';
            else
                buf[pos++]='0'+num;
            if (dp==0) buf[pos++]='.';
            f=f*10.0;
            dp--;
    }
}
