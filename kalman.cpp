/*! \file kalman.cpp
 *
 *
 *  \brief <b>This file contains the two function that implement kalamn: the init where the kalman structure are fill up with the data taken by the parser </b>
 *
 *
 *  \author Copyright (C) 2005-2006 by Iacopo Masi <iacopo.masi@gmail.com>
 *   		 	and Nicola Martorana <martorana.nicola@gmail.com>
 *			and Marco Meoni <meonimarco@gmail.com>
 * 			This  code is distributed under the terms of <b>GNU GPL v2</b>
 *
 *  \version $Revision: 0.1 $
 *  \date 2006/10/27 
 * 
 *
 *
 */
 
#include "kalman.h" 

///The function that fill up the kalman structure parsing a data file, where there is the matrices
/**
 * \brief  the map of the kalman matrix in the indexMat vector
 *			0 -> A
 *			1 -> Bu
 *			2 -> H
 *			3 -> Q
 *			4 -> R
 *			5 -> P
 *			6 -> Initial state			
 * \param kalman the pointer to the kalman structure
 * \param indexMat the pointer to the vector of matrix where will be the parsed value
 */

CvKalman* initKalman(CvMat** indexMat, coord initCoord){
	
	struct matrixDesc MDSC[NUMBER_OF_MATRIX];
	
	float ValuesVect[100];

	//!parsing the file
	parse(_("./data.txt"),ValuesVect,MDSC);

	
	for (int i=0;i< NUMBER_OF_MATRIX ;i++){
		indexMat[i] = cvCreateMat( MDSC[i].nRows, MDSC[i].nCols, CV_32FC1 );
		}

	int DP = indexMat[0]->cols; //! number of state vector dimensions */
	int MP = indexMat[2]->rows; //! number of measurement vector dimensions */
	int CP = indexMat[1]->cols; //! number of control vector dimensions */
	
	CvKalman* kalman = cvCreateKalman(DP,MP,CP);
	
	int h = 0;
	
	//! gettin the value from the ValuesVect and store in indexMat
	for (int i=0;i< NUMBER_OF_MATRIX;i++){
		for (int j=0;j<indexMat[i]->rows;j++){
			for (int l=0;l<indexMat[i]->cols;l++){
				indexMat[i]->data.fl[j*indexMat[i]->cols+l] = ValuesVect[h];
				h++;
			}
		}	

	}
	
	//!filling up the kalamn structure. For now I have left the copyMat method. The other method is more efficent however.
	//Per riempire la struttura di kalman ci sono tre alternative:
	//-------Alternativa 1-------//
	
	/*
	kalman->transition_matrix = indexMat[0];//A
	kalman->control_matrix = indexMat[1];//Bu
	kalman->measurement_matrix = indexMat[2];//H
 	kalman->process_noise_cov = indexMat[3];//Q
	kalman->measurement_noise_cov =  indexMat[4];//R
	kalman->error_cov_pre = indexMat[5];//P
	*/
	
	//-------Alternativa 2-------//
#ifndef GNOME	
	for( int i=0; i < DP*DP; i++ )
		kalman->transition_matrix->data.fl[i] = indexMat[0]->data.fl[i];
 
	for( int i=0; i < DP*1; i++ )
		kalman->control_matrix->data.fl[i] = indexMat[1]->data.fl[i] ;

	for( int i=0; i < DP*MP; i++)
		kalman->measurement_matrix->data.fl[i] = indexMat[2]->data.fl[i];
 
	for( int i=0; i < DP*DP; i++)
		kalman->process_noise_cov->data.fl[i] = indexMat[3]->data.fl[i];
 
	for( int i=0; i < MP*MP; i++)
		kalman->measurement_noise_cov->data.fl[i] = indexMat[4]->data.fl[i];
 
	for( int i=0; i < DP*DP; i++)
		kalman->error_cov_pre->data.fl[i] = indexMat[5]->data.fl[i];	
#endif	
	
	//-------Alternativa 3-------//
#ifdef GNOME
	float A[] = {1,0,1,0, 0,1,0,1, 0,0,1,0, 0,0,0,1};
	float Bu[] = { 0, 0, 0, 0};
	float H[] = {1,0,0,0, 0,1,0,0 };
	float Q[] = {0.0001,0,0,0, 0,0.0001,0,0, 0,0,0.0001,0, 0,0,0,0.0001}; 
	float R[] = {0.2845,0.0045, 0.0045,0.0455 };
	float P[] = { 100,0,0,0, 0,100,0,0, 0,0,100,0, 0,0,0,100};
	
	
	for( int i=0; i < DP*DP; i++ )
		kalman->transition_matrix->data.fl[i]= A[i];
	
	for( int i=0; i < DP*1; i++ )
		kalman->control_matrix->data.fl[i] = Bu[i];
	
	for( int i=0; i < DP*MP; i++)
		kalman->measurement_matrix->data.fl[i] = H[i];
	
	for( int i=0; i < DP*DP; i++)
		kalman->process_noise_cov->data.fl[i] = Q[i];
	
	for( int i=0; i < MP*MP; i++)
		kalman->measurement_noise_cov->data.fl[i] = R[i];
	
	for( int i=0; i < DP*DP; i++)
		kalman->error_cov_pre->data.fl[i] = P[i];	
 #endif
	
	
	
	//!Here we must set the initial state
	//int initMeanx, initMeany;
	//initMeanx=(initCoord.Minx+initCoord.Maxx)/2;
	//initMeany=(initCoord.Miny+initCoord.Maxy)/2;
	float a[] = { initCoord.cX, initCoord.cY, 0, 0};

	CvMat Ma=cvMat(1, DP, CV_32FC1, a);
	//copyMat(&Ma, kalman->state_pre);
	copyMat(&Ma, kalman->state_post);
	return kalman;
}

///The function that make a copy from a matrix to another one
/**
 * \param source The source Matrix
 * \param dest The Destination  Matrix, where the values are stored.
 */

void copyMat (CvMat* source, CvMat* dest){
	int i,j;
	for (i=0; i<source->rows; i++)
		for (j=0; j<source->cols;j++)
			dest->data.fl[i*source->cols+j]=source->data.fl[i*source->cols+j];

}

///The function that will update the kalman structure with the data collected in extractBlob. it will provide to do the predict and the correct kalamn's step.
/**
 * \param kalman the pointer to the kalman structure
 * \param state the temp matrix of the state_post
 * \param measurement the temp matrix of the measurement collected in extractBlob
 * \param process_noise the temp matrix of the process_noise
 * \param struct coordinate the struct in which are the measurement coordinate. (z_k)
 */

float* updateKalman(CvKalman * kalman, coord coord){
	
	int Meanx, Meany;
	CvMat* measurement = cvCreateMat(2,1, CV_32FC1 );
	Meanx = (int) coord.cX;
	Meany =  (int) coord.cY;
	cvmSet(measurement,0,0,Meanx);
	cvmSet(measurement,1,0,Meany);
	CvMat* u = cvCreateMat(1,1, CV_32FC1 );
	u->data.fl[0]=1;
	
	//Kalman Predict
	const CvMat* predict = cvKalmanPredict(kalman,u);

	//Kalman Correct
	const CvMat* correct= cvKalmanCorrect(kalman, measurement);
	
	return correct->data.fl;

}

float* updateKalman(CvKalman * kalman){
	
	CvMat* u = cvCreateMat(1,1, CV_32FC1 );
	u->data.fl[0]=1;

	//Kalman Predict
	const CvMat* predict = cvKalmanPredict(kalman,u);

	//Kalman Correct
	//const CvMat* correct= cvKalmanCorrect(kalman, measurement);
	
	return predict->data.fl;

}