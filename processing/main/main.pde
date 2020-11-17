import processing.serial.*;

mint_graph sibal = new mint_graph(400,400,300);
Serial mySerial;

int x = 0;
void setup(){
  
  size(800,800);
}

void draw(){

   background(255,255,255);

   sibal.draw_graph();
}
