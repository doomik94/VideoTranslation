#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"

#include <iostream>
#include <conio.h>
#include "TCPServer.h"
#include <math.h>
#include <stdio.h>
#include <iterator>
using namespace std;
using namespace cv;
 
char *addr = "127.0.0.1";
int port = 3167;
std::vector<uchar> outVet;
//unsigned char* OutBytes (Mat& A)
//{
//	
//    imencode(".png", A, outVet);
//    int outLen = outVet.size();
//    unsigned char* outBytes;
//    outBytes = new unsigned char[outVet.size()];
//    std::copy(outVet.begin(), outVet.end(), outBytes);
//    return outBytes;
//}

int main(){
    cout<<"=== VIDEO SENDER ==="<<endl;
 try{
    //setup UDP client:
	
	TCPServer *Serv = new TCPServer((char*)"127.0.0.1", port);
	const string s = "Server";
	char* c = "safaweae";
	//
	//namedWindow(s, 1);
 //   //setup openCV
 //   cvNamedWindow("UDP Video Sender", CV_WINDOW_AUTOSIZE);
 //   //CvCapture* capture = cvCreateCameraCapture(0);
	//VideoCapture capture;
	//capture.open(0);
	//if(!capture.isOpened()){
 //       cout<<"No camera found."<<endl;
 //       system ("pause");
	//	return 0;
 //   }
 //   //IplImage *frame;
	//Mat frame;
 //   //frame = cvQueryFrame(capture);
	//capture>>frame;
 //   /*IplImage *sm = cvCreateImage(cvSize(frame->width / 2, frame->height / 2),
 //       frame->depth, 3);*/
	
	VideoCapture Device(0);
	Mat frame;
	Device>>frame;
	int size = frame.rows * frame.cols;
	cout<<"size = "<<size<<endl;
	char* buff;
	char* flag = new char[10];
	/*for (int i=0; i<10; ++i)
		Device>>frame;*/

	char* Cols = new char[frame.cols];
	int res = Serv->SendTo(Cols, frame.cols);
	delete [] Cols;

	Cols = new char[frame.rows];
	res = Serv->SendTo(Cols, frame.rows);
	delete [] Cols;
	//namedWindow(s, 1);
	Device>>frame;
	int N=40;//frame.rows;
	buff = new char[(3*N*(frame.cols))];//---------------------------
	Vec3b bgr;
	string ext = ".jpg";
	//int R[3]={0, 0, 0};
	//::vector<uchar> buffer;
	//vector<int> params;
	//params.push_back( IMWRITE_JPEG_QUALITY );
	//params.push_back( 95 );
	//cv::imencode(ext, frame, buffer, params);
	//int sz = buffer.size();
	//uchar *B = OutBytes(frame);

	vector<uchar> buffer;
	
    while(1){
        //capture frame	
		Device>>frame;
		imencode (ext, frame, buffer);
		int siz = buffer.size();
		/*for (int i=0; i<=frame.rows-N; i+=N)
		{						
			for (int k=0; k<N; ++k)
			for (int j=k*frame.cols; j<(k + 1)*(frame.cols); ++j)
			{
				bgr = frame.at<Vec3b>(k+i, j%(frame.cols));
				buff[3*j] = bgr.val[0];
				buff[3*j+1] = bgr.val[1];
				buff[3*j+2] = bgr.val[2];
				
			}*/
			while (Serv->Recv(flag, 10) != 5);
			//int r = Serv->SendTo(buff, 3*N*frame.cols);
			int r = Serv->SendTo((char*)(&buffer[0]), buffer.size());
			buffer.clear();
			if(r < 0)
				cout<<"Failed to send."<<endl;			
		//}
    }
	delete [] buff;
	delete [] flag;
	delete Serv;
    system ("pause");
	return 0;
 

    //cout<<"Press any key to continue."<<endl;
    //getch();
 }catch(char* e)
 {
	 printf(e);
 }
	system ("pause");
	return 0;
}