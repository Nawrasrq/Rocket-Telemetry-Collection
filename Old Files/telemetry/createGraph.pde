
class createGraph{
  String xaxis,yaxis;
  int lastupdate = 0;
  int x = 50;
  int y = 900;
  int count = 0;
  int ft=0;
  int interval;

  createGraph(String xaxis,String yaxis,int interval){
    this.xaxis = xaxis;
    this.yaxis = yaxis;
    this.interval = interval;
  }
  void display(){ 
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
  void updateGraph(){
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
