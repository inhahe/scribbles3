run:
	g++ Source.cpp -o scribbles -lSDL2 -lboost_program_options -lboost_regex -Igif-h
#todo: detect if Boost or SDL2 is missing and if it is abort and show command to install