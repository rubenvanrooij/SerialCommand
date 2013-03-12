class Button
{
   int m_x, m_y, m_width,m_height;
   String m_txt;
  
  Button(int x, int y, int w, int h, String _txt)
  {
    m_x      = x;
    m_y      = y;
    m_width  = w;
    m_height = h;
    m_txt  = _txt; 
  } 
  
  boolean pressed(int x, int y)
  {
      if(x>m_x && x<m_x+m_width && y>m_y && y<m_y+m_height)
        return true;
        
      return false;
  }
  
  void draw()
  {
    pushMatrix();
       fill(200,200,200);
       translate(m_x, m_y);
       rect(0,0, m_width, m_height);
       fill(0,0,0);
       textAlign(CENTER, CENTER);
       textSize(14);
       text(m_txt, 0,0, m_width, m_height);
     popMatrix();
  }
}
