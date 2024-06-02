import random, math
import pygame

pygame.display.init()
#pygame.display.set_mode((0,0),pygame.NOFRAME, 8)
pygame.display.set_mode((1000,1000))
s = pygame.display.get_surface()

w, h = s.get_width(), s.get_height()
numcurves = 30

halfcurves = [((random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h))) for _ in range(numcurves)]

def beziercurve_quadratic(p1, p2, p3):
  def getPt(n1, n2, perc):
    diff = n2-n1
    #return n1+(int(diff)*perc)
    return n1+diff*perc
  for i in range(100):
    #The Green Line
    i2 = i/100
    xa = getPt(p1[0], p2[0], i2)
    ya = getPt(p1[1], p2[1], i2)
    xb = getPt(p2[0], p3[0], i2)
    yb = getPt(p2[1], p3[1], i2)

    # The Black Dot
    x = getPt(xa, xb, i2)
    y = getPt(ya, yb, i2)
    yield (int(x), int(y))
     
def plotline(p1, p2):
  #might have an issue when (y2-y1)/(x2-x1)==-1 and x2 > x1
  x1 = int(p1[0])
  x2 = int(p2[0])
  y1 = int(p1[1])
  y2 = int(p2[1])
  dx = x2 - x1
  dy = y2 - y1
  dx1 = abs(dx)
  dy1 = abs(dy)
  px = 2 * dy1 - dx1
  py = 2 * dx1 - dy1
  if dy1 <= dx1:
    xd = 1 if x1 < x2 else -1
    y = y1
    x = x1
    yield (x, y)
    while x != x2:
      x += xd
      if px < 0:
        px += 2 * dy1
      else:
        if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
          y += xd
        else:
          y -= xd
        px += 2 * (dy1 - dx1)
      yield (x, y)
  else:
    yd = 1 if y1 < y2 else -1
    y = y1
    x = x1
    yield (x, y)
    while y != y2:
      y += yd;
      if py <= 0:
        py += 2 * dx1
      else:
        if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
          x += yd
        else:
          x -= yd
        py += 2 * (dx1 - dy1)
      yield (x, y)      

curves = []
points = [(random.randint(0, w), random.randint(0, h)) for p in range(numcurves)]
for c in range(numcurves):
  p1 = points[c]
  p2 = points[(c+1)%numcurves]
  p3 = points[(c+2)%numcurves]
  p1c = (p1[0]+(p2[0]-p1[0])/2, p1[1]+(p2[1]-p1[1])/2)
  p2c = p2
  p3c = (p2[0]+(p3[0]-p2[0])/2, p2[1]+(p3[1]-p2[1])/2)
  curves.append((p1c, p2c, p3c))
  
up, down = 1, 2
ldir = None
lp2 = (None, None)
screen = [[False]*h for _ in range(w)]
points = [p for c in curves for p in beziercurve_quadratic(*c)]
points2 = []
for p in range(len(points)):
  for p2 in plotline(points[p], points[(p+1)%len(points)]):
    points2.append(p2)
for i, p2 in enumerate(points2 * 2):
  if lp2[1] is not None and p2[1] != lp2[1]:
    dir = up if p2[1] < lp2[1] else down
    if dir == ldir:
      screen[lp2[0]][lp2[1]] = not screen[lp2[0]][lp2[1]]
      s.set_at(lp2, (255, 255, 255))
    if i >= len(points2):
      break
    ldir = dir
  lp2 = p2

for y in range(h):
  dot = False
  for x in range(1, w):
    if screen[x][y]:
      dot = not dot
    if dot:
      s.set_at((x, y), (0, 0, 255))
    else:
      s.set_at((x, y), (255, 255, 255))

  pygame.display.flip()
while not pygame.event.wait().type == pygame.QUIT:
  pass


