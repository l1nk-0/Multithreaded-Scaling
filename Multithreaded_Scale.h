/********************************************************************************************************************************** 
 *-Anand Kumar (Efficience)
 
 Header File := Scale.h
 
 *Multithreaded_Scale (setImage ,getPixel , scaleImage)
 *Variables :- 
 *      t[]  := Threads
 *      thread_count := Counts the numbers of thread completed their execution
 * 	img  :=  Image data structure to store image
 *      imgData := Data structure to store information about the image
 *      scaled_img := Data structure to store scaled image
 *      scaled_data := Data structure to store information of the scaled image 
 *      wRatio      := Width ratio (src/dest)
 *      hRatio      := Height ratio (src/dest) 
 *Member functions :-
 *      setImage(IplImage*) :=  Takes an image and stores information of the image in their respective data structure
 *      getPixelAt(dbl,dbl.dbl&,dbl&,dbl&) := Calculate the area coverage of a scaled image pixel in the actual image
 * 					      to be scaled and calculate pixel value at that pixel
 * 	scaleImage(int,int)  := Takes width and height of the destination image (After scaling) and scales the given image
 *      partitionAndScale(int,int,int,int)  := Partition the image in independent parts so as to work by the multiple threads
 *      scaleTheRefion(int,int,int,int)     := Individual thread running on part of the image in this function
 *
 *regionCoords :-
 *Stores x-range and y-range of a part of the image and an object of Bicubic_Interpolation to work by the individual thread
***********************************************************************************************************************************/

#ifndef MULTITHREADED_SCALE_H
#define MULTITHREADED_SCALE_H

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <pthread.h>


class Multithreaded_Scale {
	private :
		pthread_t t[100];
		uchar *imgData;
		uchar *scaled_data;
		int thread_count;
		double wRatio,hRatio;    // (Image Size)/(Scaled Image Size)
	public :
		Multithreaded_Scale();
		Multithreaded_Scale getSelf();
		IplImage *img;
		IplImage *scaled_img;	
		void setImage(IplImage *img);
		void getPixelAt(double,double,double&,double&,double&);		
		void scaleImage(int,int);		
		void partitionAndScale(int,int,int,int);
		void scaleTheRegion(int,int,int,int);
};

struct regionCoords {
	int wL;
	int wU;
	int hL;
	int hU;
	Multithreaded_Scale *B_Obj;
};
#endif
