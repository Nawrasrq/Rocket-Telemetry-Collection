import processing.serial.*;
 ArrayList <createLine> lines;

Serial myPort;
float sensorData;
int constant = 0;

int lastupdate = 0;
int i = 0;

int count = 50;
int pX = 50;
float pY = 900;
  
createGraph altitude;
PWindow win;

public void settings() {
  size(900,1000);
}
void setup(){
  //myPort = new Serial(this,"COM5", 9600);
  win = new PWindow();

  lines = new ArrayList();

  altitude = new createGraph("Time(Seconds)","Altitude(Ft)",65);
  altitude.display();
}

void draw(){
  
  
  if(myPort.available()>0 && constant == 1){
  sensorData = myPort.read();  
  constant = 2;
  }
  else
  sensorData = 0;
  
//  println(sensorData);
  
  altitude.updateGraph();
  handleLines();
}

//create a new line for each new measurement
void handleLines(){
  lines.add(new createLine(sensorData));
  if((millis() - lastupdate) > 1000){
      lastupdate = millis();
      i = i +1;
      createLine line = (createLine) lines.get(i);
      line.updateLine();
  }
}
