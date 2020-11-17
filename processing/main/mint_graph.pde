public class mint_graph{
  
  private float raw, pitch, roll;
  private int pos_x, pos_y, ext;
  
  mint_graph(int pos_x, int pos_y, int ext){
    this.pos_x =pos_x;
    this.pos_y =pos_y;
    this.ext = ext;}
    
  public void bg(){
    fill(255,255,255);
    square(this.pos_x-(this.ext/2),this.pos_y-(this.ext/2),this.ext);
    noFill();
    circle(this.pos_x, this.pos_y, this.ext*0.9);
    line(this.pos_x-(this.ext/2), this.pos_y, this.pos_x+(this.ext/2), this.pos_y);
    line(this.pos_x, this.pos_y-(this.ext/2), this.pos_x, this.pos_y+(this.ext/2));
    
  }
  
  public void get_data(float raw, float pitch, float roll){
    
    this.raw = raw;
    this.pitch = pitch;
    this.roll = roll;
     
  }

  public void draw_graph(){
    this.bg();
    
    //raw
    float radius = this.ext * 0.9 * 0.5;
    float x_raw = radius * cos(HALF_PI - (this.raw * (PI / 180)));
    float y_raw = radius * sin(HALF_PI - (this.raw * (PI / 180)));
    fill(0,255,0);
    circle(this.pos_x + x_raw, this.pos_y  - y_raw, this.ext  * 0.05);
    
    //pitch and roll
    float x = (this.ext * 0.5)*(this.roll / 180);
    float y = (this.ext * 0.5)*(this.pitch / 180);
    fill(255,0,0);
    circle(this.pos_x + x, this.pos_y -y, this.ext  * 0.05);
  }

  
}
