PImage img;
int a=360;
int b=321;
int t1=0; //inclination : "Gi-Ul-Gi" of Motor 1-3
int t2=0;//of Motor 2-4

int xp;
int xn;
int yp;
int yn;//the four values of inclination. wanna go home...let me go home...


import processing.serial. *;
Serial myPort;

/*----------------------------------------void setup-------------------*/
void setup(){
   size(720,645);
  smooth();
  img = loadImage("background.PNG");
  
  println("Available serial ports:");
  println(Serial.list());

 //port = new Serial(this, "COM17", 115200);
  myPort = new Serial(this, Serial.list()[2], 115200);
}
/*---------------------void serialEvent-------------------------------------*/
void serialEvent (Serial myPort) {
// get the byte:
  int inByte = myPort.read();
  println(inByte);//I do have "4" values. I must split the "inByte" up into four values of xp~yn
  
}
/*---------------------------------------------void draw------------------*/
void draw() {
 //a=t1;
  //b=t2;s
 image(img, 0, 0);
 noFill();
 stroke(245,10,10);
 strokeWeight(1);
 ellipse(a,b,200,200);
 stroke(245,10,10);
 strokeWeight(3);
 ellipse(a,b,3,3);
 
}
