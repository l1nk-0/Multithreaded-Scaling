/**********************************************************************************************************************************
 *-Anand Kumar (Efficience)
 
 File Name:= main.cpp
 Main programme to check the programme
***********************************************************************************************************************************/

#include "Multithreaded_Scale.h"
#include <iostream>

using namespace std;

int main(int argc,char *argv[])
{
	int height,width;
	IplImage *img = cvLoadImage(argv[1]); //Takes input arugument image and works on it
	Multithreaded_Scale Object;
	if(img==NULL) {
		cout << "Image not loaded "<<endl;
		return 0;
	}
	
	cvNamedWindow("Normal",CV_WINDOW_AUTOSIZE);//Window for normal image	
	cvNamedWindow("Scaled",CV_WINDOW_AUTOSIZE);//Window for scaled image
	
	Object.setImage(img); //Set the image to be scaled
	
	cout<<"width  "<<img->width<<"Image height "<<img->height<<endl;	
	cout<<"Give width and height of the scaled image"<<endl;
	cin>>width>>height;
	
	Object.scaleImage(width,height); //Scales the image
	
	cvShowImage("Normal",Object.img);
	cvShowImage("Scaled",Object.scaled_img);	
	
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&Object.img);
	
	return 0;
}
