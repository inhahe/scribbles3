# scribbles3
Makes cool animated patterns with command-line parameters. Can save to animated gif. 

Here are some examples of what it can do (click on the images to start the animation):

![test](https://github.com/inhahe/scribbles3/assets/61786/134d331d-4d9e-4aca-ac27-4e1632bc1d03)

![1624674315085004800](https://github.com/inhahe/scribbles3/assets/61786/cb0a4fa8-816a-4c80-9ff6-56e0f8384e70)

https://github.com/inhahe/scribbles3/assets/61786/80c9f098-0e14-4bc7-9885-5779426eb0db

![1624686369346912256](https://github.com/inhahe/scribbles3/assets/61786/6d781b73-be81-4845-b4e8-6086bc4d58e0)

![1624687321143513089](https://github.com/inhahe/scribbles3/assets/61786/7fc76eac-9582-4554-ba47-76b23124a2ee)

![1624688246864199680](https://github.com/inhahe/scribbles3/assets/61786/b960cde4-002e-4253-bb45-e34b2da05822)

I used to draw designs like what this makes (the first pic), on paper, a long time ago, and for many years I wanted to create it progammatically, but I just couldn't quite figure out how I could do it. But recently I started thinking about it again and coding, and I had a couple of realizations that made it possible.

This program runs on both Linux and Windows.

Here's a little information on how it works: https://ask.absturztau.be/@Inhahe/a/113346862968751642

Dependencies:
- boost_program_options
- boost_regex
- SDL2

Linux installation:\
`sudo apt update && sudo apt install build-essential` if you don't already have G++\
`sudo apt-get install libsdl2-dev`\
`sudo apt-get install libboost-all-dev`\
`git clone https://github.com/inhahe/scribbles3`\
`cd scribbles3`\
`make`\
To run it:\
`./scribbles3`

Windows installation:\
I don't remember and it's way too complicated, just download scribbles3.exe and SDL2.dll from the latest release. https://github.com/inhahe/scribbles3/releases

I do know that you'd have to to set Additional Include Directories and Additional Library Directories in Visual Studio to point to your Boost and SDL2 directories.\
Right-click on the project name in Solution Explorer\
Go to Configuration Properties->C/C++->General->Additional Include Directories and add your SDL2 directory and your Boost directory\
Go to Configuration Properties->Linker->General->Additionals Library Directories\
Add the Release directory that's under your SDL2 directory, e.g. C:\SDL2\Release\
Add the stage\lib directory that's under your Boost directory, e.g. C:\local\boost_1_81_0\stage\lib
Go to Configuration Properties->Linker->Input->Additional Dependencies and add SDL2.lib and SDL2main.lib

The hard part, though, is building Boost and SDL2.

Command line options:

```
Options:
  --help                 This help screen
  --seed arg             randomization seed. use this to get the same exact
                         pattern you got before (but some of the other options
                         will eliminate all similarity in the pattern if
                         they're any different)
  --file arg             if an output file is specified, --loop will be
                         enabled, and the animation will stop after one loop.
                         filename extension should be "gif"
  --noscreen             doesn't display anything. only for use with --file
  --vsync                don't update the screen faster than the screen refresh
                         rate. this isn't supported on all platforms
  --spacecurves arg      number of curves in space. increase this to make more
                         complicated shapes. defaults to 30
  --timecurves arg       number of curves in time per space curve. only applies
                         when --loop or --file is enabled. increase this to
                         make the loops longer. defaults to 5
  --w arg                window width. defaults to 1000. I recommend a square
                         aspect ratio; otherwise the graphics are kinda skewed
  --h arg                window height. defaults to 1000. smaller width and
                         height make the program run faster
  --bgcolor arg          background color, six-digit hex number. defaults to
                         #ffffff, or #000000 if --rotatehue is enabled
  --fgcolor arg          foreground color, six-digit hex number. defaults to
                         #0000ff
  --rotatehue            make fgcolor cycle through the hues. overrides
                         --fgcolor
  --huespeed arg         amount to increment hue per frame if --rotatehue is
                         enabled. floating point. defaults to 1. hue cycles
                         from 0 to 360. ignored if --file or --loop is enabled.
  --huemult arg          if --loop or --file is enabled and --rotatehue is
                         enabled, --huemult specifies how many times to cycle
                         through hues per time loop. defaults to 1
  --saturation arg       saturation of colors when using --rotatehue. 1 to 100.
                         defaults to 100
  --value arg            brightness of colors when using --rotatehue. 1 to 100.
                         defaults to 100
  --loop                 loops back on itself in time seamlessly. I recommend
                         using --vsync with --loop; otherwise it'll go way too
                         fast
  --incontiguous         make it so that spacecurves don't move contiguously
                         through time, but skip pixels. this will have the
                         effect of making the animation change faster.
                         --incontiguous is automatically enabled when --loop or
                         --file is enabled. I recommend using --vsync with
                         --incontiguous; otherwise it'll go way too fast
  --spacecurvepoints arg number of points calculated on each bezier curve in
                         space. lines are drawn between each point. defaults to
                         100. make this 1 for a jagged effect
  --timecurvepoints arg  number of points calculated on each bezier curve of
                         change. if --incontiguous is not enabled, points are
                         connected linearly across time. try 1 along with
                         --incontiguous to get a rapid-fire succession of
                         completely different shapes
```
