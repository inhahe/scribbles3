run:
	g++ Source.cpp -o scribbles -lSDL2 -lboost_program_options -lboost_regex -Igif-h

#todo: detect if Boost or SDL2 is missing and if it is abort and show command to install. Google is being of no help.
#todo: see if I'm missing any important g++ flags to make the program faster. -O3 and -OFast didn't make much difference. 
#I read https://stackoverflow.com/questions/3005564/gcc-recommendations-and-options-for-fastest-code/3005672#3005672 and they seemed mostly irrelevant to my code.