import processing.serial.*;

Serial myPort;
String incommingString = "> WAITING FOR DEVICE TO BE READY. \n";
Button ledOn, ledOff, ledInfo, help;

void setup()
{
   size(600, 600);
  
  println(Serial.list());
  
  // You may need to change the number in [ ] to match 
  // the correct port for your system
  myPort = new Serial(this, Serial.list()[6], 115200);
  myPort.bufferUntil(10);
  
  ledOn = new Button(320,10, 100,100, "LED ON");
  ledOff = new Button(440,10, 100,100, "LED OFF");
  ledInfo = new Button(320,130, 100,100, "LED INFO");
  help = new Button(440,130, 100,100, "HELP");
  
  textSize(14);
  textAlign(LEFT, BOTTOM);
  fill(165, 27, 27);
}

void mousePressed()
{
   if(ledOn.pressed(mouseX, mouseY))
   {
      myPort.write("LED, 1;");
      incommingString = incommingString + "Processing > " + "LED, 1; \n";
   }
   
   if(ledOff.pressed(mouseX, mouseY))
   {
      myPort.write("LED, 0;");
      incommingString = incommingString + "Processing > " + "LED, 0; \n";
   }
   
   if(ledInfo.pressed(mouseX, mouseY))
   {
      myPort.write("LED_STATUS;");
      incommingString = incommingString + "Processing > " + "STATUS; \n";
   }
   
   if(help.pressed(mouseX, mouseY))
   {
      myPort.write("HELP;");
      incommingString = incommingString + "Processing > " + "HELP; \n";
   }
    
}

void draw()
{
  background(255);
  
  fill(230,230,230);
  rect(-1,-1,300,602);
  fill(165, 27, 27);
  textAlign(TOP, LEFT);
  text("Communication log:", 10,585);
  textAlign(LEFT, BOTTOM);
  text(incommingString, 10,580);
  
  ledOn.draw();
  ledOff.draw(); 
  ledInfo.draw(); 
  help.draw();
}

void serialEvent(Serial p) 
{
  incommingString = incommingString + "Device > " + (myPort.readString());
}
