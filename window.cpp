//Header Declaration
#include "cv.h"
#include "highgui.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

#include <stdlib.h>
//defines 
#define NUM_MAX_FILES_PER_FOLDER	4000
#define NAME_BUFFER_LEN				128	// in sizeof(char)

//system(">dir /B /A:D >folderList.txt");
//*******************************structure*****************************************************//
/*
typedef struct File_List_Struct
{
	int		numFile;
	int		numDir;
	//int		currFramNum;

	char	PicName[128];
	//char	CurrPicName[128];
	char	DirName[128];

	char**	strInputPicNameArr;

}File_List_Struct; 

File_List_Struct*	gFileList;  //global structure 
*/
//*******************************The Slide caculated for the pole**********************************************//
float sinTheta, cosTheta;
float sinThetaS[128], cosThetaS[128];
//*******************************Dynamical line exhibition*****************************************************//
IplImage *selectedImg= 0;
IplImage* tempImg;
//*******************************Variables*************************************************// 
int n_pos;
int distance_array[1024][3];
int success= 0;                   //whether loading input image is successful
int width_ground, height_ground;   //background image width and height
int width_img, height_img;      //foreground image width and height
CvSize img_size;
/*const*/ char background[]= "background";  //window name
IplImage *groundImg_org=0, *groundImg_org_text= 0, *image_org=0, *groundImg= 0, *image= 0, *standard= 0, *foreImg= 0, *groundImg_copy= 0;  //background image and foreground image
int img_num = 0;        // the nth input image
int folder_num= 0;
char inputName[128];    //input image name
int pushed_Button;     //the value  of the button that is pushed
int delay1= 100, delay2;                 //the delay by ms
static int times=0;      //time could be at most 3, choose dot by mouse 3 times
int pointarray[1024][2];//= { { 0,0 },{ 0,0 },{ 0,0 } };  //record dots chosen
int pointarrayAll[1024][6];
int offset_x= 34, offset_y= 80;             //offset coordinate of the foreground Image
int offsetNumber_x, offsetNumber_y;      //offset coordinate of the number image
int step1, step2, step3;               //WidthStep
int x,y,z,xx,yy,m,n;
int doExit = 0;
int c, channels1, channels2, channels3;
uchar* data1, *data2, *data3; 
int key;
IplImage *one= 0, *two= 0, *three= 0, *number= 0;
CvScalar CvColorRed = CV_RGB(255,0,0);
CvScalar CvColorGreen = CV_RGB(0,255,0);
CvScalar CvColorBlue = CV_RGB(0,0,255);
CvScalar CvColorWhite = CV_RGB(255,255,255);

int dotx, doty;
int changePic= 0;
//CvMat *intrinsic, *distortion ;
int mouseParam= 5;
float scale;
char folder[128];
char text[128];
char outputimage[128];
//CvMat objectPoints, imagePoints, pointCounts;
FILE *points_2D3D;
float x_scale;
int flag_board_not_enough= 0;


float Next[2][2]= {
	{199, 958},
	{263, 1146}
  };
float Calibration[2][2]= {
	{409, 960},
	{469, 1146}
  };
float Exit[2][2]= {
	{41, 1043},
	{103, 1139}
  };
float UndoMark[2][2]= {
	{303, 956},
	{364, 1144}
  };

//************************Parameters for calibration******************************//
int n_boards = 0;
const int board_dt = 20;
int board_w;
int board_h;
int board_n;
int img_num_calibration= 0;//1;     ning modification: 2/10/2014
char inputName_calibration[128];    //input image name
CvMat* image_points, *object_points, *point_counts, *intrinsic_matrix, *distortion_coeffs, *rotation_vectors, *translation_vectors; 
	int corner_count;
	int successes = 0;
	int step, frame = 0;
	CvPoint2D32f* corners;
	IplImage *image_calibration = 0;
CvMat *intrinsic, *distortion,  *rotation, *translation;
CvSize board_sz;
/**********************************************************************************/
//Flags
int flag_NI= 0;
int flag_UM= 0;
int flag_Cal= 1;
int flag_EXIT= 0;
int flag_else= 0;
int flag_Cali_success= 0;

int flag_read_or_not= 0;
/************************************************************************************/
    int  flag_image;
    int Num_image= 0;	
	char **PicName;
	char cmd_arg[128];
	char PictureList_dir[128];

	 int  flag_image2;
    int Num_image2= 0;	
	char **PicName2;
	char cmd_arg2[128];
	char PictureList_dir2[128];
/***********************************************************************************/




/*
void ErrorHandler(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
*/
/*
void OpenDir()
{
    char** Names;
	char	BoardDir[256];
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	int i, iii, idxVideoName;
		
	// convert wchar_t to char
	size_t				convertedChars = 0;
	size_t				origsize;;

	// initialize file list buffer.
	gFileList			= (File_List_Struct*)malloc(sizeof(File_List_Struct));
	gFileList->numDir	= 0;
	gFileList->numFile	= 0;
	gFileList->strInputPicNameArr = (char**)malloc(NUM_MAX_FILES_PER_FOLDER * sizeof(char*));

	Names		= gFileList->strInputPicNameArr;

	for (i=0; i<NUM_MAX_FILES_PER_FOLDER; i++)
	{
		*(Names + i)	= (char*)malloc(NAME_BUFFER_LEN * sizeof(char));
	}

	//strcpy( BoardDir, "../Cal/boards/");//ning
	//strcpy( gFileList->DirName, BoardDir );

	//iii =	MultiByteToWideChar(CP_ACP, 0, BoardDir, -1, szDir, 0); 
	//iii =	MultiByteToWideChar(CP_ACP, 0, BoardDir, -1, szDir, iii); 


	StringCchCat(szDir, MAX_PATH, TEXT("..\Cal\boards\\*"));

	
	
	//StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		ErrorHandler(TEXT("FindFirstFile"));
		//return dwError;
	} 

	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// . <DIR> or .. <DIR>
			gFileList->numDir ++;
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			// list of each file.
			filesize.LowPart	= ffd.nFileSizeLow;
			filesize.HighPart	= ffd.nFileSizeHigh;

			idxVideoName		= gFileList->numFile;
			gFileList->numFile ++;

			origsize	= wcslen(ffd.cFileName) + 1;

			// save current file name in string array.
			wcstombs_s( &convertedChars,
						*(gFileList->strInputPicNameArr + idxVideoName), 
						origsize, ffd.cFileName, _TRUNCATE );
			// save current file name.
			wcstombs_s( &convertedChars, gFileList->PicName, origsize, ffd.cFileName, _TRUNCATE );
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		ErrorHandler(TEXT("FindFirstFile"));
	}

	FindClose(hFind);
	//return dwError;

}  
*/

void drawStickGroundDot()
{
	    double t[3];// since the 3D point for the bottom of the stick is [0 0 0], so T is what we need in the camera coordinate, according to M'=RM+T , which M is the 3D point and M' is the camera point
     
        CvMat* objectPoints=cvCreateMat( 4, 3, CV_32FC1 );
        CvMat* imagePoints=cvCreateMat( 4, 2, CV_32FC1 );
        
        CvMat* stick3D=cvCreateMat( 4, 3, CV_32FC1 );
        CvMat* stick2D=cvCreateMat( 4, 2, CV_32FC1 );

        CvMat* intrinsic = (CvMat*)cvLoad( "..//Intrinsics.xml" );
        CvMat* distortion = (CvMat*)cvLoad( "..//Distortion.xml" );
        CvMat* rvec=cvCreateMat( 3, 1, CV_32FC1 );
        CvMat* tvec=cvCreateMat( 3, 1, CV_32FC1 );


        CV_MAT_ELEM( *stick3D, float , 0, 0)=0;
        CV_MAT_ELEM( *stick3D, float , 0, 1)=0;
        CV_MAT_ELEM( *stick3D, float , 0, 2)=0;
        CV_MAT_ELEM( *stick3D, float , 1, 0)=0;
        CV_MAT_ELEM( *stick3D, float , 1, 1)=0;
        CV_MAT_ELEM( *stick3D, float , 1, 2)=0;
        CV_MAT_ELEM( *stick3D, float , 2, 0)=0;
        CV_MAT_ELEM( *stick3D, float , 2, 1)=0;
        CV_MAT_ELEM( *stick3D, float , 2, 2)=0;
        CV_MAT_ELEM( *stick3D, float , 3, 0)=0;
        CV_MAT_ELEM( *stick3D, float , 3, 1)=0;
        CV_MAT_ELEM( *stick3D, float , 3, 2)=0;
  
         for (int j=0; j<4; j++)
        {
               CV_MAT_ELEM( *imagePoints, float , j, 0)= int((pointarray[j][0]- offset_x)*x_scale);
               CV_MAT_ELEM( *imagePoints, float , j, 1)= int ((pointarray[j][1]- offset_y)*x_scale);
               CV_MAT_ELEM( *objectPoints, float , j, 0)= 0;
               CV_MAT_ELEM( *objectPoints, float , j, 1)= 0;
               CV_MAT_ELEM( *objectPoints, float , j, 2)= 2*(j+1);
        }      
        
        cvFindExtrinsicCameraParams2(objectPoints, imagePoints, intrinsic, distortion, rvec, tvec, 1);
        cvProjectPoints2(stick3D, rvec, tvec, intrinsic, distortion, stick2D);

        float u, v;  // (u,v)= (    CV_MAT_ELEM( *stick2D, float , 0, 0),  CV_MAT_ELEM( *stick2D, float , 0, 1)    )
        u=   CV_MAT_ELEM( *stick2D, float , 0, 0);
        v=  CV_MAT_ELEM( *stick2D, float , 0, 1);
        cvCircle(  groundImg, cvPoint( int (u/x_scale + offset_x), int (v/x_scale + offset_y)), 3, CvColorBlue, 2, 8, 0 );
	    cvCircle(  groundImg, cvPoint( int (u/x_scale + offset_x), int (v/x_scale + offset_y)), 1,  CvColorRed, 2, 8, 0 );
		cvCircle(  groundImg, cvPoint( int (u/x_scale + offset_x), int (v/x_scale + offset_y)), 1,  CvColorWhite, 1, 8, 0 );
		 //cvCircle(  groundImg, cvPoint( int (u/x_scale + offset_x), int (v/x_scale + offset_y)), 2,  CvColorBlue, 2, 8, 0 );
		cvShowImage( "background", groundImg );
               
        cvReleaseMat(&objectPoints);
        cvReleaseMat(&imagePoints);
        cvReleaseMat(&stick3D);
        cvReleaseMat(&stick2D);
        cvReleaseMat(&intrinsic);
        cvReleaseMat(&distortion);
        cvReleaseMat(&rvec);
        cvReleaseMat(&tvec);

}

void Init_Pole()
{

	PicName = (char **)malloc(256 * sizeof(char *));
	for(n=0;n<256; n++)
	PicName[n] = (char *)malloc(256 * sizeof(char));

	//sprintf( cmd_arg, "dir ..\\Cal\\poles\\ /B /O:D >..\\Cal\\poles\\PolesList.txt");   //modify command argument
	sprintf( cmd_arg, "dir ..\\Cal\\poles\\ /B /O:N >..\\Cal\\PolesList.txt");   //modify command argument
    system(cmd_arg);                                                      //generate Picture list in that folder

    sprintf( PictureList_dir, "..\\Cal\\PolesList.txt");   //derive 
	FILE* fp_PictureList = fopen( PictureList_dir, "r" );
	
	for(int i= 0; i<256; i++)
	{
	  flag_image= fscanf(fp_PictureList, "%s", PicName[i]);                          //derive first folder's name
	  if (flag_image==1)
	  {
	     Num_image++;
	  }
	  //printf("flag_image is: %d\n",flag_image);
	}
    fclose(fp_PictureList );
	 Num_image--;  //exclude the txt file
	//fscanf( fp_PictureList, "%s", &PicName[1]);  
	
    printf("Fist Pic is: %s\n", PicName[0]);
	printf("Num_image is: %d\n", Num_image);
}
void Init_Board()
{

	PicName2 = (char **)malloc(256 * sizeof(char *));
	for(n=0;n<256; n++)
	PicName2[n] = (char *)malloc(256 * sizeof(char));

	//sprintf( cmd_arg2, "dir ..\\Cal\\boards\\ /B /O:D >..\\Cal\\boards\\BoardsList.txt");   //modify command argument
	sprintf( cmd_arg2, "dir ..\\Cal\\boards\\ /B /O:N >..\\Cal\\BoardsList.txt");   //modify command argument
    system(cmd_arg2);                                                      //generate Picture list in that folder

    sprintf( PictureList_dir2, "..\\Cal\\BoardsList.txt");   //derive 
	FILE* fp_PictureList2 = fopen( PictureList_dir2, "r" );
	
	for(int i= 0; i<256; i++)
	{
	  flag_image2= fscanf(fp_PictureList2, "%s", PicName2[i]);                          //derive first folder's name
	  if (flag_image2==1)
	  {
	     Num_image2++;
	  }
	  //printf("flag_image is: %d\n",flag_image);
	}
    fclose(fp_PictureList2 );
	 Num_image2--;  //exclude the txt file
	//fscanf( fp_PictureList, "%s", &PicName[1]);  
	
    //printf("Fist Pic is: %s\n", PicName2[0]);
	//printf("Num_image is: %d\n", Num_image2);
}
int DispImage2(int img_num)
{
	sprintf( inputName, "%s\\%s", folder, PicName[img_num]);
	//sprintf( inputName, "%s%d%s", folder, img_num, ".JPG"); 
   //free(image_org);
   image_org = cvLoadImage( inputName, 1);

   //Make them gray   //cancel this
   groundImg= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
   cvCopy(groundImg_org, groundImg);
   
   image= cvCreateImage( cvSize(image_org->width, image_org->height), IPL_DEPTH_8U, 3);
   cvCopy(image_org, image);

   /*
     //Trying to have a curve to compensate inaccuray near the camera 
   // Build the undistort map that we will use for all subsequent frames
	IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
	IplImage* mapy = cvCreateImage( cvGetSize( image), IPL_DEPTH_32F, 1 );
	cvInitUndistortMap( intrinsic, distortion, mapx, mapy );
	IplImage *t = cvCloneImage(image );	
	cvRemap( t, image, mapx, mapy ); // undistort image
	cvReleaseImage( &t );
	cvNamedWindow("undistorted image");
	cvShowImage("undistorted image", image);
	cvWaitKey(0);
	  //new in v0.9.2
	  */

   //Resize image based on the window/backgroundimage
   width_ground= groundImg->width;
   height_ground= groundImg->height; 
   scale= float(image->width)/float(image->height) ;
   img_size.height=  (0.7*height_ground);
   img_size.width=   scale*(img_size.height);   //so the width/height of image does not change
   standard= cvCreateImage( img_size, IPL_DEPTH_8U, 3);
   cvResize(image,  standard,  CV_INTER_LINEAR );
   /////////////////////////////////////////
    x_scale= (image->height)/(0.7*height_ground);
	////////////////////////////////////////////
   foreImg= standard;  
   //Set image to the right position on top of the ground image
   // write the fore ground image onto the ground image
   step1= groundImg->widthStep;
   step2= foreImg->widthStep;
   data1    = (uchar *)groundImg->imageData;
   data2    = (uchar *)foreImg->imageData;
   channels1 = groundImg ->nChannels;
   channels2 = foreImg->nChannels;
  
   for (y= 0; y< img_size.height; y++)
   {
	   for (x= 0; x< img_size.width; x++)
	   {
            //data1[ (offset_x+x) + (offset_y+y)*step1 + channels1*z ]=   data2[x+ y*step2+ z*channels2]; 
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 0]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 0];
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 1]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 1];
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 2]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 2];
	   }
   }
   //get a copy for re-paint
   groundImg_copy= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
   cvCopy(groundImg, groundImg_copy);
   /**********************************************************/
   cvShowImage( "background", groundImg );
   return 1;
}
int calibration()
{
	board_w = 9;//9;//8;//5;//9; // Board width in squares
	board_h = 6;//4;//6; // Board height 
	//board_w = 8; // Board width in squares
	//board_h = 8; // Board height 
	n_boards = 10; // Number of boards
	board_n = board_w * board_h;
	board_sz = cvSize( board_w, board_h );
	//CvCapture* capture = cvCreateCameraCapture( 0 );
	//assert( capture );

	//cvNamedWindow( "Calibration" );
	// Allocate Sotrage
	/*CvMat**/ image_points		    = cvCreateMat( n_boards*board_n, 2, CV_32FC1 );
	/*CvMat**/ object_points		= cvCreateMat( n_boards*board_n, 3, CV_32FC1 );
	/*CvMat**/ point_counts			= cvCreateMat( n_boards, 1, CV_32SC1 );
	/*CvMat**/ intrinsic_matrix		= cvCreateMat( 3, 3, CV_32FC1 );
	/*CvMat**/ distortion_coeffs	= cvCreateMat( 5, 1, CV_32FC1 );
	/*CvMat**/ rotation_vectors		= cvCreateMat( n_boards, 3, CV_32FC1 );  //new
	/*CvMat**/ translation_vectors	= cvCreateMat( n_boards, 3, CV_32FC1 );   //new

	corners = new CvPoint2D32f[ board_n ];

	   IplImage *image_calibration = 0;
		//sprintf( inputName_calibration, "%s%d%s", "..\\Cal\\boards\\", img_num_calibration, ".JPG");
       sprintf( inputName_calibration, "..\\Cal\\boards\\%s", PicName2[0]);


        if( (image_calibration = cvLoadImage( inputName_calibration, 1)) == 0 )
		{
			printf("Board images cannot found in dirctory: /Calibration Tool v1.0/Cal/boards/ \n");
			
		    return -1;
		}

		//Get resized image for exhibition
		float scale= 0.4;
		CvSize dst_cvsize;
		dst_cvsize.width= image_calibration->width * scale;
		dst_cvsize.height= image_calibration->height * scale;
		IplImage* image_calibration_resized= cvCreateImage(dst_cvsize, image_calibration->depth, image_calibration->nChannels);
		cvResize(image_calibration,image_calibration_resized,1);

	IplImage *gray_image = cvCreateImage( cvGetSize( image_calibration ), 8, 1 );

    //fitin();     //put the picture in the background image

	// Capture Corner views loop until we've got n_boards
	// succesful captures (all corners on the board are found)

	while( successes < n_boards ){
		// Skp every board_dt frames to allow user to move chessboard
		//if( frame++ % board_dt == 0 ){
			// Find chessboard corners:
			int found = cvFindChessboardCorners( image_calibration, board_sz, corners,
				&corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );
            //printf("Doing findcorners\n");
			// Get subpixel accuracy on those corners
			cvCvtColor( image_calibration, gray_image, CV_BGR2GRAY );
			cvFindCornerSubPix( gray_image, corners, corner_count, cvSize( 11, 11 ), 
				cvSize( -1, -1 ), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

			// Draw it
			cvDrawChessboardCorners( image_calibration, board_sz, corners, corner_count, found );
			cvResize(image_calibration,image_calibration_resized,1);
			cvShowImage( "background", image_calibration_resized );
			//printf("Corner count is: %d\n", corner_count);

			// If we got a good board, add it to our data
			if( corner_count == board_n ){
				step = successes*board_n;
				for( int i=step, j=0; j < board_n; ++i, ++j ){
					CV_MAT_ELEM( *image_points, float, i, 0 ) = corners[j].x;
					CV_MAT_ELEM( *image_points, float, i, 1 ) = corners[j].y;
					CV_MAT_ELEM( *object_points, float, i, 0 ) = j/board_w;
					CV_MAT_ELEM( *object_points, float, i, 1 ) = j%board_w;
					CV_MAT_ELEM( *object_points, float, i, 2 ) = 0.0f;
				}
				CV_MAT_ELEM( *point_counts, int, successes, 0 ) = board_n;
               // printf("successes is: %d\n", successes);
				successes++;      //if 8 successes, finish loop
			}
		//} 

		// Handle pause/unpause and ESC
		
		int c = cvWaitKey( 15 );
		if( c == 'p' ){
			c = 0;
			while( c != 'p' && c != 27 ){
				c = cvWaitKey( 250 );
			}
		}
		if( c == 27 )
			return 0;
		
		img_num_calibration++;
		//sprintf( inputName_calibration, "%s%d%s", "..\\Cal\\boards\\", img_num_calibration, ".JPG"); 
       sprintf( inputName_calibration, "%s%s", "..\\Cal\\boards\\", PicName2[img_num_calibration-1]);
	   printf("name is: %s\n", PicName2[img_num_calibration-1]);

        if( (image_calibration = cvLoadImage( inputName_calibration, 1)) == 0 ) 
		{
	    flag_board_not_enough= 1;
        printf("Board images cannot found in dirctory: /Calibration Tool v1.0/Cal/boards/ \n");
		return -1;  // Get next image
		}
		cvResize(image_calibration,image_calibration_resized,1);
		cvShowImage( "background", image_calibration_resized );
		
	} // End collection while loop

	// Allocate matrices according to how many chessboards found
	CvMat* object_points2 = cvCreateMat( successes*board_n, 3, CV_32FC1 );
	CvMat* image_points2 = cvCreateMat( successes*board_n, 2, CV_32FC1 );
	CvMat* point_counts2 = cvCreateMat( successes, 1, CV_32SC1 );
	
	// Transfer the points into the correct size matrices
	for( int i = 0; i < successes*board_n; ++i ){
		CV_MAT_ELEM( *image_points2, float, i, 0) = CV_MAT_ELEM( *image_points, float, i, 0 );
		CV_MAT_ELEM( *image_points2, float, i, 1) = CV_MAT_ELEM( *image_points, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 0) = CV_MAT_ELEM( *object_points, float, i, 0 );
		CV_MAT_ELEM( *object_points2, float, i, 1) = CV_MAT_ELEM( *object_points, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 2) = CV_MAT_ELEM( *object_points, float, i, 2 );
	}

	for( int i=0; i < successes; ++i ){
		CV_MAT_ELEM( *point_counts2, int, i, 0 ) = CV_MAT_ELEM( *point_counts, int, i, 0 );
	}
	cvReleaseMat( &object_points );
	cvReleaseMat( &image_points );
	cvReleaseMat( &point_counts );

	// At this point we have all the chessboard corners we need
	// Initiliazie the intrinsic matrix such that the two focal lengths
	// have a ratio of 1.0

	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0;

	// Calibrate the camera
	cvCalibrateCamera2( object_points2, image_points2, point_counts2, cvGetSize( image_calibration ), 
	//	intrinsic_matrix, distortion_coeffs, NULL, NULL, CV_CALIB_FIX_ASPECT_RATIO );
	intrinsic_matrix, distortion_coeffs, rotation_vectors, translation_vectors, CV_CALIB_FIX_ASPECT_RATIO );
	
	// Save the intrinsics and distortions
	cvSave( "..//Intrinsics.xml", intrinsic_matrix );
	cvSave( "..//Distortion.xml", distortion_coeffs );
	printf("saved!!\n");
	
	cvResize(image_calibration,image_calibration_resized,1);
	cvShowImage( "background", image_calibration_resized ); // Show corrected image
    cvWaitKey( 200 );

	cvReleaseImage(&gray_image);
	cvReleaseImage(&image_calibration);
	cvReleaseImage(&image_calibration_resized);

		// Handle pause/unpause and esc
		/*
		int c = cvWaitKey( 15 );
		if( c == 'p' ){
			c = 0;
			while( c != 'p' && c != 27 ){
				c = cvWaitKey( 250 );
			}
		}
		*/
    flag_NI= 1;

	return 0;
}

void fitin()
{
   groundImg= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
   cvCopy(groundImg_org, groundImg);
   
   image= cvCreateImage( cvSize(image_org->width, image_org->height), IPL_DEPTH_8U, 3);
   cvCopy(image_org, image);
   width_ground= groundImg->width;
   height_ground= groundImg->height; 
   scale= float(image->width)/float(image->height) ;
   img_size.height=  (0.7*height_ground);
   img_size.width=   scale*(img_size.height);   //so the width/height of image does not change
   standard= cvCreateImage( img_size, IPL_DEPTH_8U, 3);
   cvResize(image,  standard,  CV_INTER_LINEAR );
}

void drawlines( int pointarray[1024][2], int n1, int n2)
{
   double temp= (pointarray[n1][0]-pointarray[n2][0])*(pointarray[n1][0]-pointarray[n2][0])
	             +  (pointarray[n1][1]-pointarray[n2][1])*(pointarray[n1][1]-pointarray[n2][1]);
   double dist= sqrt( temp );
   
   CvPoint a= { pointarray[n1][0] ,pointarray[n1][1] };
   CvPoint b= { pointarray[n2][0] ,pointarray[n2][1] };
   cvLine( groundImg, a, b, cvScalarAll(255),
             1, 8, 0 );
   /*
   printf("point1 (x,y) is: (%d, %d)\n", pointarray[n1][0],pointarray[n1][1]);
   printf("point2 (x,y) is: (%d, %d)\n", pointarray[n2][0],pointarray[n2][1]);
   printf("Distance between point 1 and point 2 is: %d\n", int(dist));
   */
   //display the distance in the middle of the line
   
  CvPoint c= { (pointarray[n1][0]+ pointarray[n2][0])/2, (pointarray[n1][1] + pointarray[n2][1])/2};
  sprintf( text, "%d", int(dist));
  CvFont font;
  cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.5f, 0.5f,0,0.5,CV_AA);  
  cvPutText( groundImg, text, c, &font, cvScalar(0, 255, 255, 255));
  //cvShowImage( "background", groundImg );
}

void linkDots( int pointarray[1024][2], int n1, int n2)
{
  /* double temp= (pointarray[n1][0]-pointarray[n2][0])*(pointarray[n1][0]-pointarray[n2][0])
	             +  (pointarray[n1][1]-pointarray[n2][1])*(pointarray[n1][1]-pointarray[n2][1]);
   double dist= sqrt( temp ); */
   
   CvPoint a= { pointarray[n1][0] ,pointarray[n1][1] };
   CvPoint b= { pointarray[n2][0] ,pointarray[n2][1] };
   cvLine( groundImg, a, b, cvScalar(0, 0, 255, 0),
             1, 8, 0 );

   cvShowImage("background",groundImg);
}

void linkDotsTemp( int pointarray[1024][2], int n1, int x, int y)
{
   CvPoint a= { pointarray[n1][0] ,pointarray[n1][1] };
   CvPoint b= { x , y };
   cvLine( tempImg, a, b, cvScalar(0, 0, 255, 0),
             1, 8, 0 );

   cvShowImage("background",tempImg);
}



void distance ( int pointarray[1024][2], int img_num )
{
  double temp1= (pointarray[0][0]-pointarray[1][0])*(pointarray[0][0]-pointarray[1][0])
	             +  (pointarray[0][1]-pointarray[1][1])*(pointarray[0][1]-pointarray[1][1]);
  double temp2= (pointarray[0][0]-pointarray[2][0])*(pointarray[0][0]-pointarray[2][0])
	             +  (pointarray[0][1]-pointarray[2][1])*(pointarray[0][1]-pointarray[2][1]);
  double temp3= (pointarray[1][0]-pointarray[2][0])*(pointarray[1][0]-pointarray[2][0])
	             +  (pointarray[1][1]-pointarray[2][1])*(pointarray[1][1]-pointarray[2][1]);

  double dist_01= sqrt( temp1 );
  double dist_02= sqrt( temp2 );
  double dist_12= sqrt( temp3 );
  /*
  printf("point1 (x,y) is: (%d, %d)\n", pointarray[0][0],pointarray[0][1]);
  printf("point2 (x,y) is: (%d, %d)\n", pointarray[1][0],pointarray[1][1]);
  printf("point3 (x,y) is: (%d, %d)\n", pointarray[2][0],pointarray[2][1]);
  printf("Distance between point 1 and point 2 is: %d\n", int(dist_01));
  printf("Distance between point 1 and point 3 is: %d\n", int(dist_02));
  printf("Distance between point 2 and point 3 is: %d\n", int(dist_12));
  */
  distance_array[img_num][1]= dist_01;
  distance_array[img_num][2]= dist_02;
  distance_array[img_num][3]= dist_12;
  pointarrayAll[img_num][1]= pointarray[0][0];  //x1
  pointarrayAll[img_num][2]= pointarray[0][1];  //y1
  pointarrayAll[img_num][3]= pointarray[1][0];  //x2
  pointarrayAll[img_num][4]= pointarray[1][1];  //y2
  pointarrayAll[img_num][5]= pointarray[2][0];  //x3
  pointarrayAll[img_num][6]= pointarray[2][1];  //y3

}

void writeNumber (int times, int pointarray[1024][2])
{

  dotx= pointarray[times][0];
  doty= pointarray[times][1];
  sprintf( text, "%d", times+1);
 /*
 cvInitFont( CvFont* font, int font_face,
                         double hscale, double vscale,
                         double shear CV_DEFAULT(0),
                         int thickness CV_DEFAULT(1),
                         int line_type CV_DEFAULT(8));
						 */
 CvFont font;
 cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.5f, 0.5f,0,1,CV_AA);
 cvPutText( groundImg, text, cvPoint(dotx, doty), &font, cvScalar(228, 26, 3, 255));
 cvShowImage( "background", groundImg );
 
}

void paintNumber ( int times, int pointarray[1024][2] )
{
  dotx= pointarray[times][0];
  doty= pointarray[times][1];

  if (times == 1)
  {
	  //paint 1
      //number = one;
	  //cvReleaseImage(&number);
	  number= cvCreateImage( cvSize(one->width, one->height), IPL_DEPTH_8U, 3);
      cvCopy(one, number);
  }
  else if (times == 2)
  {
	  //paint 2
	  //number = two;
	  //cvReleaseImage(&number);
	  number= cvCreateImage( cvSize(two->width, two->height), IPL_DEPTH_8U, 3);
      cvCopy(two, number);
  }
  else if (times == 0)
  {
	  //paint 3
	  //number = three;
	  //cvReleaseImage(&number);
	  number= cvCreateImage( cvSize(three->width, three->height), IPL_DEPTH_8U, 3);
      cvCopy(three, number);
  }
  else //if (times!=4)
  {
	 // printf("times = 0");
  }
   

   step1= groundImg->widthStep;
   step3= number->widthStep;
   data1    = (uchar *)groundImg->imageData;
   channels1 = groundImg ->nChannels;
   data3    = (uchar *)number->imageData;
   channels3 = number->nChannels;
   if (times==0)
   {
   offsetNumber_x= pointarray[2][0];
   offsetNumber_y= pointarray[2][1];
   //printf("offsetNumber_x= %d\n",pointarray[2][0]);
   //printf("offsetNumber_y= %d\n",pointarray[2][1]);
   }
   else
   {
   offsetNumber_x= pointarray[times-1][0];
   offsetNumber_y= pointarray[times-1][1];
   //printf("offsetNumber_x= %d\n",pointarray[times-1][0]);
   //printf("offsetNumber_y= %d\n",pointarray[times-1][1]);
   }
  
   for (y= 0; y< number->height; y++)
   {
	   for (x= 0; x< number->width; x++)
	   {
            //data1[ (offset_x+x) + (offset_y+y)*step1 + channels1*z ]=   data2[x+ y*step2+ z*channels2]; 
			((uchar *)(groundImg->imageData + (offsetNumber_y+ y)*step1 ))[(offsetNumber_x+ x)*channels1 + 0]= ((uchar *)(number->imageData + y*step3))[x*channels3 + 0];
			((uchar *)(groundImg->imageData + (offsetNumber_y+ y)*step1 ))[(offsetNumber_x+ x)*channels1 + 1]= ((uchar *)(number->imageData + y*step3))[x*channels3 + 1];
			((uchar *)(groundImg->imageData + (offsetNumber_y+ y)*step1 ))[(offsetNumber_x+ x)*channels1 + 2]= ((uchar *)(number->imageData + y*step3))[x*channels3 + 2];
	   }
   }
  cvShowImage( "background", groundImg );
  cvWaitKey(0);
}

void buttonLight ( float Array[2][2] )
{
   for (xx= Array[0][1]; xx< Array[1][1]; xx++)
	{
      for (yy= Array[0][0]; yy< Array[1][0]; yy++)
	  {
		     ((uchar *)(groundImg->imageData + yy*step1))[xx*channels1 + 0]= 255;
	  }
	}

}

void DispImage( int img_num )
{
   sprintf( inputName, "%s%d%s", folder, img_num, ".JPG"); 
  // free(image_org);
   image_org = cvLoadImage( inputName, 1);

   //Make them gray   //cancel this
   groundImg= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
   cvCopy(groundImg_org, groundImg);
   
   image= cvCreateImage( cvSize(image_org->width, image_org->height), IPL_DEPTH_8U, 3);
   cvCopy(image_org, image);

   //Resize image based on the window/backgroundimage
   width_ground= groundImg->width;
   height_ground= groundImg->height; 
   scale= float(image->width)/float(image->height) ;
   img_size.height=  (0.7*height_ground);
   img_size.width=   scale*(img_size.height);   //so the width/height of image does not change
   standard= cvCreateImage( img_size, IPL_DEPTH_8U, 3);
   cvResize(image,  standard,  CV_INTER_LINEAR );
   
   ///////////////////////////////////////////
   x_scale= (image->height)/(0.7*height_ground);
   ///////////////////////////////////////////

   foreImg= standard;
   
   //caculate camera's extrinsic parameters based on foreImg
   //we need to 
    
   /*
    // Build the undistort map that we will use for all subsequent frames
	IplImage* mapx = cvCreateImage( cvGetSize(foreImg), IPL_DEPTH_32F, 1 );
	IplImage* mapy = cvCreateImage( cvGetSize( foreImg ), IPL_DEPTH_32F, 1 );
	cvInitUndistortMap( intrinsic, distortion, mapx, mapy );
	IplImage *t = cvCloneImage(foreImg );	
	cvRemap( t, foreImg, mapx, mapy ); // undistort image
	cvReleaseImage( &t );
	cvNamedWindow("undistorted image");
	cvShowImage("undistorted image", foreImg);
	cvWaitKey(0);
	*/
  
   //Set image to the right position on top of the ground image
   // write the fore ground image onto the ground image
   step1= groundImg->widthStep;
   step2= foreImg->widthStep;
   data1    = (uchar *)groundImg->imageData;
   data2    = (uchar *)foreImg->imageData;
   channels1 = groundImg ->nChannels;
   channels2 = foreImg->nChannels;
  
   for (y= 0; y< img_size.height; y++)
   {
	   for (x= 0; x< img_size.width; x++)
	   {
            //data1[ (offset_x+x) + (offset_y+y)*step1 + channels1*z ]=   data2[x+ y*step2+ z*channels2]; 
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 0]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 0];
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 1]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 1];
			((uchar *)(groundImg->imageData + (offset_y+y)*step1 ))[(offset_x+x)*channels1 + 2]= ((uchar *)(foreImg->imageData + y*step2))[x*channels2 + 2];
	   }
   }
   //get a copy for re-paint
   groundImg_copy= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
   cvCopy(groundImg, groundImg_copy);

   cvShowImage( "background", groundImg );
}


void onMouse(int event,int x,int y,int flags,void* param) 
{ 
	switch(event){
      case CV_EVENT_LBUTTONDOWN:
        if(flags & CV_EVENT_FLAG_CTRLKEY) 
          printf("Left button down with CTRL pressed\n");
        break; 

	  case	CV_EVENT_MOUSEMOVE:
		  if(times>0){
			   if (tempImg==NULL){
                                tempImg = cvCloneImage(groundImg);
                                cvRectangle(tempImg,
                                        cvPoint(x - 1, y - 1),
                                        cvPoint(x + 1, y + 1),
                                        cvScalar(0, 0,  255,0), 2, 8, 0);

                                 //adjust the lines
                                 linkDotsTemp( pointarray, times-1, x, y); 
			   }
		  }
		  cvReleaseImage(&tempImg);
          break;
 

      case CV_EVENT_LBUTTONUP:

 if( (x> UndoMark[0][1])&& (x< UndoMark[1][1]) &&(y> UndoMark[0][0]) &&(y< UndoMark[1][0]) )  
{
	if (flag_UM)
	{
    printf("ClearAll\n");
	cvCopy(groundImg_copy, groundImg);
	buttonLight ( UndoMark );
	cvShowImage( "background", groundImg );
	cvWaitKey(3);

	cvCopy(groundImg_copy, groundImg);
	cvShowImage( "background", groundImg );
	times= 0;
	flag_else= 1;
	}
	else{};
	
    /*  This is for newer versions. To be modified.
	if ( times == 1)
	{
    
    cvCopy(groundImg_copy, groundImg);
	buttonLight ( UndoMark );
	//cvCircle(  groundImg, cvPoint(pointarray[times-1][0],pointarray[times-1][1]), 3, CvColorRed, 4, 8, 0 );
	cvShowImage( "background", groundImg );
	}
	else if( times == 2)
	{
	 cvCopy(groundImg_copy, groundImg);
	 buttonLight ( UndoMark );
     //cvCircle(  groundImg, cvPoint(pointarray[times-2][0],pointarray[times-2][1]), 3, CvColorRed, 4, 8, 0 );
	 cvCircle(  groundImg, cvPoint( pointarray[times-1][0],pointarray[times-1][1]), 3, CvColorRed, 4, 8, 0 );
	 cvShowImage( "background", groundImg );
	}
	else if( times == 0)
	{
	 cvCopy(groundImg_copy, groundImg);
	 buttonLight ( UndoMark );
     cvCircle(  groundImg, cvPoint(pointarray[times][0],pointarray[times][1]), 3, CvColorRed, 4, 8, 0 );
	 cvCircle(  groundImg, cvPoint(pointarray[times+1][0],pointarray[times+1][1]), 3, CvColorRed, 4, 8, 0 );
	 cvShowImage( "background", groundImg );
	}
	*/

}

else if( (x> Next[0][1])&& (x< Next[1][1]) &&(y> Next[0][0]) &&(y< Next[1][0]) )
{
	if(flag_NI)
	{
	flag_NI= 1; //0
    flag_UM= 1;
    flag_Cal= 0;
    flag_EXIT= 1;
    flag_else= 1;
    //doNext();
	//printf("Next\n");
    //sprintf(outputimage, "%s/%2d%s","..//Save",img_num,".jpg");
	//printf("output path is: %s\n",outputimage);
	//cvSaveImage(outputimage,groundImg,0);

    //fprintf( points_2D3D, "%s\n", outputimage );
    fprintf( points_2D3D, "\n");
	buttonLight ( Next );
	cvShowImage( "background", groundImg );
	cvWaitKey(3);
	
	img_num++;
	//DispImage(img_num);
    ///////////////////////////////////////////////////////////////
	//read in every four pole positions when clicking next
	if(flag_read_or_not)
	{
    fprintf( points_2D3D, "%d %d %d %d %d\n", int((pointarray[0][0]- offset_x)*x_scale), int((pointarray[0][1]- offset_y)*x_scale), 0, 0, 2 );
    fprintf( points_2D3D, "%d %d %d %d %d\n", int((pointarray[1][0]- offset_x)*x_scale), int((pointarray[1][1]- offset_y)*x_scale), 0, 0, 4 );
    fprintf( points_2D3D, "%d %d %d %d %d\n", int((pointarray[2][0]- offset_x)*x_scale), int((pointarray[2][1]- offset_y)*x_scale), 0, 0, 6 );
	fprintf( points_2D3D, "%d %d %d %d %d\n", int((pointarray[3][0]- offset_x)*x_scale), int((pointarray[3][1]- offset_y)*x_scale), 0, 0, 8 );
	}
	flag_read_or_not= 0;
	///////////////////////////////////////////////////////////////
	if(img_num >10)
	{
	DispImage2(img_num);
	CvPoint d = { 55, 656 };
    sprintf( text,  "More than 10 poles collected." );	        
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.7f, 0.7f,0,1,CV_AA);
    cvPutText( groundImg, text, d, &font, cvScalar(0, 255, 0, 255));  //in Green

	sprintf( text,  "You can Press Exit to calibrate..." );
	d.y += 22;
	cvPutText( groundImg, text, d, &font, cvScalar(0, 255, 0, 255));  //in Green
	}

	//sprintf( inputName, "%s%d%s", folder, img_num+1, ".JPG"); 
	sprintf( inputName, "%s\\%s", folder, PicName[img_num+1]);
	if  ( (image_org = cvLoadImage( inputName, 1))==0)    
	{
        DispImage2(img_num);
	    CvPoint d = { 60, 660 };
        sprintf( text, "No more images in this folder. Press Exit to save.");
        CvFont font;
        cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.7f, 0.7f, 0, 1, CV_AA);
        cvPutText( groundImg, text, d, &font, cvScalar(255, 255, 255, 255));

		cvShowImage("background",groundImg);
	    cvSetMouseCallback("background",onMouse,&mouseParam);
	
	flag_NI= 0;//1;//0
    flag_Cal= 0;
    flag_EXIT= 1;
	flag_else= 0;
	 
	}
	else
	{
	DispImage2(img_num);
	}


	cvSetMouseCallback("background",onMouse,&mouseParam);
    cvWaitKey(0);
	}
	else{}
}
		/*
else if( (x> Prev[0][1])&& (x< Prev[1][1]) &&(y> Prev[0][0]) &&(y< Prev[1][0]) )
{
   //doPrevious();
	printf("Previous\n");
	img_num--;
	buttonLight ( Prev );
	cvShowImage( "background", groundImg );
	cvWaitKey(3);
	//cvDestroyWindow( "background" );
	DispImage(img_num);
	cvSetMouseCallback("background",onMouse,&mouseParam);
    cvWaitKey(0);
}
*/
else if( (x> Calibration[0][1])&& (x< Calibration[1][1]) &&(y> Calibration[0][0]) &&(y< Calibration[1][0]) )
{
	if(flag_Cal)
	{
	flag_NI= 0;
    flag_UM= 1;
    flag_Cal= 0;
    flag_EXIT= 1;
	flag_else= 1;
    // doCalibration();
	//printf("Calibration\n");
	calibration(); 
    DispImage2(img_num);
	//*************************************************************************************************
	if(flag_board_not_enough)
	{
	flag_NI= 0;
    flag_UM= 0;
    flag_Cal= 0;
    flag_EXIT= 1;
	flag_else= 0;
	CvPoint d = { 50, 648 };
    sprintf( text,  "Calibration part 1 incomplete..." 
		            "Board images not enough..."
					"Please collect more board images and try again.");
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.5f, 0.5f,0,1,CV_AA);
    cvPutText( groundImg, text, d, &font, cvScalar(0, 0, 255, 255));
	}
	else
	{
	CvPoint d = { 50, 648 };
    sprintf( text,  "Calibration part 1 done..." 
		            "Intrinsic parameters set..."
					"Please select points for poles.");
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.5f, 0.5f,0,1,CV_AA);
    cvPutText( groundImg, text, d, &font, cvScalar(255, 255, 255, 255));
	}
	//*************************************************************************************************
	cvShowImage("background",groundImg);
	cvSetMouseCallback("background",onMouse,&mouseParam);
    cvWaitKey(0);
	}
	else
	{}

	

		/*
	if ( times == 1)
	{
    
    cvCopy(groundImg_copy, groundImg);
	buttonLight ( Clear );
	//cvCircle(  groundImg, cvPoint(pointarray[times-1][0],pointarray[times-1][1]), 3, CvColorRed, 4, 8, 0 );
	cvShowImage( "background", groundImg );
	}
	else if( times == 2)
	{
	 cvCopy(groundImg_copy, groundImg);
	 buttonLight ( Clear );
     //cvCircle(  groundImg, cvPoint(pointarray[times-2][0],pointarray[times-2][1]), 3, CvColorRed, 4, 8, 0 );
	 cvCircle(  groundImg, cvPoint( pointarray[times-1][0],pointarray[times-1][1]), 3, CvColorRed, 4, 8, 0 );
	 cvShowImage( "background", groundImg );
	}
	else if( times == 0)
	{
	 cvCopy(groundImg_copy, groundImg);
	 buttonLight ( Clear );
     cvCircle(  groundImg, cvPoint(pointarray[times][0],pointarray[times][1]), 3, CvColorRed, 4, 8, 0 );
	 cvCircle(  groundImg, cvPoint(pointarray[times+1][0],pointarray[times+1][1]), 3, CvColorRed, 4, 8, 0 );
	 cvShowImage( "background", groundImg );
	}
	*/
}
/*
else if( (x> ClearAll[0][1])&& (x<ClearAll[1][1]) &&(y>ClearAll[0][0]) &&(y<ClearAll[1][0]) )
{
   //doClearAll();
	printf("ClearAll\n");
	cvCopy(groundImg_copy, groundImg);
	buttonLight ( ClearAll );
	cvShowImage( "background", groundImg );
	
}
*/
else if( (x> Exit[0][1])&& (x< Exit[1][1]) &&(y> Exit[0][0]) &&(y< Exit[1][0]) )
{
   //doExit();
	
	//fclose(points_2D3D);
	if(flag_EXIT && flag_Cali_success)
	{
    fclose(points_2D3D);
	std::ifstream fin;
	double t[3];// since the 3D point for the bottom of the stick is [0 0 0], so T is what we need in the camera coordinate, according to M'=RM+T , which M is the 3D point and M' is the camera point
	fin.open("..//points_2D3D.txt", std::ifstream::in);
	//fin.open(argv[1], std::ifstream::in);
	if(!fin){std::cout<<"can't open file"<<std::endl; break;}
	
	CvMat* objectPoints=cvCreateMat( 4, 3, CV_32FC1 );
	CvMat* imagePoints=cvCreateMat( 4, 2, CV_32FC1 );
	CvMat* stick3D=cvCreateMat( 4, 3, CV_32FC1 );

		CV_MAT_ELEM( *stick3D, float, 0, 0)=0;
		CV_MAT_ELEM( *stick3D, float, 0, 1)=0;
		CV_MAT_ELEM( *stick3D, float, 0, 2)=0;
		/*
		CV_MAT_ELEM( *stick3D, float, 1, 0)=0.1;
		CV_MAT_ELEM( *stick3D, float, 1, 1)=0.1;
		CV_MAT_ELEM( *stick3D, float, 1, 2)=0.1;
		CV_MAT_ELEM( *stick3D, float, 2, 0)=-0.1;
		CV_MAT_ELEM( *stick3D, float, 2, 1)=-0.1;
		CV_MAT_ELEM( *stick3D, float, 2, 2)=-0.1;
        */
		CV_MAT_ELEM( *stick3D, float, 1, 0)=0;
		CV_MAT_ELEM( *stick3D, float, 1, 1)=0;
		CV_MAT_ELEM( *stick3D, float, 1, 2)=0;
		CV_MAT_ELEM( *stick3D, float, 2, 0)=0;
		CV_MAT_ELEM( *stick3D, float, 2, 1)=0;
		CV_MAT_ELEM( *stick3D, float, 2, 2)=0;
		CV_MAT_ELEM( *stick3D, float, 3, 0)=0;
		CV_MAT_ELEM( *stick3D, float, 3, 1)=0;
		CV_MAT_ELEM( *stick3D, float, 3, 2)=0;
	
	CvMat* stick2D=cvCreateMat( 4, 2, CV_32FC1 );

	std::ofstream fout;
	fout.open("..\\stick2D_camera3D.txt" , std::fstream::out);


	CvMat* intrinsic = (CvMat*)cvLoad( "..//Intrinsics.xml" );
	CvMat* distortion = (CvMat*)cvLoad( "..//Distortion.xml" );

	CvMat* rvec=cvCreateMat( 3, 1, CV_32FC1 );
	CvMat* tvec=cvCreateMat( 3, 1, CV_32FC1 );


	while(!fin.eof())
	{
		for(int j=0; j<4; j++)
		{
			fin >>  CV_MAT_ELEM( *imagePoints, float, j, 0);
			fin >>  CV_MAT_ELEM( *imagePoints, float, j, 1);
			fin >> CV_MAT_ELEM( *objectPoints, float, j, 0);
			fin >> CV_MAT_ELEM( *objectPoints, float, j, 1);
			fin >> CV_MAT_ELEM( *objectPoints, float, j, 2);
		}
		
		
		cvFindExtrinsicCameraParams2(objectPoints, imagePoints, intrinsic, distortion, rvec, tvec, 1);
		cvProjectPoints2(stick3D, rvec, tvec, intrinsic, distortion, stick2D);

		fout<< CV_MAT_ELEM( *stick2D, float, 0, 0) <<' ' << CV_MAT_ELEM( *stick2D, float, 0, 1)<<' ';

		CvMat _t = cvMat( tvec->rows, tvec->cols, CV_MAKETYPE(CV_64F,CV_MAT_CN(tvec->type)), t );
		cvConvert( tvec, &_t );
		// camera 3D
		fout<< t[0] <<' ' << t[1] <<' ' <<t[2] << std::endl;	
	}
	cvDestroyWindow( "background" );

	cvReleaseMat(&objectPoints);
	cvReleaseMat(&imagePoints);
	cvReleaseMat(&stick3D);
	cvReleaseMat(&stick2D);
	cvReleaseMat(&intrinsic);
	cvReleaseMat(&distortion);
	cvReleaseMat(&rvec);
	cvReleaseMat(&tvec);

	}
	else
	{
		fclose(points_2D3D);
		cvDestroyWindow( "background" );
	}
}
else    
{        
	if(flag_else)
	{
     //get a red dot where clicked by mouse
    cvCircle(  groundImg, cvPoint(x,y), 2, CvColorRed, 2, 8, 0 ); 
    cvShowImage( "background", groundImg );
    pointarray[times][0]=x;
    pointarray[times][1]=y;
    //printf("The %d dot chosen is:(%d, %d)\n ", times+1 ,x, y);
    //fprintf( points_2D3D, "%d %d %d %d %d\n", int((x- offset_x)*x_scale), int((y- offset_y)*x_scale), 0, 0, 2*(times+1) );

/////////////////////////////////////
    sprintf(outputimage, "%s/%2d%s","..//Save", img_num,".jpg");
	//printf("output path is: %s\n",outputimage);
    cvCircle(  image, cvPoint(int((x- offset_x)*x_scale), int((y- offset_y)*x_scale)), 3,  CvColorGreen, 2, 8, 0 );
	
    cvSaveImage(outputimage,image,0);
//////////////////////////////////////////////
	//*************************************************************************************************
    pointarray[times][0]=x;
    pointarray[times][1]=y;
	writeNumber(times, pointarray);
	//*******************************************************************************************
	CvPoint d = { 50+ times*220, 678 };
    sprintf( text,  "%d%s", times+1, " out of 4 points selected.");
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.5f, 0.5f,0,1,CV_AA);
    cvPutText( groundImg, text, d, &font, cvScalar(255, 255, 255, 255));
    cvShowImage("background",groundImg);
	//cvSetMouseCallback("background",onMouse,&mouseParam);
    //cvWaitKey(0);
	//************************************************************************************************* 
	flag_read_or_not= 1; //now able to read
	flag_NI= 0; //added

     if(times> 0) {
	    linkDots(pointarray, times, times-1);
	 }

        if (times== 3)
        {
          times= -1;//drawlines(pointarray, times, times-1);
		  flag_else= 0;
		  flag_NI= 1;      //when clicked four dots, can move on to next image

		  drawStickGroundDot();
        }
		/*
		if (times== 0)
		{
		  flag_NI= 1;     //when haven't clicked once, can move on to next image
		}
		*/
     times++;
     //writeNumber(times-1, pointarray);
        break;
	}//end if 
	else{}
    }//end else

}//end switch
}//end onMouse
//Get to know the number of images in the folder
int LoadImgsFromDir( std::string &strSubDirName,std::vector<std::string>&m_szImgs)  
{  
    WIN32_FIND_DATAA stFD = {0};  
    std::string strDirName;  
    
    strDirName = strSubDirName;  
    
    std::string strFindName = strDirName + "//*";  
    HANDLE hFile = FindFirstFileA(strFindName.c_str(), &stFD);  
    BOOL bExist = FindNextFileA(hFile, &stFD);  
      
    for (;bExist;)  
    {  
        std::string strTmpName = strDirName + stFD.cFileName;  
        if (strDirName + "." == strTmpName || strDirName + ".." == strTmpName)  
        {  
            bExist = FindNextFileA(hFile, &stFD);  
            continue;  
        }  
     //   if (PathIsDirectoryA(strTmpName.c_str()))  
       // {  
         //   strTmpName += "//";  
           // LoadImgsFromDir(strTmpName,m_szImgs);  
            //bExist = FindNextFileA(hFile, &stFD);  
            //continue;  
        //}  
        std::string strSubImg = strDirName + stFD.cFileName;  
        m_szImgs.push_back(strSubImg);  
        bExist = FindNextFileA(hFile, &stFD);  
    }  
     
    return m_szImgs.size();  
} 



//Main function
int main( int argc, char** argv )
{
	Init_Pole();
	Init_Board();
	
	//system("dir ..\\ /B /A:D >..\\Release\\folderList.txt");
    // Load camera's intrinsic parameters
	/*CvMat * */intrinsic = (CvMat*)cvLoad( "Intrinsics.xml" );
	/*CvMat * */distortion = (CvMat*)cvLoad( "Distortion.xml" );
	
	// camera's extrinsic parameters will be caculated when animal image is loaded


   //create a window
   //success= cvNamedWindow( "background", CV_WINDOW_AUTOSIZE );
   success= cvNamedWindow( "background", 1 ); //2/18/2014 ning
	if (success == 0)
   {
	   //printf("Can't create window, please contact 911.");
       cvWaitKey( delay1 ); //Time to see this warning
	   return -1;
   }
   else
   {
   //Load ground image
                                    
	 if( (groundImg_org = cvLoadImage( "..\\img\\background2.jpg", 1)) == 0 ) 
	 {
		printf("background2.jpg is missing\n");
		return -1;
	 }
   }

   //Load number image
   if( (one = cvLoadImage( "..\\img\\one.JPG", 1)) == 0 )   
   {
	    printf("one.jpg is missing\n");
		return -1;
   }
   if( (two = cvLoadImage( "..\\img\\two.JPG", 1)) == 0 )  
   {
	    printf("two.jpg is missing\n");
		return -1;
   }
   if( (three = cvLoadImage( "..\\img\\three.JPG", 1)) == 0 )  
   {
	    printf("three.jpg is missing\n");
		return -1;
   }
    //Calculate the image number in the folder
    /*std::string floder = "C:\\10.4\\data\\";
    std::vector<std::string> pic_name;
    int n_pos = LoadImgsFromDir(floder,pic_name);
	*/

   //start from the first image of the first folder 
   folder_num= 1;
   img_num= 0;   // 1;    ning modification: 2/10/2014
   //sprintf( folder, "%s%d%s", "..\\Cal\\poles\\", folder_num, "\\"); 
   sprintf( folder, "%s", "..\\Cal\\poles\\"); 
   


   
   //sprintf( inputName_calibration, "%s%d%s", "..\\Cal\\boards\\", img_num_calibration, ".JPG"); 
   sprintf( inputName_calibration, "%s%s", "..\\Cal\\boards\\", PicName2[img_num_calibration]); 
   if( (image_calibration = cvLoadImage( inputName_calibration, 1)) == 0 )
   {

	flag_NI= 0;
    flag_UM= 0;
    flag_Cal= 0;
    flag_EXIT= 1;
	flag_else= 0;
	flag_Cali_success= 0;

    groundImg_org_text= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
    cvCopy(groundImg_org, groundImg_org_text);
    CvPoint d = { 55, 656 };
    sprintf( text,  "No Calibration data available.");
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.7f, 0.7f,0,1,CV_AA);
    cvPutText( groundImg_org_text, text, d, &font, cvScalar(255, 255, 255, 255));

	d.y += 22;
    sprintf( text,  "Please put in boards and poles images.");
	cvPutText( groundImg_org_text, text, d, &font, cvScalar(255, 255, 255, 255));
    cvShowImage( "background", groundImg_org_text );
	mouseParam= 5;
    cvSetMouseCallback("background",onMouse,&mouseParam);
    cvWaitKey(0);
   }
   else
   {
    flag_Cali_success= 1;
    //DISPLAY SOME INSTRUCTIONS
    groundImg_org_text= cvCreateImage( cvSize(groundImg_org->width, groundImg_org->height), IPL_DEPTH_8U, 3);
    cvCopy(groundImg_org, groundImg_org_text);
    CvPoint d = { 55, 660 };
    sprintf( text,  "Please click Calibration first to get the intrinsic parameters of the camera.");
    CvFont font;
    cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX ,0.7f, 0.7f,0,1,CV_AA);
    cvPutText( groundImg_org_text, text, d, &font, cvScalar(255, 255, 255, 255));
    cvShowImage( "background", groundImg_org_text );

	 //Points2D3D
	points_2D3D= fopen("..\\points_2D3D.txt","w");
    fprintf( points_2D3D, "\n");

    //Respond to the mouse click
    mouseParam= 5;
    cvSetMouseCallback("background",onMouse,&mouseParam);
    cvWaitKey(0);

   }





   if (img_num== n_pos)
        {
			printf("This is the last picture..");
        }
   
   //output resized image with dots on it

}



/*
char text[128];
 
       sprintf(text, "Frame %5d", pBKSegmentPara->iFrameNum);
       cvInitFont(&font1, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);
       cvPutText(frameCopy, text, cvPoint(10, 460), &font1, cvScalar(255, 255, 255, 255));
	   */