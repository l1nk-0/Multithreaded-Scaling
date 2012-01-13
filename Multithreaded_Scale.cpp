#include "Multithreaded_Scale.h"
#include <iostream>

using namespace std;

Multithreaded_Scale::Multithreaded_Scale()
/*Initializes threadcount to 0*/
{
	thread_count=0;
}

void Multithreaded_Scale::setImage(IplImage * image)
/*Takes an image as an input and stores it to the img data structure and 
their information in imgData data structure */
{
	if(img!=NULL) {
		img = image;
		imgData = (uchar*)img->imageData;
	}
}

void Multithreaded_Scale::getPixelAt(double wX,double hY,double &blue,double &green,double &red)
/*Calculate BGR value at the destination image by it's area coverage in the source image*/
{
	bool initial=true;
	int pX,pY,b,g,r;
	double w,h,fracX,fracY,k=0.0,t=0.0,pix=0.0;
	fracX=wX-(int)wX; //To calculate Length coverage in X-direction
	fracY=hY-(int)hY; //To calculate Height coverage in Y-direction	
	w=wRatio+fracX;
	pX=(int)wX;
	red = green = blue = 0;
	while(w > 0) {
		if(w > 1)
			t=1;
		else
			t=w;
		if(initial)
			t=t-fracX;
		h=hRatio+fracY;
		pY=(int)hY;
		while(h > 0) {
			if(h > 1)
				k=1;
			else
				k=h;
			if(initial) {
				h=h-fracY;
				initial=false;
			}
			if(pX < img->height && pY < img->width)	{
				/* Take actual pixel value from the imageData (imgData) */
				b=imgData[pX*img->widthStep+pY*img->nChannels+0];
				g=imgData[pX*img->widthStep+pY*img->nChannels+1];
				r=imgData[pX*img->widthStep+pY*img->nChannels+2];
				blue+=t*k*(double)b;   //t*k gives the area coverage at a perticular pixel level
				green+=t*k*(double)g;  
				red+=t*k*(double)r; 
			} 
			h=h-1;
			pY++; //Increment Y direction Pixel
		}
		w=w-1;
		pX++; //Increment X direction pixel
	}
}

void Multithreaded_Scale::scaleTheRegion(int wL,int wU,int hL,int hU) 
/*Scales a region of the image partiotioned by the PartitionAndScale function*/
{
	double b,g,r;
	
	double wX=wL*wRatio,hY=hL*hRatio;
	for(int i=hL;i<hU;i++) {
		for(int j=wL;j<wU;j++) {
			//Moves at each pixel at dest img and calculate it's value from getPixelAt
			getPixelAt(hY,wX,b,g,r);
			scaled_data[i*scaled_img->widthStep+j*scaled_img->nChannels+0]=(uchar)b;
			scaled_data[i*scaled_img->widthStep+j*scaled_img->nChannels+1]=(uchar)g;
			scaled_data[i*scaled_img->widthStep+j*scaled_img->nChannels+2]=(uchar)r;
			wX=wX+wRatio;//Moves right in the src img
		}
		wX=wL*wRatio;
		hY=hY+hRatio;//Moves down in the src img		
	}
}

void *scaleThread(void *v_Obj)
/*Runs Individual threads from the partioioned image*/
{
	regionCoords *Obj = (regionCoords*)v_Obj;
	Obj->B_Obj->scaleTheRegion(Obj->wL,Obj->wU,Obj->hL,Obj->hU);
}

void Multithreaded_Scale::partitionAndScale(int wL,int wU,int hL,int hU)
/*A recursive function that paritions the image into various inependent parts */
{
	if((wU-wL)/2 > 100 and (hU-hL)/2 > 100) {
		/*Partion in four equal parts*/
		partitionAndScale(wL,wL+(wU-wL)/2,hL,hL+(hU-hL)/2);
		partitionAndScale(wL+(wU-wL)/2,wU,hL,hL+(hU-hL)/2);
		partitionAndScale(wL,wL+(wU-wL)/2,hL+(hU-hL)/2,hU);
		partitionAndScale(wL+(wU-wL)/2,wU,hL+(hU-hL)/2,hU);
	} else {
		struct regionCoords *Obj=new regionCoords;
		/*Stores information of the partition in regionCoords*/
		Obj->wL=wL;
		Obj->wU=wU;
		Obj->hL=hL;
		Obj->hU=hU;
		Obj->B_Obj = this;
		pthread_create(&t[thread_count],NULL,scaleThread,(void*)Obj);  //Run scaling function on the partition
		thread_count++;
	}
}

void Multithreaded_Scale::scaleImage(int width,int height)
{
	wRatio=(double)img->width/width;
	hRatio=(double)img->height/height;
	
	scaled_img = cvCreateImage(cvSize(width,height),img->depth,img->nChannels);
	scaled_data = (uchar*)scaled_img->imageData;
	cout <<"wRatio : hRatio "<<wRatio<<"  "<<hRatio<<endl;	
	partitionAndScale(0,width,0,height);//Run threads on the image to scale it

	for(int i=0;i<thread_count;i++) 
		pthread_join(t[i],NULL);
	
}
