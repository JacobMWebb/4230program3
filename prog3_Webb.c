/*
   Jacob Webb
   Program 3
   CSCE 4230 Computer Graphics
   Dr. Renka

   This program uses OpenGL to animate a scene. Several spheres(3-dimensional)are used
   along with hexagons, lines, and a disk. The spheres have perpetually increasing/decreasing radius
   along with the distance of the hexagons.
   
	This can be compiled on the CSE machines with the makefile that is on Dr. Renka's page.
	
	TO COMPILE:***
	This can be compiled on a Apple computer with the following pasted into terminal:

	gcc prog3_Webb.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL
	
	TO USE:***
	The required functionality of "+", "-", "escape", and "space" are activated as stated.
   You can adjust main rotations angles with 1,2,3,4,5, and 6.
   Pressing 'p' reduces the animation speed to be what I believe is fastest, while still noticable. 
 
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>//for using sin/cos functions and M_PI
#include <time.h>//not sure if I needed for glutelapsedtime, but kept anyways.

//Globals ***
int adjusting_radius = 0; //adjust the size of our spheres
int bounce = 1; //make the hexagons bounce from 0 radius to 8 radius and back.
GLfloat spin_speed = 0.0; //for GLRotatef()
GLfloat spin_increment = 2.0; //again for rotate
GLfloat length_of_wire = 8.0; //hexagon distance
GLfloat radius_of_bob = 1.0; //hexagon radius

GLfloat ROTATE_Y = 0.5; //This will be used to rotate about the entire y-axis
GLfloat ROTATE_X = 0.1; //This will be used to rotate about the entire x-axis
GLfloat ROTATE_Z = 1.0; //This will be used to rotate about the entire z-axis
GLfloat animation_speed = 240; //starting slow, 240ms wait time
GLUquadric* my_circle; //for drawing spheres
GLUquadric* my_required_disk; //in case i had not fulfilled assignment
//these are for animations speeds
GLfloat time_since_last_check = 0;
GLfloat prev_time = 0;
//End of Globals ***


GLfloat Convert_Degrees_To_Radians(float degrees) { //took me ah while to figure out everything was in radians.
   GLfloat to_return = degrees*M_PI/180;
   return to_return;
}

void display(void) {
   glClear(GL_COLOR_BUFFER_BIT); //clear screen
   glPushMatrix();
   glRotatef(spin_speed, ROTATE_X, ROTATE_Y, ROTATE_Z);  //start by rotating entire model view slightly tilted in x and y
                                                         //to show 3-d effect
   my_circle = gluNewQuadric(); //going to draw spheres, because 2-D disks are not as cool
   gluQuadricOrientation(my_circle, GLU_OUTSIDE);
   gluQuadricDrawStyle(my_circle, GLU_POINT); //points looks like fireworks, GLU_LINES also looks interesting
   my_required_disk = gluNewQuadric(); //to draw disk
   glColor3f(0, 0, 1);
   
   glPushMatrix(); //create a very thin disk rotated about the x and y axis 45 degrees.
      glRotatef(45, 1, 1, 0); 
      gluDisk(my_required_disk, 9, 10, 30, 50);  //inner then outer radius
   glPopMatrix();
   


   GLdouble variable_radius = 1; //for quadrant spheres
   GLint variable_slices = 100;
   GLint variable_stacks = 70;

   GLint slice = 300; //for the big sphere in center
   GLint stack = 200;
   
   glColor3f(0.0, 0.5, 0.0); //green inner sphere
   gluSphere(my_circle, 11-adjusting_radius, slice-=20, stack-=30); //sphere around the axis
   
   glColor3f(0.5, 0.0, 0.7); //purple spheres
   glPushMatrix(); //push/pop each new matrix to make translation easier.
      glTranslatef(5, 5, 0);  //quadrant 1
      gluSphere(my_circle, variable_radius+adjusting_radius, variable_slices+=20, variable_stacks+=30);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(-5, 5, 0); //quadrant 2
      gluSphere(my_circle, variable_radius+adjusting_radius, variable_slices+=20, variable_stacks+=30);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(-5, -5, 0); //quadrant 3
      gluSphere(my_circle, variable_radius+adjusting_radius, variable_slices+=20, variable_stacks+=30);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(5, -5, 0); //quadrant 4
      gluSphere(my_circle, variable_radius+adjusting_radius, variable_slices+=20, variable_stacks+=30);
   glPopMatrix();
   
   glBegin(GL_LINES); //draw the lines that the bobs are attached to.
      glColor3f(0.0, 1.0, 0.0); //Green axis is x-axis
      glVertex2f(-length_of_wire, 0.0);
      glVertex2f(length_of_wire, 0.0);
      glColor3f(0.0, 0.0, 1.0); //blue axis is y-axis
      glVertex2f(0.0, length_of_wire);
      glVertex2f(0.0, -length_of_wire);
   glEnd();
   

   //The following draws hexagons, slightly adjusted from program 2
   glColor3f(1.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
   glBegin(GL_POLYGON); //draw our hexagon
      glVertex2f(radius_of_bob, -length_of_wire); // far right
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), -length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper right
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), -length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper left
      glVertex2f(-radius_of_bob, -length_of_wire); //far left
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), -length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom left
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), -length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom right
   glEnd();

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
   glBegin(GL_POLYGON); //draw our hexagon
      glVertex2f(radius_of_bob, length_of_wire); // far right
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper right
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper left
      glVertex2f(-radius_of_bob, length_of_wire); //far left
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom left
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom right
   glEnd();

   glColor3f(1.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
   glBegin(GL_POLYGON); //draw our hexagon
      glVertex2f(length_of_wire, radius_of_bob); //top 
      glVertex2f(length_of_wire - (radius_of_bob * sin(Convert_Degrees_To_Radians(60))), radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //upper right
      glVertex2f(length_of_wire - (radius_of_bob * sin(Convert_Degrees_To_Radians(60))), -radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //upper left
      glVertex2f(length_of_wire, -radius_of_bob); //far left
      glVertex2f(length_of_wire + (radius_of_bob*sin(Convert_Degrees_To_Radians(60))), -radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //bottom left
      glVertex2f(length_of_wire + (radius_of_bob*sin(Convert_Degrees_To_Radians(60))), radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //bottom right
   glEnd();

   glColor3f(1.0,0.0,0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
   glBegin(GL_POLYGON); //draw our hexagon
      glVertex2f(-length_of_wire, radius_of_bob); //top 
      glVertex2f(-length_of_wire - (radius_of_bob * sin(Convert_Degrees_To_Radians(60))), radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //upper right
      glVertex2f(-length_of_wire - (radius_of_bob * sin(Convert_Degrees_To_Radians(60))), -radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //upper left
      glVertex2f(-length_of_wire, -radius_of_bob); //far left
      glVertex2f(-length_of_wire + (radius_of_bob*sin(Convert_Degrees_To_Radians(60))), -radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //bottom left
      glVertex2f(-length_of_wire + (radius_of_bob*sin(Convert_Degrees_To_Radians(60))), radius_of_bob * cos(Convert_Degrees_To_Radians(60))); //bottom right
   glEnd();

   glColor3f(1.0,0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
   glBegin(GL_POLYGON); //draw our hexagon
      glVertex2f(radius_of_bob, length_of_wire); // far right
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper right
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire + radius_of_bob *sin(Convert_Degrees_To_Radians(60))); //upper left
      glVertex2f(-radius_of_bob, length_of_wire); //far left
      glVertex2f(-radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom left
      glVertex2f(radius_of_bob*cos(Convert_Degrees_To_Radians(60)), length_of_wire - radius_of_bob * sin(Convert_Degrees_To_Radians(60))); //bottom right
   glEnd();

   glPopMatrix();

   glutSwapBuffers();
}

void spinDisplay(void) {
   spin_speed = spin_speed + spin_increment;
   if(spin_increment >= 10) //cap out spin speed 
      spin_increment = 10;
   else if(spin_increment <= -1)
      spin_increment = -10;
   
   time_since_last_check = glutGet(GLUT_ELAPSED_TIME);
   if(time_since_last_check - prev_time > animation_speed) { //elapsed time is in milliseconds
      prev_time = time_since_last_check;

      if(length_of_wire >= 1 && bounce == 1) { //complicated if/else statements that 
         length_of_wire--;                      //bounds the hexagons from length 0 to 9
         if(length_of_wire == 0)
            bounce = 0;
      }
      else if(length_of_wire <= 9 && bounce == 0) {
         length_of_wire++;
         if(length_of_wire == 10)
            bounce = 1;
      } //end of hexagon bounding

      if(bounce == 1) //using the logic for hexagon distance, bound the sphere radii
         adjusting_radius++;
      else if(bounce == 0 && adjusting_radius > 1)
         adjusting_radius--;
      glutPostRedisplay();     
   }
   glutPostRedisplay(); //need to call display again, otherwise it would stall in between animations
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void reshape(int w, int h) {
   
   if(w <= h) { //The following is to keep the viewport square if the user creates an oddly shaped window.
      glViewport(0, (GLsizei)(h-w)/2, (GLsizei)w, (GLsizei)w); //if you try to make vertical rectangle, preserve shape
   }
   else if(w > h){
      glViewport((GLsizei)(w-h)/2, 0, (GLsizei)h, (GLsizei)h); //horizontal rectangle, preserve shape
   }
   else {
      glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   }
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-15, 15, -15, 15, -15, 15.0); 
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(spinDisplay);
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}

void keyboard(unsigned char key, int x, int y) {
   if(key == '=') {
      animation_speed = 75;
      glutIdleFunc(spinDisplay);
   }
   else if(key == '-') {
      animation_speed = 200;
      glutIdleFunc(spinDisplay);
   }
   else if(key == '0') {
      animation_speed = 350;
      glutIdleFunc(spinDisplay);
   }
   else if(key == 'p') { //this is very fast. 25ms delay
      animation_speed = 25;
      glutIdleFunc(spinDisplay);
   }
   else if(key == 32) //Space
      glutIdleFunc(NULL); //Stop spinning
   else if(key == 27)
      exit(1);//exit peacefully
   else if(key == 'm' && length_of_wire > 1) { //pressing m DECREASES the length
      if(length_of_wire < 3)
         length_of_wire = length_of_wire-0.1;
      else
         length_of_wire--;
   }
   else if(key == 'n' && length_of_wire < 12) { //pressing n INCREASES the length
      if(length_of_wire > 7)
         length_of_wire = length_of_wire+0.1;
      else
         length_of_wire++;
   }
   else if(key == '1') //these are controls that you can use to have fun with rotation angle, not part of assingment
      ROTATE_X = ROTATE_X + .1;
   else if(key == '2')
      ROTATE_X = ROTATE_X - .1;
   else if(key == '3')
      ROTATE_Y = ROTATE_Y + .1;
   else if(key == '4')
      ROTATE_Y = ROTATE_Y - .1;
   else if(key == '5')
      ROTATE_Z = ROTATE_Z + .1;
   else if(key == '6')
      ROTATE_Z = ROTATE_Z - .1;
   else if(key == 'r') { //reset angles
      ROTATE_X = 0.0;
      ROTATE_Y = 0.0;
      ROTATE_Z = 1.0;
   }

}


void mymenu(int item_selected) {
   if(item_selected == '=')
      animation_speed = 75;
   else if(item_selected == '-')
      animation_speed = 200;
   else if(item_selected == '0')
      animation_speed = 350;
   else if(item_selected == 'p')
      animation_speed = 40;
   else if(item_selected == 32) //space stop
      glutIdleFunc(NULL);
   else if(item_selected == 27) //escape
      exit(1);
}


void CreateMenu() {
   glutCreateMenu(mymenu);
   glutAddMenuEntry("Press '+' for fast speed animation", '+');
   glutAddMenuEntry("Press '-' for medium speed animation", '-');
   glutAddMenuEntry("Press '0' for slow speed animation", '0');
   glutAddMenuEntry("Press 'p' for VERY fast speed animation", 'p');
   glutAddMenuEntry("Press <space> to stop animation", 32);
   glutAddMenuEntry("Press <escape> to exit the program", 27);
   glutAttachMenu(GLUT_RIGHT_BUTTON); //where to open the menu
}

int main(int argc, char** argv) {
   srand(NULL);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Program 3 Webb,Jacob");
   init();
   CreateMenu();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;   
}
