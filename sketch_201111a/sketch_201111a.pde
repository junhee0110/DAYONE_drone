void setup() {  
    
    size(1000,1000,P3D); 
    noFill();
    strokeWeight(3); 
    frameRate(30);
} 
//pointO:(300,300,0)+(200,200,200)=(500,500,200)

void draw() {  
    background(0); 
    lights();

    pushMatrix(); 
    rotateX(mouseY*0.01);
    rotateY(mouseX*0.01);
    
    translate(300,300,0);
    stroke(100);
    box(200);//box1
    
    translate(100,100,-100);
    sphere(10);//for pointO
    
    //for the vector-arrow
    
    translate(-100,-100,100);  
    translate(200,200,0);
    stroke(100);
    box(200);//box2
    
    translate(0,0,-200);
    stroke(100);
    box(200,200,200);//box3
    
    translate(0,-200,200);
    stroke(100);
    box(200,200,200);//box4
   
    
    translate(-200,0,-0);
    stroke(100);
    box(200,200,200);//box5
    
    translate(0,0,-200);
    stroke(100);
    box(200,200,200);//box6
    
    translate(200,0,0);
    stroke(100);
    box(200,200,200);//box7
    
    translate(-200,200,200);
    stroke(100);
    box(200,200,200);
    
    translate(0,0,-200);
    stroke(100);
    box(200,200,200);
    
    popMatrix();
    
 
   
}                                                                                                                                                                                                  
