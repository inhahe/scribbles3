# scribbles3
Makes cool animated designs with command-line parameters. Can save to animated gif. 

Here are some examples of what it can do:

![test](https://github.com/inhahe/scribbles3/assets/61786/134d331d-4d9e-4aca-ac27-4e1632bc1d03)

![1624674315085004800](https://github.com/inhahe/scribbles3/assets/61786/cb0a4fa8-816a-4c80-9ff6-56e0f8384e70)

https://github.com/inhahe/scribbles3/assets/61786/80c9f098-0e14-4bc7-9885-5779426eb0db

![1624686369346912256](https://github.com/inhahe/scribbles3/assets/61786/6d781b73-be81-4845-b4e8-6086bc4d58e0)

![1624687321143513089](https://github.com/inhahe/scribbles3/assets/61786/7fc76eac-9582-4554-ba47-76b23124a2ee)

![1624688246864199680](https://github.com/inhahe/scribbles3/assets/61786/b960cde4-002e-4253-bb45-e34b2da05822)

I used to draw designs like what this makes, on paper, a long time ago, and for many years I wanted to create it progammatically, but I just couldn't quite figure out how I could do it. But recently I started thinking about it again and coding, and I had a couple of realizations that made it possible.

```
Options:
  --help                 This help screen
  --seed arg             randomization seed. use this to get the same exact
                         pattern you got before (but some of the other options
                         will eliminate all similarity in the pattern if
                         they're any different)
  --file arg             if an output file is specified, --loop will be
                         enabled, and the animation will stop after one loop
  --noscreen             doesn't display anything. only for use with --file
  --spacecurves arg      number of curves in space. increase this to make more
                         complicated shapes. defaults to 30
  --timecurves arg       number of curves in time per space curve. only applies
                         when --loop or --file is enabled. increase this to
                         make the loops longer. defaults to 5
  --w arg                window width. defaults to 1000
  --h arg                window height. defaults to 1000. smaller width and
                         height make the program run faster
  --bgcolor arg          background color, six-digit hex number. defaults to
                         #ffffff, or #000000 if --rotatehue is enabled
  --fgcolor arg          foreground color, six-digit hex number. defaults to
                         #0000ff
  --rotatehue            make fgcolor cycle through the hues. overrides
                         --fgcolor
  --huespeed arg         amount to increment hue per frame if --rotatehue is
                         enabled. ignored if --file or --loop is enabled.
                         defaults to 1. hue cycles from 0 to 360
  --huemult arg          if --loop or --file is enabled and --rotatehue is
                         enabled, --huemult specifies how many times to cycle
                         through hues per time loop. defaults to 1
  --saturation arg       saturation of colors when using --rotatehue. 1 to 100.
                         defaults to 100
  --value arg            brightness of colors when using --rotatehue. 1 to 100.
                         defaults to 100
  --loop                 loops back on itself in time seamlessly
  --incontiguous         make it so that spacecurves don't move contiguously
                         through time, but skip pixels. this will have the
                         effect of making the animation change faster.
                         --incontiguous is automatically enabled when --loop or
                         --file is enabled
  --spacecurvepoints arg number of points calculated on each bezier curve in
                         space. lines are drawn between each point. defaults to
                         100. make this 1 for a jagged effect
  --timecurvepoints arg  number of points calculated on each bezier curve of
                         change. if --incontiguous is not enabled, points are
                         connected linearly across time. try 1 along with
                         --incontiguous to get a rapid-fire succession of
                         completely different shapes
```
