float voltage_chan1 [9] = {2.43,2.31,2.20,2.14,2.09,2.06,2.04,2.046,2.054};
float voltage_chan2 [9] = {2.57,2.53,2.49,2.41,2.34,2.30,2.27,2.23,2.29};
int dir=1;

if(dir == 1)
  {
      int largest = channel_1[0];
      for (int i = 1; i < TOTAL_SAMPLES; i++)
      {
      if (largest < channel_1[i])
        largest = channel_1[i];
      } 
      distance(largest,1);

  }
else if(dir == 2)
  {
      int largest = channel_2[0];
      for (int i = 1; i < TOTAL_SAMPLES; i++)
      {
      if (largest < channel_2[i])
        largest = channel_2[i];
      } 
      distance(largest,2);
  }
  else{
    //int largest = -1; 
    distance(0,0);
  }






float distance (float volt, int dir){
  distance = 0; 
  if(dir==1){
    
    if(volt>=voltage_chan1[8]){
      
 for(int j=0;j<8;j++){
  if(volt<voltage_chan1[j] && volt>=voltage_chan1[j+1])
    distance=(j+1)*1.5;
 }
    }else{
      distance = 12;
    }
}else if(dir==2){

  if(volt>=voltage_chan2[8]){
  for(int j=0;j<8;j++){
  if(volt<voltage_chan2[j] && volt>=voltage_chan2[j+1])
    distance=(j+1)*1.5;
 }
  }else{
    distance = 12;
  }
}else{
  printfln("dir is incorrect");
  break;
 }
return distance;
}
