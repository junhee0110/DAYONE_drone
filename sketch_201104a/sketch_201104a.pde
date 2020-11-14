
PImage img;
int a=360;
int b=321;
int t1=0; //inclination : "Gi-Ul-Gi" of Motor 1-3
int t2=0;//of Motor 2-4

void setup()  {
  size(720,645);
  smooth();
  img = loadImage("background.PNG");
}

void draw()  {
  //a=t1;
  //b=t2;
 image(img, 0, 0);
 noFill();
 stroke(245,10,10);
 strokeWeight(1);
 ellipse(a,b,200,200);
 stroke(245,10,10);
 strokeWeight(3);
 ellipse(a,b,3,3);
 }
 
