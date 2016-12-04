// Blue   - H-(123-125) S-(151-155) L-(70-82)           R-30 G-124 B-136 // H-187 S-78 V-53
// Orange - H-(26-28) S-(207-213) L-(104-112)           R-222 G-150 B-16 // H-39 S-93 V-87
// Green  - H-(46-48) S-(231-240) L-(72-75)             R-127 G-154 B-2  // H-71 S-98.7 V-60.4
// Pink   - H-(228-230) S-(127-130) L-(140-143)         R-207 G-93 B-124
// Yellow - H-(38-40) S-(217-226) L-(80-88)             R-170 G-168 B-7

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace cv;

int main( int argc, char** argv ) 
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    
    unsigned long long int flag1,flag2,flag3,flag4;
    long long int x,x1,y,y1;
	
    flag1=flag2=flag3=flag4=0;
    int c = 0;
    
    CvCapture* capture = cvCaptureFromCAM(0);
    
    if(!cvQueryFrame(capture))
    { 
	cout<<"Video camera capture status: OK"<<endl;
    }
    else
    {
	cout<<"Video capture failed, please check the camera."<<endl;
    }
	
    CvSize sz = cvGetSize(cvQueryFrame( capture));
    IplImage* img = cvCreateImage( sz,8, 3 );
    
    IplImage* imgHSV = cvCreateImage(cvSize(300,310), 8, 3);
    IplImage* imgThreshed1 = cvCreateImage(cvSize(300,310), 8, 1);
    IplImage* imgThreshed2 = cvCreateImage(cvSize(300,310), 8, 1);
    IplImage* imgThreshed3 = cvCreateImage(cvSize(300,310), 8, 1);
    
    while( c != 27)
    {
	flag1+=1;
	flag3+=1;
	int e,r;
	x=y=x1=y1=0;
	img = cvQueryFrame(capture);
	cvSetImageROI(img, cvRect(340,100,360,310));	
	//CvSize sz1 = cvGetSize(img);
        cvCvtColor(img, imgHSV, CV_BGR2HSV);
        cvInRangeS(imgHSV, cvScalar(20, 40, 100), cvScalar(32, 255, 255), imgThreshed1); //yellow (final) - Left Click
	cvInRangeS(imgHSV, cvScalar(120, 120, 100), cvScalar(180, 255, 255), imgThreshed2); //pink (final) - Position
	cvInRangeS(imgHSV, cvScalar(90, 100, 100), cvScalar(130, 255, 255), imgThreshed3); //blue (final) - Right Click
	
	//  -- yellow --   -- yellow --   -- yellow --   -- yellow --   -- yellow --   -- yellow --   -- yellow --
	{
	    CvMemStorage* storage = cvCreateMemStorage();
	    CvSeq* first_contour = NULL;
	    CvSeq* maxitem=NULL;
	    int cn=cvFindContours(imgThreshed1,storage,&first_contour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
	    double area,max_area=0.0;
	    CvSeq* ptr=0;
	    if(cn>0)
	    {
		for(ptr=first_contour;ptr!=NULL;ptr=ptr->h_next)
		{
		    area=fabs(cvContourArea(ptr,CV_WHOLE_SEQ,0));
		    if(area>max_area)
		    {
		    	max_area=area;
		    	maxitem=ptr;
		    	//maxn=n;
		    }
	            //n++;
		}
			
		if(max_area > 50)
		{
		    CvPoint pt0;
		    CvMemStorage* storage1 = cvCreateMemStorage();
		    CvMemStorage* storage2 = cvCreateMemStorage(0);
		    CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage1 );
		    CvSeq* hull;
		    CvSeq* defects;
				
		    for(int i = 0; i < maxitem->total; i++ )
        	    {
			CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, maxitem, i );
            		pt0.x = p->x;
            		pt0.y = p->y;
            		cvSeqPush( ptseq, &pt0 );
        	    }
        	    hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
        	    int hullcount = hull->total;
        	    defects= cvConvexityDefects(ptseq,hull,storage2  );
        	    //pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
        	    //printf("** : %d :**",hullcount);
        	    CvConvexityDefect* defectArray; 
        	    for(int i = 0; i < hullcount; i++ )
        	    {
            		CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
            		cvLine( img, pt0, pt, CV_RGB( 0, 255, 0 ), 1, CV_AA, 0 );
            		pt0 = pt;
            		//flag2+=1;
            		flag1=0;
        	    }
        	    // Free memory.         
		    free(defectArray);
		    cvReleaseMemStorage( &storage1 );
		    cvReleaseMemStorage( &storage2 );
        	}
            }
            cvReleaseMemStorage( &storage );
        }
        if(flag1==0)
        {
            flag2++;
            if(flag2==1)
            mouse_event(MOUSEEVENTF_LEFTDOWN,e,r,35,70);
        }
        else if(flag1==1)
        {
            mouse_event(MOUSEEVENTF_LEFTUP,e,r,0,0);
            flag2=0;
        }
        else
        {
            //nothing
        }
        
        //  -- blue --   -- blue --   -- blue --   -- blue --   -- blue --   -- blue --   -- blue --
	{
	    CvMemStorage* storage = cvCreateMemStorage();
	    CvSeq* first_contour = NULL;
	    CvSeq* maxitem=NULL;
	    int cn=cvFindContours(imgThreshed3,storage,&first_contour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
	    double area,max_area=0.0;
	    CvSeq* ptr=0;
	    if(cn>0)
	    {
	 	for(ptr=first_contour;ptr!=NULL;ptr=ptr->h_next)
		{
		    area=fabs(cvContourArea(ptr,CV_WHOLE_SEQ,0));
		    if(area>max_area)
		    {
		    	max_area=area;
		    	maxitem=ptr;
		    	//maxn=n;
		    }
	            //n++;
		}
			
		if(max_area > 50)
		{
		    CvPoint pt0;
		    CvMemStorage* storage1 = cvCreateMemStorage();
		    CvMemStorage* storage2 = cvCreateMemStorage(0);
		    CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage1 );
		    CvSeq* hull;
		    CvSeq* defects;
				
		    for(int i = 0; i < maxitem->total; i++ )
        	    {
		        CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, maxitem, i );
            		pt0.x = p->x;
            		pt0.y = p->y;
            		cvSeqPush( ptseq, &pt0 );
        	    }
        	    hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
        	    int hullcount = hull->total;
        	    defects= cvConvexityDefects(ptseq,hull,storage2  );
        	    //pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
        	    //printf("** : %d :**",hullcount);
        	    CvConvexityDefect* defectArray; 
        	    for(int i = 0; i < hullcount; i++ )
        	    {
            		CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
            		cvLine( img, pt0, pt, CV_RGB( 255, 0, 0 ), 1, CV_AA, 0 );
            		pt0 = pt;
            		flag3=0;
        	    }
        	    // Free memory.         
		    free(defectArray);
		    cvReleaseMemStorage( &storage1 );
		    cvReleaseMemStorage( &storage2 );
        	}
            }
            cvReleaseMemStorage( &storage );
        }
        if(flag3==0)
        {
            flag4++;
            if(flag4==1)
            mouse_event(MOUSEEVENTF_RIGHTDOWN,e,r,35,70);
        }
        else if(flag3==1)
        {
            mouse_event(MOUSEEVENTF_RIGHTUP,e,r,0,0);
            flag4=0;
        }
        else
        {
            //nothing	
        }
        
        //    -- pink --   -- pink --   -- pink --   -- pink --   -- pink --   -- pink --   -- pink --
        {
	    CvMemStorage* storage = cvCreateMemStorage();
	    CvSeq* first_contour = NULL;
	    CvSeq* maxitem=NULL;
	    int cn=cvFindContours(imgThreshed2,storage,&first_contour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
	    double area,max_area=0.0;
	    CvSeq* ptr=0;
	    if(cn>0)
	    {
		for(ptr=first_contour;ptr!=NULL;ptr=ptr->h_next)
		{
		    area=fabs(cvContourArea(ptr,CV_WHOLE_SEQ,0));
		    if(area>max_area)
		    {
		    	max_area=area;
		    	//maxn=n;
		    	maxitem=ptr;
		    }
	            //n++;
		}
		if(max_area > 50)
		{
		    POINT l;
		    CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
		    cvMoments(imgThreshed2, moments, 1);
		    double moment10 = cvGetSpatialMoment(moments, 1, 0);
		    double moment01 = cvGetSpatialMoment(moments, 0, 1);
		    double area = cvGetCentralMoment(moments, 0, 0);
		    int posX=moment10/area;
		    int posY=moment01/area;
		    GetCursorPos(&l);  //Get current position of cursor
		    if(posX >= 0 && posX < 110)
		    {
			if(posY >= 0 && posY < 110)
			{
			    x1=l.x-3;
			    y1=l.y-3;
			    SetCursorPos(x1,y1);  //Set the position of cursor according to the mapping
			    SetConsoleTextAttribute(hConsole, 13);
			    cout<<"Pink Lady\n";
			}
			else if(posY >= 110 && posY < 190)
			{
			    x1=l.x-3;
			    y1=l.y;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 6);
			    cout<<"Yellow Sun\n";
			}
			else if(posY >= 190 && posY < 311)
			{
			    x1=l.x-3;
			    y1=l.y+3;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 3);
			    cout<<"Matrix Green\n";
			}
			else
			{
			    SetConsoleTextAttribute(hConsole, 15);
			    cout<<"Not in Range\n";
			}
		    }
		    else if(posX >= 110 && posX < 190)
		    {
			if(posY >= 0 && posY < 110)
			{
			    x1=l.x;
			    y1=l.y-3;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 1);
			    cout<<"Blue Water\n";
			}
			else if(posY >= 110 && posY < 190)
			{
			    SetConsoleTextAttribute(hConsole, 12);
			    cout<<"Red Block\n";
			}
			else if(posY >= 190 && posY < 311)
			{
			    x1=l.x;
			    y1=l.y+3;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 11);
			    cout<<"Aqua Fresh\n";
			}
			else
			{
			    SetConsoleTextAttribute(hConsole, 15);
			    cout<<"Not in Range\n";
			}
		    }
		    else if(posX >= 190 && posX < 301)
		    {
			if(posY >= 0 && posY < 110)
			{
			    x1=l.x+3;
			    y1=l.y-3;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 5);
			    cout<<"Purple\n";
			}
			else if(posY >= 110 && posY < 190)
			{
			    x1=l.x+3;
			    y1=l.y;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 14);
			    cout<<"Yellow Girl\n";
			}
			else if(posY >= 190 && posY < 311)
			{
			    x1=l.x+3;
			    y1=l.y+3;
			    SetCursorPos(x1,y1);
			    SetConsoleTextAttribute(hConsole, 10);
			    cout<<"Lemon Green\n";
			}
			else
			{
			    SetConsoleTextAttribute(hConsole, 15);
			    cout<<"Not in Range\n";
			}
		    }
		    else
		    {
			SetConsoleTextAttribute(hConsole, 15);
			cout<<"Not in Range\n";
		    }
				
		    CvPoint pt0;
		    CvMemStorage* storage1 = cvCreateMemStorage();
		    CvMemStorage* storage2 = cvCreateMemStorage(0);
		    CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),sizeof(CvPoint), storage1 );
		    CvSeq* hull;
		    CvSeq* defects;
			
		    for(int i = 0; i < maxitem->total; i++ )
        	    {
		        CvPoint* p = CV_GET_SEQ_ELEM( CvPoint, maxitem, i );
            		pt0.x = p->x;
            		pt0.y = p->y;
            		cvSeqPush( ptseq, &pt0 );
        	    }
        	    hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
        	    int hullcount = hull->total;
        	    defects= cvConvexityDefects(ptseq,hull,storage2  );
        	    //pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
        	    //printf("** : %d :**",hullcount);
        	    CvConvexityDefect* defectArray; 
        	    for(int i = 0; i < hullcount; i++ )
        	    {
            		CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
            		cvLine( img, pt0, pt, CV_RGB( 0, 0, 255 ), 1, CV_AA, 0 );
            		pt0 = pt;
        	    }
        	    // Free memory.         
		    free(defectArray);
		    cvReleaseMemStorage( &storage1 );
		    cvReleaseMemStorage( &storage2 );
        	}
            }
            cvReleaseMemStorage( &storage );
        }
        
        cvRectangle(img,cvPoint(2,2),cvPoint(295,305),CV_RGB(255,255,255),1,CV_AA,0);
	cvLine(img, cvPoint(110,0), cvPoint(110,308),CV_RGB(0,255,0),1, CV_AA, 0 );
	cvLine(img, cvPoint(190,0), cvPoint(190,308),CV_RGB(0,255,0),1, CV_AA, 0 );
	cvLine(img, cvPoint(0,110), cvPoint(298,110),CV_RGB(255,255,0),1, CV_AA, 0 );
	cvLine(img, cvPoint(0,190), cvPoint(298,190),CV_RGB(255,255,0),1, CV_AA, 0 );

	//cvNamedWindow( "imgHSV", CV_WINDOW_AUTOSIZE );cvShowImage( "imgHSV", imgHSV );
	//cvNamedWindow( "imgThreshed1", CV_WINDOW_AUTOSIZE );cvShowImage( "imgThreshed1", imgThreshed1 );
	//cvNamedWindow( "imgThreshed2", CV_WINDOW_AUTOSIZE );cvShowImage( "imgThreshed2", imgThreshed2 );
	//cvNamedWindow( "imgThreshed3", CV_WINDOW_AUTOSIZE );cvShowImage( "imgThreshed3", imgThreshed3 );
	cvNamedWindow( "img1", CV_WINDOW_AUTOSIZE );cvShowImage( "img1", img );
	c = cvWaitKey(30);
	cvResetImageROI(img);
	cvNamedWindow( "img", CV_WINDOW_AUTOSIZE );cvShowImage( "img", img );
    }
    cvReleaseCapture( &capture);
    cvDestroyAllWindows();
}
