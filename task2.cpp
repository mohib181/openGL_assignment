#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double square_len;
double circle_rad;
double bubble_rad;

int total_circles;
int drawn_circles;
bool play;

clock_t current_time;

struct point
{
	double x,y,z;
};

double speed;
int trapped[5];
struct point positions[5];
struct point velocity[5];

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
	glBegin(GL_LINES);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);

		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);

		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);

		glVertex3f(-a, a,2);
		glVertex3f( a, a,2);
	}glEnd();
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
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
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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


void updatePositions() {
	for (int i = 0; i < drawn_circles; i++) {
		positions[i].x += velocity[i].x * speed;
		positions[i].y += velocity[i].y * speed;
		positions[i].z += velocity[i].z * speed;
		//printf("v: %f, %f, %f\n", velocity[i].x, velocity[i].y, velocity[i].z);
	}
}

void boundaryCheckSqaure() {
	for (int i = 0; i < drawn_circles; i++) {
		if(positions[i].x >= (square_len - bubble_rad) || positions[i].x <= -(square_len - bubble_rad)) {
			velocity[i].x = -velocity[i].x;
		}
		if(positions[i].y >= (square_len - bubble_rad) || positions[i].y <= -(square_len - bubble_rad)) {
			velocity[i].y = -velocity[i].y;
		}
	}
}

void updateCircleStatus() {
	for (int i = 0; i < drawn_circles; i++) {
		struct point next = {positions[i].x+velocity[i].x, positions[i].y+velocity[i].y, 0};
        double d1 = sqrt(positions[i].x*positions[i].x + positions[i].y*positions[i].y);
        double d2 = sqrt(next.x*next.x + next.y*next.y);

        if (d1 >= (circle_rad-bubble_rad) && d2 <= (circle_rad-bubble_rad)) trapped[i] = 1;
	}
}

bool hitCircle(int i) {
    struct point next = {positions[i].x+velocity[i].x, positions[i].y+velocity[i].y, 0};
	double d1 = sqrt(positions[i].x*positions[i].x + positions[i].y*positions[i].y);
	double d2 = sqrt(next.x*next.x + next.y*next.y);

	if (d1 <= (circle_rad-bubble_rad) && d2 >= (circle_rad-bubble_rad)) return true;
	return false;
}

void circleCollisionDetect() {
	for (int i = 0; i < drawn_circles; i++) {
		for (int j = i+1; j < drawn_circles; j++) {
			if(trapped[i] && trapped[j]) {
				struct point next_i = {positions[i].x+velocity[i].x, positions[i].y+velocity[i].y, 0};
				struct point next_j = {positions[j].x+velocity[j].x, positions[j].y+velocity[j].y, 0};

				double d1 = sqrt((positions[i].x-positions[j].x)*(positions[i].x-positions[j].x) + (positions[i].y-positions[j].y)*(positions[i].y-positions[j].y));
				double d2 = sqrt((next_i.x-next_j.x)*(next_i.x-next_j.x) + (next_i.y-next_j.y)*(next_i.y-next_j.y));

				if (d1 >= (2*bubble_rad) && d2 <= (2*bubble_rad)) {
                    double unit;
					struct point c = {positions[i].x - positions[j].x, positions[i].y - positions[j].y, 0};
					//double unit = sqrt(c.x*c.x + c.y*c.y + c.z*c.z);

					/*velocity[i].x = c.x/unit;
					velocity[i].y = c.y/unit;

					velocity[j].x = -c.x/unit;
					velocity[j].y = -c.y/unit;*/

					velocity[i].x += c.x;
					velocity[i].y += c.y;

					unit = sqrt(velocity[i].x*velocity[i].x + velocity[i].y*velocity[i].y + velocity[i].z*velocity[i].z);
					velocity[i].x /= unit;
					velocity[i].y /= unit;

					velocity[j].x -= c.x;
					velocity[j].y -= c.y;

					unit = sqrt(velocity[j].x*velocity[j].x + velocity[j].y*velocity[j].y + velocity[j].z*velocity[j].z);
					velocity[j].x /= unit;
					velocity[j].y /= unit;

				}
			}
		}
	}
}

void boundaryCheckCircle() {
	for (int i = 0; i < drawn_circles; i++)
	{
		if (hitCircle(i))
		{
			double dot = velocity[i].x*positions[i].x + velocity[i].y*positions[i].y;

			double vx = velocity[i].x - 2*dot*positions[i].x;
			double vy = velocity[i].y - 2*dot*positions[i].y;
			double unit = sqrt(vx*vx + vy*vy);
			vx /= unit;
			vy /= unit;

			velocity[i].x = vx;
			velocity[i].y = vy;
		}
	}

}

void drawBubble(struct point c) {
	glPushMatrix();

	glColor3f(1, 0.6, 0);
	glTranslatef(c.x, c.y, c.z);
	drawCircle(bubble_rad, 30);

	glPopMatrix();
}

void draw() {
	glColor3f(0,1,0);
    drawSquare(square_len);

	glColor3f(1,0,0);
	drawCircle(circle_rad, 60);

	for (int i = 0; i < drawn_circles; i++) drawBubble(positions[i]);
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'p':
			play = !play;
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			if(speed > 0.001) {
				speed -= 0.001;
			}
			break;
		case GLUT_KEY_UP:		// up arrow key
			if(speed < 0.1) {
				speed += 0.001;
			}
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	draw();
    
    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
	//drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	angle+=0.05;

	if (drawn_circles != total_circles) {
		if(clock() - current_time > 500) {
			current_time = clock();
			drawn_circles++;
		}
	}

	if(play) {
		updatePositions();
		updateCircleStatus();
		boundaryCheckSqaure();
		boundaryCheckCircle();
		circleCollisionDetect();
	}

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	square_len = 120;
	circle_rad = 90;
	bubble_rad = 15;

	total_circles = 5;
	drawn_circles = 1;

	speed=0.005;
	play = true;

	current_time = clock();

	srand(time(0));

	for (int i = 0; i < total_circles; i++) 	{
		trapped[i] = 0;

		positions[i] = {-(square_len-bubble_rad-5), -(square_len-bubble_rad-5), 0};
		
		double vx = rand()%100;
		double vy = rand()%100;
		double unit = sqrt(vx*vx + vy*vy);
		vx /= unit;
		vy /= unit;
		
		velocity[i] = {vx, vy, 0};
		printf("vx, vy: %f, %f\n", vx, vy);
	}


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
	//srand(time(0));

	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
