all: main

main: main.cpp vec.h ray.h color.h
	g++ main.cpp -o main
	main.exe > image.ppm
	feh image.ppm

clean:
	rm -f main
	rm -f image.ppm