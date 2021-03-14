#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define UP 1
#define RIGHT 2
#define LOOK 3

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double rotate_angle;

struct point
{
	double x,y,z;
};

struct point pos;
struct point u, r, l;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int slices, int stacks) {

}

void drawSS()
{
    glColor3f(1,0,0);
    //drawSquare(20);
    drawSphere(20, 24, 20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    //drawSquare(15);
    drawSphere(15, 24, 20);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        //drawSquare(10);
        drawSphere(10, 24, 20);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    //drawSquare(5);
    drawSphere(5, 24, 20);
}

void rotate_camera(struct point a, struct point b, double theta, bool clockwise, int dir){
    struct point p, c;

    if(clockwise) {
        p.x = (a.y*b.z - a.z*b.y);
        p.y = (a.z*b.x - a.x*b.z);
        p.z = (a.x*b.y - a.y*b.x);
    }
    else {
        p.x = (b.y*a.z - b.z*a.y);
        p.y = (b.z*a.x - b.x*a.z);
        p.z = (b.x*a.y - b.y*a.x);
    }
    //printf("perpendicular to a and b: p.x: %f, p.y: %f, p.z: %f\n", p.x, p.y, p.z);

	c.x = a.x * cos(theta) + p.x * sin(theta);
	c.y = a.y * cos(theta) + p.y * sin(theta);
	c.z = a.z * cos(theta) + p.z * sin(theta);

	double unit = sqrt(c.x*c.x + c.y*c.y + c.z*c.z);

	if(dir == LOOK){
        l.x = c.x/unit;
        l.y = c.y/unit;
        l.z = c.z/unit;
        //printf("l: l.x: %f, l.y: %f, l.z: %f\n", l.x, l.y, l.z);
	}
	else if (dir == RIGHT) {
        r.x = c.x/unit;
        r.y = c.y/unit;
        r.z = c.z/unit;
        //printf("r: r.x: %f, r.y: %f, r.z: %f\n", r.x, r.y, r.z);
	}
	else {
        u.x = c.x/unit;
        u.y = c.y/unit;
        u.z = c.z/unit;
        //printf("u: u.x: %f, u.y: %f, u.z: %f\n", u.x, u.y, u.z);
	}
}

void reset_pos() {
    pos = {100, 100, 25};
	u = {0, 0, 1};
	r = {-1.0/sqrt(2), 1.0/sqrt(2), 0};
	l = {-1.0/sqrt(2), -1.0/sqrt(2), 0};
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//look left
			//rotate counterclockwise l, r w.r.t u
			rotate_camera(l, u, rotate_angle, false, LOOK);
			rotate_camera(r, u, rotate_angle, false, RIGHT);
			break;
		case '2':
			//look right
			//rotate clockwise l, r w.r.t u
			rotate_camera(l, u, rotate_angle, true, LOOK);
			rotate_camera(r, u, rotate_angle, true, RIGHT);
			break;
		case '3':
			//look up
			//rotate counterclockwise l, u w.r.t r
			rotate_camera(l, r, rotate_angle, false, LOOK);
			rotate_camera(u, r, rotate_angle, false, UP);
			break;
		case '4':
			//look down
			//rotate clockwise l, u w.r.t r
			rotate_camera(l, r, rotate_angle, true, LOOK);
			rotate_camera(u, r, rotate_angle, true, UP);
			break;
		case '5':
			//tilt clockwise
			//rotate clockwise r, u w.r.t l
			rotate_camera(r, l, rotate_angle, true, RIGHT);
			rotate_camera(u, l, rotate_angle, true, UP);
			break;
		case '6':
			//tilt counterclockwise
			//rotate counterclockwise r, u w.r.t l
			rotate_camera(r, l, rotate_angle, false, RIGHT);
			rotate_camera(u, l, rotate_angle, false, UP);
			break;
        case '0':
            reset_pos();
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.x += -l.x;
			pos.y += -l.y;
			pos.z += -l.z;
			break;
		case GLUT_KEY_UP:		//up arrow key
			//cameraHeight += 3.0;
			pos.x += l.x;
			pos.y += l.y;
			pos.z += l.z;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x += r.x;
			pos.y += r.y;
			pos.z += r.z;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x += -r.x;
			pos.y += -r.y;
			pos.z += -r.z;
			break;

		case GLUT_KEY_PAGE_UP:
			pos.x += u.x;
			pos.y += u.y;
			pos.z += u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.x += -u.x;
			pos.y += -u.y;
			pos.z += -u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
	gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	rotate_angle=pi/12.0;

	pos = {100, 100, 25};
	u = {0, 0, 1};
	r = {-1.0/sqrt(2), 1.0/sqrt(2), 0};
	l = {-1.0/sqrt(2), -1.0/sqrt(2), 0};

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("OUR OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
