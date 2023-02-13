import random, math
import pygame

pygame.display.init()
pygame.display.set_mode((0,0),pygame.NOFRAME, 8)
s = pygame.display.get_surface()

w, h = s.get_width(), s.get_height()
numcurves = 20

halfcurves = [((random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h))) for _ in range(numcurves)]

def beziercurve_quadratic(p1, p2, p3):
  def getPt(n1, n2, perc):
    diff = n2-n1
    return n1+(int(diff)*perc)
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
    yield (x, y)
     
def beziercurve_cubic(p1, p2, p3, p4):
  def getPt(n1, n2, perc):
    diff = n2-n1
    return n1+(int(diff*perc))
  for i in range(100):
    i2 = i/100
    #The Green Lines
    xa = getPt(p1[0], p2[0], i2)
    ya = getPt(p1[1], p2[1], i2)
    xb = getPt(p2[0], p3[0], i2)
    yb = getPt(p2[1], p3[1], i2)
    xc = getPt(p3[0], p4[0], i2)
    yc = getPt(p3[1], p4[1], i2)
  
    #The Blue Line
    xm = getPt(xa, xb, i2)
    ym = getPt(ya, yb, i2)
    xn = getPt(xb ,xc, i2)
    yn = getPt(yb, yc, i2)
  
    #The Black Dot
    x = getPt(xm, xn, i2)
    y = getPt(ym, yn, i2)
    yield (x, y)
    
def beziercurve_cubic(p1, p2, p3, p4):
  def getPt(n1, n2, perc):
    diff = n2-n1
    #return n1+(int(diff*perc))
    return n1+diff*perc
  for i in range(100):
    i2 = i/100
    #The Green Lines
    xa = getPt(p1[0], p2[0], i2)
    ya = getPt(p1[1], p2[1], i2)
    xb = getPt(p2[0], p3[0], i2)
    yb = getPt(p2[1], p3[1], i2)
    xc = getPt(p3[0], p4[0], i2)
    yc = getPt(p3[1], p4[1], i2)
  
    #The Blue Line
    xm = getPt(xa, xb, i2)
    ym = getPt(ya, yb, i2)
    xn = getPt(xb ,xc, i2)
    yn = getPt(yb, yc, i2)
  
    #The Black Dot
    x = getPt(xm, xn, i2)
    y = getPt(ym, yn, i2)
    yield (int(x), int(y))

# curves = []
# points = [(random.randint(0, w), random.randint(0, h)) for p in range(numcurves*2)]
# for c in range(numcurves):
#   p1 = points[c*2]
#   p2 = points[c*2+1]
#   p3 = points[(c*2+2)%(numcurves*2)]
#   p4 = points[(c*2+3)%(numcurves*2)]
#   p1c = (p1[0]+(p2[0]-p1[0])/2, p1[1]+(p2[1]-p1[1])/2)
#   p2c = p2
#   p3c = p3
#   p4c = (p3[0]+(p4[0]-p3[0])/2, p3[1]+(p4[1]-p3[1])/2)
#   curves.append((p1c, p2c, p3c, p4c))

# def plotline(p1, p2):
#   dx = p2[0]-p1[0]
#   dy = p2[1]-p1[1]
#   D = 2*dy-dx
#   y = p1[1]
#   for x in range(p1[0], p2[0] + (1 if p1[0] < p2[0] else -1), 1 if p1[0] < p2[0] else -1):
#     yield (x, y)
#     if D > 0:
#       y += 1
#       D -= 2*dx
#     D += 2*dy
    
# def plotline(p1, p2):
#   m_new = 2 * (p2[1] - p1[1])
#   slope_error_new = m_new - (p2[0] - p1[0])
#   y = p1[1]
#   for x in range(p1[0], p2[0] + (1 if p1[0] < p2[0] else -1), 1 if p1[0] < p2[0] else -1):
#     yield (x, y)
#     slope_error_new += m_new
#     if slope_error_new >= 0:
#       y += 1
#       slope_error_new -= 2 * (p2[0] - p1[0])
      
# def plotline(p1, p2):
#   if p1[0] > p1[0]:
#     p1, p2 = p2, p1
#   m_new = 2 * (p2[1] - p1[1])
#   slope_error_new = m_new - (p2[0] - p1[0])
#   y = p1[1]
#   for x in range(p1[0], p2[0] + 1):
#     yield (x, y)
#     slope_error_new += m_new
#     if slope_error_new >= 0:
#       y += 1
#       slope_error_new -= 2 * (p2[0] - p1[0])

# def plotline(p1, p2):
#   if p1[0]==p2[0]:
#     if p1[1] > p2[1]:
#       p1, p2 = p2, p1
#     for y in range(p1[1], p2[1]+1):
#       yield (p1[0], y)
#   else:  
#     if p1[0] > p2[0]:
#       p1, p2 = p2, p1
#     slope = (p2[1]-p1[1])/(p2[0]-p1[0])
#     if slope > 1:
#       slope = 1/slope
#       if p1[1] > p2[1]:
#         p1, p2 = p2, p1
#         slope = -slope
#       x = p1[0]
#       for y in range(p1[1], p2[1]+1):
#         yield (int(x), y)
#         x += slope
#     else:
#       y = p1[1]
#       for x in range(p1[0], p2[0]+1):
#         yield (x, int(y))
#         y += slope
#         

# def plotline(p1, p2):
#   x1 = int(p1[0])
#   x2 = int(p2[0])
#   y1 = int(p1[1])
#   y2 = int(p2[1])
#   dx = x2 - x1
#   dy = y2 - y1
#   dx1 = abs(dx)
#   dy1 = abs(dy)
#   px = 2 * dy1 - dx1
#   py = 2 * dx1 - dy1
#   if dy1 <= dx1:
#     if dx >= 0:
#       x = x1
#       y = y1
#       xe = x2
#     else:
#       x = x2
#       y = y2
#       xe = x1
#     #yield (x, y)
#     i = 0
#     while x < xe:
#       x += 1
#       if px < 0:
#         px = px + 2 * dy1
#       else:
#         if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
#           y += 1
#         else:
#           y -= 1
#         px = px + 2 * (dy1 - dx1)
#       yield (x, y)
#   else:
#     if dy >= 0:
#       x = x1
#       y = y1
#       ye = y2
#     else:
#       x = x2
#       y = y2
#       ye = y1
#     #yield (x, y)
#     while y < ye:
#       y += 1;
#       if py <= 0:
#         py = py + 2 * dx1
#       else:
#         if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
#           x += 1
#         else:
#           x -= 1
#         py = py + 2 * (dx1 - dy1)
#       yield (x, y)

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
        px = px + 2 * dy1
      else:
        if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
          y += xd
        else:
          y -= xd
        px = px + 2 * (dy1 - dx1)
      yield (x, y)
  else:
    yd = 1 if y1 < y2 else -1
    y = y1
    x = x1
    yield (x, y)
    while y != y2:
      y += yd;
      if py <= 0:
        py = py + 2 * dx1
      else:
        if (dx < 0 and dy < 0) or (dx > 0 and dy > 0):
          x += yd
        else:
          x -= yd
        py = py + 2 * (dx1 - dy1)
      yield (x, y)      

# for x in range(20):
#   r = (2*math.pi)*x/20
#   #for p in plotline((600, 600), (int(600+500*math.sin(r)), int(600+500*(math.cos(r))))):
#   #  s.set_at(p, (255*x/20, 255*x/20, 255*x/20))
#   for p in plotline((600+300*math.sin(r), 600+300*math.cos(r)), (int(600+500*math.sin(r)), int(600+500*(math.cos(r))))):
#     s.set_at(p, (255, 255, 255))
#   #pygame.draw.circle(s, (255*x/20, 255*x/20, 255*x/20), (int(600+550*math.sin(r)), int(600+550*(math.cos(r)))), 10)
       
# for l in range(10):
#   for p in plotline((random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h))):
#     s.set_at(p, (255, 255, 255))

#for c in range(10):
#  for p in beziercurve_cubic((random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h)), (random.randint(0, w), random.randint(0, h))):
#    s.set_at(p, (255, 255, 255))
  
# for i in range(numcurves):
#   lbezierpoint = None
#   for bezierpoint in beziercurve_cubic(halfcurves[i][0], halfcurves[i][1], halfcurves[(i+1)%numcurves][0], halfcurves[(i+1)%numcurves][1]):
#     if lbezierpoint is not None:
#       for p in plotline(lbezierpoint, bezierpoint):
#         s.set_at(p, (255, 255, 255))
#     lbezierpoint = bezierpoint
    
# screen = []
# for x in range(w):
#   screen2 = []
#   for y in range(h):
#     screen3 = []
#     for p in range(numcurves):
#       screen3.append(False)
#     screen2.append(screen3)
#   screen.append(screen2)
 
# numpoints = 0
# for i in range(numcurves):
#   lbezierpoint = None
#   for bezierpoint in beziercurve_cubic(halfcurves[i][1], halfcurves[i][0], halfcurves[(i+1)%numcurves][0], halfcurves[(i+1)%numcurves][1]):
#     if lbezierpoint is not None:
#       for x, y in plotline(lbezierpoint, bezierpoint):
#         screen[x][y][i] = True
#     lbezierpoint = bezierpoint

curves = []
rs = [random.random() for r in range(numcurves)]
points = [(random.randint(0, w), random.randint(0, h)) for p in range(numcurves*2)]
for c in range(numcurves):
  p1 = points[c*2]
  p2 = points[c*2+1]
  p3 = points[(c*2+2)%(numcurves*2)]
  p4 = points[(c*2+3)%(numcurves*2)]
  r1 = rs[c]
  r2 = rs[(c+1)%numcurves]
  p1c = (p1[0]+(p2[0]-p1[0])*r1, p1[1]+(p2[1]-p1[1])*r1)
  p2c = p2
  p3c = p3
  p4c = (p3[0]+(p4[0]-p3[0])*r2, p3[1]+(p4[1]-p3[1])*r2)
  curves.append((p1c, p2c, p3c, p4c))
  
# for c, curve in enumerate(curves):
#   color = tuple((int(random.random()*255) for _ in range(3)))
#   for x, y in plotline(curve[0], curve[1]):
#     s.set_at((x, y), color)
#   for x, y in plotline(curve[1], curve[2]):
#     s.set_at((x, y), color) 
#   for x, y in plotline(curve[2], curve[3]):
#     s.set_at((x, y), color)   

curves = []
rs = [random.random() for r in range(numcurves)]
#rs = [.5]*numcurves
points = [(random.randint(0, w), random.randint(0, h)) for p in range(numcurves*2)]
for c in range(numcurves):
  p1 = points[c*2]
  p2 = points[c*2+1]
  p3 = points[(c*2+2)%(numcurves*2)]
  p4 = points[(c*2+3)%(numcurves*2)]
  r1 = rs[c]
  r2 = rs[(c+1)%numcurves]
  p1c = (p1[0]+(p2[0]-p1[0])*r1, p1[1]+(p2[1]-p1[1])*r1)
  p2c = p2
  p3c = p3
  p4c = (p3[0]+(p4[0]-p3[0])*r2, p3[1]+(p4[1]-p3[1])*r2)
  curves.append((p1c, p2c, p3c, p4c))
   
# for c in range(numcurves):
#   lbezierpoint = None
#   for bezierpoint in beziercurve_cubic(*curves[c]):
#     if lbezierpoint is not None:
#       for x, y in plotline(lbezierpoint, bezierpoint):
#         screen[x][y][i] = True
#     lbezierpoint = bezierpoint

# screen = [[False]*h for _ in range(w)]
# points = [p for c in curves for p in beziercurve_cubic(*c)]
# lp2 = None
# #llp2 = None
# #historylen = 1
# #historyindex = 0
# #history = [None] * historylen
# for p in range(len(points)):
#   for p2 in plotline(points[p], points[(p+1)%len(points)]):
#     #s.set_at(p2, (255, 0, 0))
#     if p2 != lp2: #and p2 != llp2:
#     #if p2 not in history:
#       #print(f"{p2}, {lp2}, {llp2}")
#       screen[p2[0]][p2[1]] = not screen[p2[0]][p2[1]]
#       if screen[p2[0]][p2[1]]:
#         s.set_at(p2, (255, 255, 255))
#       else:
#         #print(f"{p2}, {history}")
#         s.set_at(p2, (255, 0, 0))
#     else:
#       s.set_at(p2, (0, 0, 255))
#     #history[historyindex] = p2
#   #  historyindex = (historyindex+1) % historylen
#     #llp2 = lp2
#     lp2 = p2

# screen = [[False]*h for _ in range(w)]
# points = [p for c in curves for p in beziercurve_cubic(*c)]
# lp2 = (None, None)
# for p in range(len(points)):
#   for p2 in plotline(points[p], points[(p+1)%len(points)]):
#     if p2[1] != lp2[1]:
#       screen[p2[0]][p2[1]] = not screen[p2[0]][p2[1]]
#       if screen[p2[0]][p2[1]]:
#         s.set_at(p2, (255, 255, 255))
#       else:
#         s.set_at(p2, (255, 0, 0))
#     else:
#       s.set_at(p2, (0, 0, 255))
#     lp2 = p2

# up, down = 1, 2
# ldir = None
# lp2 = (None, None)
# screen = [[False]*h for _ in range(w)]
# points = [p for c in curves for p in beziercurve_cubic(*c)]
# for p in range(len(points)):
#   for p2 in plotline(points[p], points[(p+1)%len(points)]):
#     if lp2[1] is not None and p2[1] != lp2[1]:
#       dir = up if p2[1] < lp2[1] else down
#       if dir == ldir:
#         screen[lp2[0]][lp2[1]] = not screen[lp2[0]][lp2[1]]
#         s.set_at(lp2, (255, 255, 255))
#       ldir = dir
#     lp2 = p2
    
# up, down = 1, 2
# ldir = None
# lp2 = (None, None)
# screen = [[False]*h for _ in range(w)]
# points = [p for c in curves for p in beziercurve_cubic(*c)]
# points2 = []
# for p in range(len(points)):
#   for p2 in plotline(points[p], points[(p+1)%len(points)]):
#     points2.append(p2)
# for p2 in points2 + [points2[0]]:
#   if lp2[1] is not None and p2[1] != lp2[1]:
#     dir = up if p2[1] < lp2[1] else down
#     if dir == ldir:
#       screen[lp2[0]][lp2[1]] = not screen[lp2[0]][lp2[1]]
#       s.set_at(lp2, (255, 255, 255))
#     ldir = dir
#   lp2 = p2

up, down = 1, 2
ldir = None
lp2 = (None, None)
screen = [[False]*h for _ in range(w)]
points = [p for c in curves for p in beziercurve_cubic(*c)]
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

# for y in range(h):
#   dot = False
#   for x in range(1, w):
#     if screen[x][y] and not screen[x-1][y]:
#       dot = not dot
#     if dot:
#       s.set_at((x, y), (255, 0, 0))

for y in range(h):
  dot = False
  for x in range(1, w):
    if screen[x][y]:
      dot = not dot
    if dot:
      s.set_at((x, y), (0, 0, 255))
    else:
      s.set_at((x, y), (255, 255, 255))

# for c in range(numcurves):
#   lbezierpoint = None
#   for bezierpoint in beziercurve_cubic(*curves[c]):
#     if lbezierpoint is not None:
#       for x, y in plotline(lbezierpoint, bezierpoint):
#         s.set_at((x, y), (255, 255, 255))
#     lbezierpoint = bezierpoint
    
# for y in range(h):
#   dot = False
#   for x in range(1, w):
#     for curve in range(numcurves):
#       if (not screen[x-1][y][curve]) and screen[x][y][curve]:
#         dot = not dot
#     if dot:
#       s.set_at((x, y), (0, 0, 255))
#     else:
#       s.set_at((x, y), (255, 255, 255))
#      if screen[x][y][curve]: s.set_at((x, y), (0, 0, 255))
  
up, down = 1, 2
ldir = None
lp2 = (None, None)
screen = [[False]*h for _ in range(w)]
points = [p for c in curves for p in beziercurve_cubic(*c)]
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

  pygame.display.flip()
while not pygame.event.wait().type == pygame.QUIT:
  pass


