class createLine{
  float sensorValue;
  
  createLine(float sensorValue){
    this.sensorValue = sensorValue;
  }

  void updateLine(){
      count = count + 30;
      
      line(pX,pY,count,900-sensorValue*.461538); //multiply sensorValue by .461538
      pX=count;
      pY=900-sensorValue*.461538;
      
      if(count > 890){
          count = 50;
          pX = 50;
        }
        
     }

     
}
