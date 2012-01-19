Requirement :-
	OpenCv2.1 or higher 
	To setup opencv :- http://configureopencv.blogspot.com
	

Successfully checked and run on
Fedora-11 
Package := OpenCv2.1
Compilar := gcc-4.4.1 and gcc-4.6.1

To compile :-
g++  `pkg-config opencv --cflags --libs` -lpthread Multithreaded_Scale.cpp main.cpp -o scale

To run 

./scale sample_img.jpg

Note := Give any image in the place of Sample image

-Anand Kumar
(Efficience)
http://iws45.iiita.ac.in/IIT2008090
http://linkinAnand.blogspot.com
