import processing.serial.*;

class PWindow extends PApplet {
  PWindow() {
    super();
    PApplet.runSketch(new String[] {this.getClass().getSimpleName()}, this);
  }
  float sensorValue;
  float sensorValue2;
  createGraph acc;
  String xaxis = "Time(Seconds)";
  String yaxis = "Acceleration(Ft)";
  int lastupdate = 0;
  int x = 50;
  int y = 900;
  int count = 0;
  int ft=0;
  int interval = 65;
 

int countLine = 50;
int pX = 50;
float pY = 900;

int countLine2 = 50;
int pX2 = 50;
float pY2 = 900;
  
  void settings() {
    size(900,1000);
  }
  
  void setup() {
    //graph////////////////////////////////////////////////////////
   background(255);    
  //x,y axis
  line(50,900,50,50);
  line(50,900,900,900);
  
  //x,y axis names
  fill(0);
  textSize(20);
  text(xaxis,width/2-30,950);
  text(yaxis,10,30);
  
  //create y axis notches
    for(int i=0;i<28;i++){
      y=y-30;  
      line(45,y,50,y);
    
      ft = ft+interval; //65 for altitude
    
      fill(0, 105, 153);
      textSize(11);
      text(ft,10,y+5);
    } 
 }

  void draw() {

  if(myPort.available()>0 && constant == 2){
  sensorValue = myPort.read();  //random(1820) + 65.0;
  sensorValue2 = myPort.read();
  constant = 1;
  }
  else
  sensorValue = 0;

    
    //graph stuff///////////////////////////////////////////////////
   if((millis() - lastupdate) > 1000){
      lastupdate = millis();

      count = count + 1;
      x = x+30;
    
      line(x,900,x,905);
      
      fill(255);
      noStroke();
      rect(x-5,910,15,10);
      stroke(0);
      
      fill(0, 105, 153);
      textSize(11);
      text(count,x-5,920);

        countLine = countLine + 30;
        countLine2 = countLine2 + 30;
      stroke(#011FA8);
      line(pX,pY,countLine,900-sensorValue*.461538); //multiply sensorValue by .461538
      
      stroke(#A10000);
      line(pX2,pY2,countLine2,900-sensorValue2*.461538);
      
      pX=countLine;
      pY=900-sensorValue*.461538;
 
      pX2=countLine2;
      pY2=900-sensorValue2*.461538;
      
      stroke(0);
        if(countLine > 890){
          countLine = 50;
          pX = 50;
          
          countLine2 = 50;
          pX2 = 50;
          
        }
                if(x > 890){
          x = 50;
          line(x+30,900,x+30,905);
          fill(255);
          noStroke();
          rectMode(CORNER);
          rect(0,910,1000,15);
          rect(51,40,900,860);
        }
        
      }
  }
} 
