//! Including the wx library header
#include <wx/wx.h> 
#include <wx/string.h>
#include <wx/ffile.h>

//!Including the Opencv library header
#include <cvaux.h>  
#include <highgui.h>
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <cxcore.h>

//! Including the standard C++ library header
#include <vector>
#include <math.h>
#include <iostream>

//! Including Blob header
#include "Blob.h"
#include "BlobResult.h"
using namespace std;

//! the numeber of matrix in the data.txt file to parse
#define NUMBER_OF_MATRIX 6

//! \struct Coordinate of the tracked object in this case a ball.
struct coordinate { 
	//!Coordinates of the rectangle around the blob
	int Minx,Maxx,Maxy,Miny;
	//!float radius;
	bool flag;
};

struct matrixDesc{
		int nCols;
		int nRows;
};


//! All the prototype functions of the video-tracker
CvKalman* initKalman(CvMat** indexMat, struct coordinate selectedCoord);

void execute(char * aviName,int id );

std::vector<float> getValue(wxString filename);

IplImage* getBackground(char* aviName);

struct coordinate extractBlob(IplImage* tmp_frame, IplImage* background,struct coordinate selectedCoord);

void drawInitialBlobs(IplImage* tmp_frame, IplImage* binBack);

int getNumBlob(IplImage* tmp_frame, IplImage* binBack);

void parse(wxString fileName,float* ValuesVect,struct matrixDesc* MDSC);

IplImage* getFiltredBinaryImage(IplImage* currentImage, IplImage* backgroundImage, int value );

void drawBlob(IplImage * image, struct coordinate coord,int R, int G, int B );
	
void copyMat (CvMat* source, CvMat* dest);

float* updateKalman(CvKalman * kalman,CvMat *state, CvMat* measurement, CvMat * process_noise, struct coordinate coord);

CBlob getNearestBlob(CBlobResult blobs, struct coordinate coord);

void initBackgroundModel(CvBGStatModel ** bgmodel, IplImage* tmp_frame, CvGaussBGStatModelParams* paramMoG);

IplImage* updateBackground(CvBGStatModel *bg_model, IplImage * tmp_frame);
