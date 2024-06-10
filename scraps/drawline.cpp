
  
  /*
  for (int y = 0; y < h; y++)
  {
    point l = { 0, y };
    bool dot = false;
    for (int x = 0; x < w; x++)
    {
      if (screen[++sp]) dot = not dot;
      if (dot)
      {
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
        SDL_RenderDrawLine(renderer, l.x, l.y, x, y);
        if (dowrite) SetPixel(x, y, fg.r, fg.g, fg.b);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, 255);
        SDL_RenderDrawLine(renderer, l.x, l.y, x, y);
        if (dowrite) SetPixel(x, y, bg.r, bg.g, bg.b);
      }
      l = { x, y };
    }
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
    SDL_RenderDrawLine(renderer, w-1, y, l.x, l.y);
  }
  */
  