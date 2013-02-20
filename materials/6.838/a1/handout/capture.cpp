#include "cv.hpp" 
#include "opencv2/opencv.hpp"
 #include "highgui.h" 
 #include <stdio.h>
#include <iostream>
using namespace cv;
 #include <GL/gl.h>
 #include <GL/glu.h>
 #include <GL/freeglut.h>
 #include <cmath>
 #include <time.h>
 #include <stdlib.h>
 #include <string>
 #include <sstream>
 #include <vector>
 #define TEX_SIZE 1024
 #define IX(xx,yy) ((xx) + (yy) * TEX_SIZE)

///@TODO change the following 5 parameters according to your need
//Screen width of your primary display so that the scan line
//can be displayed to the projector instead of the primary display.
//It's a hack.
int screenSize=1366 ;
int winID=0;
int resx=832;
int resy=624;

int imgNum = 0;
int viewNum = 0;
int globeTime = -100;
VideoCapture capture ;
 void init(void)
 {
   glClearColor (0, 0, 0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_ALPHA);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-resx,resx,-resy,resy, 0.1, 10);
   
 }
 
 void display(void)
 {
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   gluLookAt(0,0,10,
             0,0,0,
             0.0, 1.0, 0.0);
   glColor3f(0,0,0);

   ///@TODO draw scanline here
   
   glFinish();
   glutSwapBuffers();
  //a sync hack to make sure the first image is completely drawn
  if(globeTime<0){
     globeTime++;
     std::cout<<globeTime<<"\n";
     return;
   }
   Mat image;
//Another sync hack to make sure the camera returns the most updated image
//instead of buffered image.
//A better way is to use threads and mutex. Create a thread that just keeps
//grabbing frames from the camera.
   bool result = capture.read(image);
   result = capture.read(image);
   result = capture.read(image);
   if ( !result ) {
     fprintf( stderr, "ERROR: frame is null...\n" );
     getchar();
   }
   else{
     std::stringstream filename;
     filename<<"data"<<viewNum<<"/";
     filename<<imgNum<<".png";
     std::cout<<"save image "<<filename.str()<<"\n";
     imwrite(filename.str().c_str(),image); 
     imgNum++;   
   }
}
 
 void reshape (int w, int h)
 {
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0,resx,0,resy,0.1,10);
 }
 
 void keyboard(unsigned char key,int x, int y)
 {
   
   switch(key){
     case 27: // Escape key
    printf("exit\n");
    glutDestroyWindow(winID);
    exit (0);
    break;
   }
   glutPostRedisplay();
 }
 
 void mouse(int button, int state, int x, int y)
 {
   switch (button) {
     case GLUT_LEFT_BUTTON:
       switch (state){
         case GLUT_DOWN:
           break;
         case GLUT_UP:
           break;
       }
       break;
         default:
           break;
   }
 }
 void motion (int x, int y)
 {
 }
 
 void animate(int t)
 {
   glutTimerFunc(20, animate, 0);
   glutPostRedisplay();
 }
 
 int main(int argc, char** argv)
 {
   capture.open( CV_CAP_ANY );
   if ( !capture.isOpened() ) {
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
     return -1;
   }
   capture.set(CV_CAP_PROP_FRAME_WIDTH, 1600);
   capture.set( CV_CAP_PROP_FRAME_HEIGHT, 1200);
   
   if(argc>1){
     viewNum = atoi(argv[1]);
   }
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH|GLUT_ALPHA );
   glutInitWindowSize (resx , resy);
   //moving window to secondary display does not work on os x
   glutInitWindowPosition (screenSize, 0);
   winID = glutCreateWindow (argv[0]);
   glutFullScreen();
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(20, animate, 0);
   
   Mat image;
  //a sync hack so that the camera updates to use your parameters
   for(int ii = 0;ii<50;ii++){
     capture.read(image);
     std::cout<<ii<<"\n";
   }
   glutMainLoop();
   return 0;
 }
 
