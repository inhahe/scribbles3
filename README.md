# scribbles3
Makes cool animated designs with command-line parameters. Can save to animated gif. 
For an example of what it does, see https://twitter.com/iyaelsorai/status/1624196182745444352 

I used to draw designs like what this makes, on paper, a long time ago, and for many years I wanted to create it progammatically, but I just couldn't quite figure out how I could do it. But recently I started thinking about it again and coding, and I had a couple of realizations that made it possible.

```
Options:
  --help                    This help screen
  --seed arg                randomization seed. use this to get the same exact
                            pattern you did before
  --file arg                if an output file is specified, --loop will be
                            enabled and the animation will only loop once, and
                            it won't let you close it until it's done.
  --spacepoints arg         number of curve anchorpoints in space. increase
                            this to make more complicated shapes.defaults to 30
  --timepoints arg          number of curve anchorpoints in time per space
                            anchorpoint. increase this to make the loops longer
                            when specifying --loop or --file. defaults to 5
  --w arg                   window width, defaults to 500
  --h arg                   window height, defaults to 500
  --bgcolor arg             background color, six-digit hex number, defaults to
                            #ffffff, or #000000 if --changehue is enabled
  --fgcolor arg             foreground color, six-digit hex number, defaults to
                            #0000ff
  --changehue               make fgcolor cycle through the hues. overrides
                            --fgcolor
  --huespeed arg            amount to increment hue per frame if --changehue is
                            enabled. ignored if --file or --loop enabled.
                            defaults to 1. hue cycles from 0 to 360
  --huemult arg             if --loop or --file is enabled and chaneghue is
                            enabled, huemult is how many times to cycles
                            through hues per time loop
  --loop                    loops back on itself in time seamlessly
  --incontiguous            make it so that spacepoints don't move contiguously
                            through time, but skip pixels
  --pointsperspacecurve arg number of points calculated on each bezier curve.
                            lines are drawn between each point. defaults to
                            100. make this 1 for a jagged effect
  --pointspertimecurve arg  number of points calculated on each bezier curve of
                            change. if incontiguous is not specified, points
                            are connected linearly across time. try 1 along
                            with --incontiguous to get a rapid-fire succession
                            of unique shapes
```
