void plotline(SDL_Renderer* renderer, point p1, point p2)
{
  int dx, dy, dx1, dy1, px, py, xd, yd, x, y;
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  dx1 = abs(dx);
  dy1 = abs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1)
  {
    xd = (p1.x < p2.x) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    SDL_RenderDrawPoint(renderer, x, y);
    while (x != p2.x)
    {
      x += xd;
      if (px < 0)
      {
        px += 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          y += xd;
        }
        else
        {
          y -= xd;
        }
        px += 2 * (dy1 - dx1);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  else
  {
    yd = (p1.y < p2.y) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    SDL_RenderDrawPoint(renderer, x, y);
    while (y != p2.y)
    {
      y += yd;
      if (py <= 0)
      {
        py += 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          x += yd;
        }
        else
        {
          x -= yd;
        }
        py += 2 * (dx1 - dy1);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
} 
