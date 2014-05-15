#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"

#include <process.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include "TCPClient.h"
using namespace std;
using namespace cv;

const string s = "Output";
//bool exit = false;
bool DetFlag = false;
std::vector<Rect> faces;
CascadeClassifier face_cascade;
Mat FaceDet(Mat& F)
{	
	
	Mat grayscale;
	Mat Res = F.clone();
	cvtColor(Res, grayscale, CV_BGR2GRAY);
	equalizeHist(grayscale, grayscale);
	face_cascade.detectMultiScale(grayscale, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, Size(30,30));
	for(int i = 0; i < faces.size(); i++)
	{
		Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		Point pt2(faces[i].x, faces[i].y);
		rectangle(Res, pt1, pt2, cvScalar(0, 255, 0, 0), 1, 8, 0);
	}
	faces.clear();
	return Res;
	/*imshow (s, Res);
	waitKey(1)*/;	
}

int TCPMAX = 921600;//65507; // max buffer size
int port = 3167; // listen port number
int main()
{
	try{
	face_cascade.load("haarcascade_frontalface_alt.xml");
	cout<<"=== VIDEO RECEIVER ==="<<endl;
	char *flag = "ready";
	Mat Result;	
    //setup UDP client
	string a /*= "127.0.0.1"*/;
	cin>>a;
    TCPClient *client = new TCPClient((char*)&a.at(0), port);
    char *buff = new char[TCPMAX];

	namedWindow(s, 1);
    //setup openCV
    //cvNamedWindow("UDP Video Receiver", CV_WINDOW_AUTOSIZE);
    int Cols = client->Recv(buff, TCPMAX); 
	if(Cols < 0)
		cout<<"Failed to receive frame."<<endl;
	int Rows = client->Recv(buff, TCPMAX);
	if(Rows < 0)
		cout<<"Failed to receive frame."<<endl;
	delete [] buff;
	int N = 40;//Rows;
	Vec3b bgr;
	/*int c;
	buff = new char[TCPMAX];
	c = client->Recv(buff, TCPMAX);
	delete buff;*/
	Mat Frame (Rows, Cols, DataType<Vec3b>::type);
	for (int i=0; i<Rows; ++i)
			for (int j=0; j<Cols; ++j)
				Frame.at<Vec3b>(i, j) = Vec3b(0,0,0);
	char** ImgBuff = new char*[Rows/N];
	for (int i=0; i<Rows/N; ++i)
		ImgBuff[i] = new char[3*Cols*N];
	buff = new char[TCPMAX];
	

	/*char**ImgBuff = new char*[Rows/N];
		for (int i=0; i<Rows/N; ++i)
			ImgBuff[i] = new char[3*N*Cols];*///----------------------------------------------
		while(1){
			
		/*if (c != 5)
			while (c != 5)
			{
				buff = new char[TCPMAX];
				c = client->Recv(buff, TCPMAX);
				delete buff;
			}
		else
		{
			imshow(s, Frame);
			waitKey(200);
			for (int i=0; i<Rows; ++i)
				for (int j=0; j<Cols; ++j)
					Frame.at<Vec3b>(i, j) = Vec3b(0,0,0);
		}*/
		
		//imshow(s, Frame);
		//Mat A(30, 40, DataType<float>::type);		
        //cout<<"Got a frame of size "<<result<<endl;
		int v;	
		/*for (int i=0; i <= Rows - N; i+=N)
		{*/
			client->SenTo(flag, 5);
			do{
				v = client->Recv(buff, TCPMAX);
			}while(v<0);
			/*char *tmp = new char[v];
			memcpy(tmp, buff, v);*/
			vector<uchar> buffer(v);
			memcpy ((uchar*)&buffer[0], buff, v);
			Frame = imdecode(buffer, CV_LOAD_IMAGE_COLOR);
		
			//waitKey(33);
		if (waitKey(33) == 48) DetFlag = !DetFlag;
		if (DetFlag)
			Result = FaceDet(Frame).clone();
		else
			Result = Frame;
		imshow(s, Result);
		//FaceDet(Frame.clone());
		/*imshow(s, Res.clone());
		waitKey(1);*/
		
    }
	for (int i=0; i<Rows/N; ++i)
		delete [] ImgBuff[i];
	delete [] ImgBuff;
	}catch(char *e)
	{
		printf(e);
	}
	
	system("pause");
	return 0;
}