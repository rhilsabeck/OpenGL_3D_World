//Final project by Chase Smith, Jennifer Dust, and Ryan Hilsabeck. 
//First coordinates on console output are to stonehenge

#include <GL\glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include "Object.h"
#include "World.h"
World theWorld;

// color definitions
#define RED        1
#define GREEN      2
#define BLUE       3
#define CYAN       4
#define MAGENTA    5
#define YELLOW     6
#define WHITE      7
#define BLACK      8
#define BROWN	   9


//object definitions
#define BIRD 10
#define TREE1 11
#define TREE2 12
#define TREE3 13
#define ROCK 14
#define FLOOR 15
#define TEAPOT 16
#define PILLARUPRIGHT 17
#define PILLARSIDEWAYS 18


bool continuousRendering = false;
//initalize objects
hummingBird bird;
hummingBird overheadCam;

tree1 tree;
tree2 treetwo;

float angle = 0.035;
float amount = 20.00;
GLsizei wh = WINDOW_HEIGHT, ww = WINDOW_WIDTH;



void drawMoreDetail(int x)
{
	//if (theWorld.NeedsDrawing())
	{
		glutPostRedisplay();
	}	
}

//set time delay
void timerFunc(int x)
{
	drawMoreDetail(0);
	//if(x && continuousRendering)
		glutTimerFunc(100, timerFunc, 1);
}

//world lighting
void initLighting()
{
	GLfloat lightPos0[] = { 500.0, 2000.0, 500.0, 1.0 };
	GLfloat lightPos1[] = { 500.0, 400.0, 500.0, 1.0 };
	GLfloat lightPos2[] = { 500.0, 400.0, 500.0, 0.0 };
	GLfloat ambientLight0[] = { 0.2, 0.2, 0.0, 1.0 };
	GLfloat ambientLight1[] = { 0.2, 0.2,0.0, 1.0 };
	GLfloat diffuseLight3[] = { 1.0, 1.0, 0.0,1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight3);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45);

}

//initalize 
void init(void)
{ 
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

	initLighting();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	bird.Home();
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);
   theWorld.SetBird(&bird);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

}


//Used to render string output on screen
void RenderString(float x, float y, const char* modeString, int length)
{
	int shift = 0;
	glRasterPos2f(x, y);
	glColor3f(1.0, 1.0, 1.0); //draw text in white
	for (int i = 0; i < length; i++)
	{
		glRasterPos2f(x + shift, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, modeString[i]);
		shift += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, modeString[i]);
	}
}

//draw user interface: Compass, and key control list
void drawUI()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();

	//background for display
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.3f, 0.8f);
	glVertex3i(0, (WINDOW_HEIGHT / 3) * 2, 0);
	glVertex3i(WINDOW_WIDTH, (WINDOW_HEIGHT / 3) * 2, 0);
	glVertex3i(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glVertex3i(0, WINDOW_HEIGHT, 0);
	glEnd();

	//LEFT SIDE DISPLAY INFORMATION
#define WestLeft ((WINDOW_WIDTH/3)/2) - ((WINDOW_HEIGHT / 3) / 2)
#define WestRight ((WINDOW_WIDTH/3)/2) + ((WINDOW_HEIGHT / 3) / 2)
#define WestCenter  (((WINDOW_HEIGHT / 3) * 2) + (WINDOW_HEIGHT / 6))

#define NorthTop ((WINDOW_HEIGHT / 3) * 2) + ((WINDOW_HEIGHT / 3) / 2)*2 - ((WINDOW_HEIGHT / 3) / 2)/6
#define NorthBottom ((WINDOW_HEIGHT / 3) * 2) + ((WINDOW_HEIGHT / 3) / 2)/6
#define NorthCenter ((WINDOW_WIDTH / 3) / 2)

	//Compass 
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 0.0f);
	//West - East Line
	glVertex3i(WestLeft, WestCenter, 0);
	glVertex3i(WestRight, WestCenter, 0);
	//North - South Line
	glVertex3i(NorthCenter, NorthBottom, 0);
	glVertex3i(NorthCenter, NorthTop, 0);
	glEnd();


	//Create directional titles
	glColor3f(1.0f, 1.0f, 1.0f);
	//N
	glRasterPos3f(NorthCenter, NorthTop, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');

	//E
	glRasterPos3f(WestRight, WestCenter, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');


	//S
	glRasterPos3f(NorthCenter, NorthBottom, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');

	//W
	glRasterPos3f(WestLeft, WestCenter, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'W');


	//moving compass needle
	float radius = 130;
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i((WINDOW_WIDTH / 3) / 2, ((WINDOW_HEIGHT / 3) * 2) + (((WINDOW_HEIGHT / 3) * 2) / 4));
	glVertex2i((WINDOW_WIDTH / 3) / 2 - (radius*cos(bird.zAH - (3.14159f /2.0f))), ((WINDOW_HEIGHT / 3) * 2) + (((WINDOW_HEIGHT / 3) * 2) / 4) + (radius*sin(bird.zAH - (3.14159f/2.0f))));
	glEnd();



	//RIGHT SIDE DISPLAY INFORMATION
	glColor3f(1.0f, 0.0f, 0.0f);
	//RIGHT SIDE DISPLAY INFORMATION
	glColor3f(1.0f, 0.0f, 0.0f);

#define xRightSide ((WINDOW_WIDTH / 3) * 2) +((WINDOW_WIDTH / 3)/15)

	//display speed information
	string textSpeed = "Current Speed: " + std::to_string(amount);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 30, textSpeed.c_str(), 18);

	//list of current controls
	RenderString(xRightSide, WINDOW_HEIGHT - 30, "List of Controls", 16);
	// black dividing line
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3i(xRightSide, WINDOW_HEIGHT - 40, 0);
	glVertex3i(WINDOW_WIDTH, WINDOW_HEIGHT - 40, 0);
	glEnd();

	//Left List
	RenderString(xRightSide, WINDOW_HEIGHT - 60, "Move North: W", 13);
	RenderString(xRightSide, WINDOW_HEIGHT - 90, "Move South: S", 13);
	RenderString(xRightSide, WINDOW_HEIGHT - 120, "Move West: A", 12);
	RenderString(xRightSide, WINDOW_HEIGHT - 150, "Move East: D", 12);
	RenderString(xRightSide, WINDOW_HEIGHT - 180, "Move Up: Q", 10);
	RenderString(xRightSide, WINDOW_HEIGHT - 210, "Move Down: Z", 12);
	RenderString(xRightSide, WINDOW_HEIGHT - 240, "Home: H", 10);
	RenderString(xRightSide, WINDOW_HEIGHT - 270, "Quit: Esc", 10);

	//Right List
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 60, "Tilt UP: Arrow Up", 18);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 90, "Tilt Down: Arrow Down", 21);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 120, "Pan Left: Arrow Left", 21);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 150, "Pan Right: Arrow Right", 22);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 180, "Map Zoom In: [", 15);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 210, "Map Zoom Out: ]", 15);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 240, "Speed Up: +", 15);
	RenderString(xRightSide + 250, WINDOW_HEIGHT - 270, "Slow Down: -", 15);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);


}

//Mini map viewport and settings
void drawMap()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//center mini map in the UI
	glViewport((WINDOW_WIDTH / 3)/2 + ((WINDOW_WIDTH / 3) / 4), ((WINDOW_HEIGHT / 3) + 5) * 2, OVERHEAD_VIEW_WIDTH, OVERHEAD_VIEW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(OVERHEAD_VIEW_WIDTH, 0, 0, OVERHEAD_VIEW_HEIGHT);

	//call the world to draw overhead
	theWorld.DrawOverhead();
	glPopMatrix();

}

//Main display function
void display(void)
{

	glClearColor(0.2f, 0.3f, 1.0f, 0.9f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, (WINDOW_HEIGHT / 3) * 2, WINDOW_WIDTH, WINDOW_HEIGHT - (WINDOW_HEIGHT / 3) * 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)WINDOW_WIDTH, (WINDOW_HEIGHT / 3) * 2, (GLdouble)WINDOW_HEIGHT, -1.0, 1.0);

	//draw the user interface **NOT THE MINI MAP**
	drawUI();

	glPopMatrix();
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//draw the actual world, with lighting
	theWorld.Draw(continuousRendering);
	glDisable(GL_LIGHTING);
	glPopMatrix();

	//draw the mini map interface 
	drawMap();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	ww = w; wh = h;

}



//Accept keyboard input, determine how to move the "bird"   
   void keyboard(unsigned char key, int x, int y)
{
	theWorld.ResetDetailedRenderDistance();
	cout << "(" << bird.Position().x << ", " << bird.Position().y << ", " << bird.Position().z << ")" << endl;
	switch (key)
	{
	case '`': continuousRendering = !continuousRendering;
		if (continuousRendering) glutTimerFunc(50, timerFunc, 1);
		break;
	case 'q':
	case 'Q':
		bird.MoveUp(amount);
		glutPostRedisplay();
		break;
	case 'E':
	case 'e':
	case 'z':
	case 'Z':
		bird.MoveDown(amount);
		glutPostRedisplay();
		break;
	case 'H':
	case 'h':
		bird.Home();
		glutPostRedisplay();
		break;
	case 'W':
	case 'w':
		bird.MoveForward(amount);
		glutPostRedisplay();
		break;
	case 'S':
	case 's':
		bird.MoveBack(amount);
		glutPostRedisplay();
		break;
	case 'A':
	case 'a':
		bird.MoveLeft(amount);
		glutPostRedisplay();
		break;
	case 'D':
	case 'd':
		bird.MoveRight(amount);
		glutPostRedisplay();
		break;
	case '-':
		if (amount > 1.0)
			amount -= 1.0;
		glutPostRedisplay();
		break;
	case '+':
		if (amount < 20.0)
			amount += 1.0;
		glutPostRedisplay();
		break;
	case '1':
		bird.MoveForwardAngle(amount);
		glutPostRedisplay();
		break;
	case '2':
		bird.MoveBackAngle(amount);
		glutPostRedisplay();
		break;
	case '[':
		OVERHEAD_VIEW_GRID_CELLS_VISIBLE--;
		if (OVERHEAD_VIEW_GRID_CELLS_VISIBLE < 2) OVERHEAD_VIEW_GRID_CELLS_VISIBLE = 2;
		glutPostRedisplay();
		break;
	case ']':
		OVERHEAD_VIEW_GRID_CELLS_VISIBLE++;
		if (OVERHEAD_VIEW_GRID_CELLS_VISIBLE > GRID_DIVISIONS_X) OVERHEAD_VIEW_GRID_CELLS_VISIBLE = GRID_DIVISIONS_X;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//Arrow keys being used to pan/tilt camera view for bird
void specialKey(int key, int x, int y)
{
	theWorld.ResetDetailedRenderDistance();
	cout << "(" << bird.Position().x << ", " << bird.Position().y << ", " << bird.Position().z << ")" << endl;
	//cout << "amount = " << amount << endl;
	switch (key)
	{
	case GLUT_KEY_UP:
		bird.TiltUp(angle);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		bird.TiltDown(angle);
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		bird.TurnLeft(angle);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		bird.TurnRight(angle);
		glutPostRedisplay();
		break;
	}
}


void main(int argc, char** argv)
{
   srand(0);
   glutInit(&argc, argv);
   
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(ww, wh);
   glutInitWindowPosition(100,0);
   glutCreateWindow("Humming Bird World");
   glewInit();
   theWorld.Init();
   theWorld.AddObjects();
   //glutFullScreen();
   
   init();
   
   //create all object lists
   tree1::SetupDisplayList();
   tree2::SetupDisplayList();
   rock::SetupTexture();
   pillarSideways::SetupDisplayList();
   pillarUpright::SetupDisplayList();
   glutDisplayFunc(display);
//   glutIdleFunc(idleDisplayFunction);
   //glutTimerFunc(1000, timerFunc, 1);
//   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKey);
   glutMainLoop();
}