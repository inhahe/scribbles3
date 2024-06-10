/*
rgb hsv2rgb(int h, float s, float v)
{
  h = h % 360;
  float c = v * s;
  float x = c * (1 - abs((h / 60) % 2 - 1));
  float m = v - c;
  float r1, g1, b1;
  if (h < 60)
  {
    r1 = c;
    g1 = x;
    b1 = 0;
  }
  else if (h < 120)
  {
    r1 = x;
    g1 = c;
    b1 = 0;
  }
  else if (h < 180)
  {
    r1 = 0;
    g1 = c;
    b1 = x;
  }
  else if (h < 240)
  {
    r1 = 0;
    g1 = x;
    b1 = c;
  }
  else if (h < 300)
  {
    r1 = x;
    g1 = 0;
    b1 = c;
  }
  else if (h < 360)
  {
    r1 = c;
    g1 = 0;
    b1 = x;
  }
  return rgb{ int((r1 + m) * 255), int((g1 + m) * 255), int((b1 + m) * 255) };
}
*/
/*
gb hsv2rgb(int H, float S, float V)
{
  float M = 255 * V;
  float m = M * (1 - S);
  float z = (M - m) * (1 - abs((H / 60) % 2 - 1));
  rgb c;
  if (H < 60)
  {
    c.r = M;
    c.g = z + m;
    c.b = m;
  }
  else if (H < 120)
  {
    c.r = z+m;
    c.g = M;
    c.b = m;
  }
  else if (H < 180)
  {
    c.r = m;
    c.g = M;
    c.b = z+m;
  }
  else if (H < 240)
  {
    c.r = m;
    c.g = z+m;
    c.b = M;
  }
  else if (H < 300)
  {
    c.r = z + m;
    c.g = m;
    c.b = M;
  }
  else if (H < 360)
  {
    c.r = M;
    c.g = m;
    c.b = z+m;
  }
  return c;
}
*/
