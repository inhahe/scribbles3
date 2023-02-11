# scribbles3
Makes cool animated designs with command-line parameters. Can save to animated gif. 
For an example of what it does, see https://twitter.com/iyaelsorai/status/1624196182745444352

```
Parameters:
  --seed <integer> - randomization seed. use this to get the same exact pattern you did before
  --file <filename.gif> - if an output file is specified, --loop will be enabled
                          and the animation will only loop once, and it won't
                          let you close it until it's done.
  --spacepoints <integer> - number of curve anchorpoints in space
                            increase this to make more complicated shapes.defaults to 30
  --timepoints <integer> - number of curve anchorpoints in time per space anchorpoint
                           increase this to make the loops longer when specifying --loop or --file
                           defaults to 5
  --w <integer> - screen width. defaults to 1000>
  --h <integer> - screen height. defaults to 1000
  --bgcolor <six-digit hex number> - background color - defaults to #ffffff>
  --fgcolor <six-digit hex number> - foreground color - defaults to #0000ff>
  --loop - loops back on itself in time seamlessly
  --incontiguous - spacepoints don't move contiguously through time; they skip pixels
  --pointsperspacecurve <integer> - number of points calculated on each bezier curve
                                    lines are drawn between each point. defaults to 100
                                    make this 1 for a jagged effect
  --pointspertimecurve <integer> - number of points calculated on each bezier curve of change
                                   if incontiguous is not specified, points are connected by lines across time
                                   try 1 along with --discontiguous to get a rapid-fire succession of unique shapes
```
