#pragma once
#include "Object.h"
#include "texture.h"
#include <GL\glew.h>

#include <GL/glut.h>
#define _XOPEN_SOURCE

//create variable lists 
unsigned int OVERHEAD_VIEW_GRID_CELLS_VISIBLE = 24;
GLuint displayList_tree1;
GLuint displayList_tree2,rock_texture;
GLuint displayList_pillarUpright, displayList_pillarSideways;



void Object::setPosition(Coord3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

hummingBird::~hummingBird()
{

}

void hummingBird::Center()
{
	zAH = 0;
	zAV = 0;
}

void hummingBird::Look()
{
	zResetRelativeVectors();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(zPosition.x, zPosition.y, zPosition.z, zTarget.x, zTarget.y, zTarget.z, zUp.x, zUp.y, zUp.z);
}

void hummingBird::AboveLook()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(zPosition.x, zPosition.y, zPosition.z, zTarget.x, zTarget.y, zTarget.z, zUp.x, zUp.y, zUp.z);
}

void hummingBird::MoveBack(float amount)
{
	zMoveFB(-amount);
}

void hummingBird::MoveBackAngle(float amount)
{
	zMoveFBAngle(-amount);
}

void hummingBird::MoveDown(float amount)
{
	zMoveUD(-amount);
}

void hummingBird::MoveForward(float amount)
{
	zMoveFB(amount);
}

void hummingBird::MoveForwardAngle(float amount)
{
	zMoveFBAngle(amount);
}

void hummingBird::MoveLeft(float amount)
{
	zMoveLR(-amount);
}

void hummingBird::MoveRight(float amount)
{
	zMoveLR(amount);
}


//set begining position and rest home position for bird
void hummingBird::Home()
{

	zPosition.x = 500;
	zPosition.y = 200;
	zPosition.z = 500;

	zTarget.x = 1;
	zTarget.y = 0;
	zTarget.z = 1;

	zUp.x = 0;
	zUp.y = 1;
	zUp.z = 0;

	zAH = 3.1415f;
	zAV = 0;

}

void hummingBird::MoveUp(float amount)
{
	zMoveUD(amount);
}

Coord3 hummingBird::Position()
{
	return zPosition;
}


//Setting camera views
void hummingBird::SetTarget(float x, float y, float z)
{
	zTarget.x = x;
	zTarget.y = y;
	zTarget.z = z;
}

void hummingBird::SetPosition(float x, float y, float z)
{
	zPosition.x = x;
	zPosition.y = y;
	zPosition.z = z;
}

void hummingBird::SetUp(float x, float y, float z)
{
	zUp.x = x;
	zUp.y = y;
	zUp.z = z;
}




void hummingBird::TiltDown(float amount)
{
	zTilt(-amount);
}

void hummingBird::TiltUp(float amount)
{
	zTilt(amount);
}

void hummingBird::TurnLeft(float amount)
{
	zTurn(amount);
}
void hummingBird::TurnRight(float amount)
{
	zTurn(-amount);
}


void hummingBird::zMoveFB(float amount)
{

	zPosition.z += amount;

}

void hummingBird::zMoveFBAngle(float amount)
{

	zPosition.x += (amount * sin(zAH));
	zPosition.z -= (amount * cos(zAH));

}

void hummingBird::zMoveLR(float amount)
{
	zPosition.x -= amount;
}

void hummingBird::zMoveUD(float amount)
{

	zPosition.y += amount;

}


void hummingBird::SetupProjection(int farview)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)WINDOW_WIDTH, ((GLsizei)WINDOW_HEIGHT / 3) * 2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat)WINDOW_WIDTH / ((GLfloat)WINDOW_HEIGHT / 2), 0.1, farview);

}


void hummingBird::zTilt(float amount)
{
	zAV += amount;
	if (zAV > GE_2PI)
		zAV -= GE_2PI;
	else if (zAV < -GE_2PI)
		zAV += GE_2PI;
	zResetRelativeVectors();
}
void hummingBird::zTurn(float amount)
{
	zAH -= ((zUp.y / fabs(zUp.y)) * amount);
	if (zAH > GE_2PI)
		zAH -= GE_2PI;
	else if (zAH < -GE_2PI)
		zAH += GE_2PI;
	zResetRelativeVectors();
}

void hummingBird::zResetRelativeVectors()
{

	zTarget.x = zPosition.x + sin(GE_PI_2 - zAV) * sin(zAH);
	zTarget.y = zPosition.y + cos(GE_PI_2 - zAV);
	zTarget.z = zPosition.z - sin(GE_PI_2 - zAV) * cos(zAH);


	zUp.y = cos(zAV);
	zUp.x = sin(zAV) * cos(GE_PI_2 + zAH);
	zUp.z = sin(zAV) * sin(GE_PI_2 + zAH);

}

//draw tree type 1
void tree1::Draw()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslatef(position.x, position.y, position.z);
	glCallList(displayList_tree1);
	glPopMatrix();
	glPopAttrib();

}

//draw tree type 2
void tree2::Draw()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glTranslatef(position.x, position.y, position.z);
	glCallList(displayList_tree2);

	glPopMatrix();
	glPopAttrib();
}

//draw teapot
void teapot::Draw()
{
	rotation = (rotation+15)%360;
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[YELLOW]);
	glTranslatef(position.x, position.y + tSize, position.z);
	glRotatef(rotation,0,1,0);
	glutSolidTeapot(tSize);
	glFlush();
	glPopMatrix();
	glPopAttrib();
}



//draw cluster of rocks, various sizes
void rock::Draw()
{
	
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.6f, 0.6f, 0.6f);
	GLUquadricObj *qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glTranslatef(position.x, position.y, position.z);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	gluQuadricTexture(qobj, true);
		
	glBindTexture(GL_TEXTURE_2D, rock_texture);
	gluSphere(qobj, rSize, numSlices, numStacks);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
	
}


//draw "Stonehenge" pillars, vertical 
void pillarUpright::Draw()
{

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(position.x, position.y, position.z);
	glCallList(displayList_pillarUpright);
	glPopMatrix();
	glPopAttrib();

}

//draw "Stonehenge" pillars, horizontal 
void pillarSideways::Draw()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(position.x, position.y, position.z);
	glCallList(displayList_pillarSideways);
	glPopMatrix();
	glPopAttrib();
}

//draw flower
void flower::Draw()
{
	//stem of flower
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[DARK_GREEN]);
	GLUquadricObj *qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glTranslatef(position.x, position.y, position.z);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, sRadius, sRadius, sHeight, 20, 2);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj);

	//Center of flower
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[BLACK]);
	GLUquadricObj *qobj2 = gluNewQuadric();
	gluQuadricDrawStyle(qobj2, GLU_FILL);
	gluQuadricNormals(qobj2, GLU_SMOOTH);
	glTranslatef(position.x, position.y + sHeight, position.z + (sRadius * 2));
	gluDisk(qobj2, 0, cRadius, 20, 20);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj2);

	//petals of flower
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[YELLOW]);
	GLUquadricObj *qobj3 = gluNewQuadric();
	gluQuadricDrawStyle(qobj3, GLU_FILL);
	gluQuadricNormals(qobj3, GLU_SMOOTH);
	glTranslatef(position.x, position.y + ((sHeight / 4) * 3), position.z + (sRadius + sRadius / 2));
	gluDisk(qobj3, 0, pRadius, 20, 1);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj3);

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[YELLOW]);
	GLUquadricObj *qobj4 = gluNewQuadric();
	gluQuadricDrawStyle(qobj4, GLU_FILL);
	gluQuadricNormals(qobj4, GLU_SMOOTH);
	glTranslatef(position.x + sHeight / 4, position.y + sHeight, position.z + (sRadius + sRadius / 2));
	gluDisk(qobj4, 0, pRadius, 20, 1);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj4);

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[YELLOW]);
	GLUquadricObj *qobj5 = gluNewQuadric();
	gluQuadricDrawStyle(qobj5, GLU_FILL);
	gluQuadricNormals(qobj5, GLU_SMOOTH);
	glTranslatef(position.x - sHeight / 4, position.y + sHeight, position.z + (sRadius + sRadius / 2));
	gluDisk(qobj5, 0, pRadius, 20, 1);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj5);

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3fv(Color[YELLOW]);
	GLUquadricObj *qobj6 = gluNewQuadric();
	gluQuadricDrawStyle(qobj6, GLU_FILL);
	gluQuadricNormals(qobj6, GLU_SMOOTH);
	glTranslatef(position.x, position.y + (sHeight + sHeight / 4), position.z + (sRadius + sRadius / 2));
	gluDisk(qobj6, 0, pRadius, 20, 1);
	glFlush();
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qobj6);


}

//determine which object is being drawn, and call its draw function
void Object::Draw()
{
	switch (type)
	{
	case TREE1:
		((tree1*)this)->Draw();
		break;
	case FLOOR:
		((Floor*)this)->Draw();
		break;
	case TREE2:
		((tree2*)this)->Draw();
		break;
	case TEAPOT:
		((teapot*)this)->Draw();
		break;
	case ROCK:
		((rock*)this)->Draw();
		break;
	case PILLARUPRIGHT:
		((pillarUpright*)this)->Draw();
		break;
	case PILLARSIDEWAYS:
		((pillarSideways*)this)->Draw();
		break;
	case FLOWER:
		((flower*)this)->Draw();
		break;
	}
}

//draw mini map overhead view
void hummingBird::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPointSize(2);
	glTranslated(x, z, 0);
	glRotated(((this->zAH + (3.1415f))*(180.0f / 3.1415f)), 0, 0, 1);
	glTranslated(-x, -z,0);
	
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(x + ((int)position.x%xlen) - 10, z + ((int)position.z%zlen)-4);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 13);
	glVertex2f(x + ((int)position.x%xlen) + 10, z + ((int)position.z%zlen)-4);

	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(x + ((int)position.x%xlen) - 9, z + ((int)position.z%zlen)-4);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 12);
	glVertex2f(x + ((int)position.x%xlen) + 9, z + ((int)position.z%zlen)-4);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 13);
	glVertex2f(x + ((int)position.x%xlen) - 20, z + ((int)position.z%zlen)+25);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 13);
	glVertex2f(x + ((int)position.x%xlen) + 20, z + ((int)position.z%zlen)+25);

	glEnd();
	
	glPopMatrix();
}

//draw mini map overhead view of tree 1
void tree1::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPointSize(2);
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(.6, .4, .12);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) - 5);
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(x + ((int)position.x%xlen)-4, z + ((int)position.z%zlen));
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen)+7);
	glVertex2f(x + ((int)position.x%xlen)+4, z + ((int)position.z%zlen));
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0, .8, 0);
	glVertex2f(x + ((int)position.x%xlen) - 3, z + ((int)position.z%zlen));
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 6);
	glVertex2f(x + ((int)position.x%xlen) + 3, z + ((int)position.z%zlen));
	glEnd();
	
	glPopMatrix();
}

//draw tree 2 overhead for mini map
void tree2::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPointSize(2);
	glLineWidth(3);
	glBegin(GL_LINES); //Tree Trunk
	glColor3f(.6, .4, .12);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) - 5);
	glEnd();
	glLineWidth(1);
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 4);
	glEnd();
	glPointSize(6);
	glBegin(GL_POINTS);
	glColor3f(0, .6, 0);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen) + 4);
	glEnd();
	glPopMatrix();
}

//drw flower for overhead mini map view
void flower::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen)-6);
	glEnd();
	glPointSize(4);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 0);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));
	glEnd();
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));
	glEnd();
	

	glPopMatrix();
}

//draw the ground for overhead view, keep colors 
void Floor::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_FAN);
	glColor4ubv(HeightMapColors[cl_middle]);
	glVertex2i(x + (xlen / 2), z + (zlen / 2));
	glColor4ubv(HeightMapColors[cl_llc]);
	glVertex2i(x, z);
	glColor4ubv(HeightMapColors[cl_lrc]);
	glVertex2i(x + xlen, z);
	glColor4ubv(HeightMapColors[cl_urc]);
	glVertex2i(x + xlen, z + zlen);
	glColor4ubv(HeightMapColors[cl_ulc]);
	glVertex2i(x, z + zlen);
	glColor4ubv(HeightMapColors[cl_llc]);
	glVertex2i(x, z);
	glEnd();
	glPopAttrib();
	
	glPopMatrix();
}

//draw rock clusters for overhead mini map
void rock::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPointSize(rSize+0.2);
	glBegin(GL_POINTS);
	glColor3f(.5, .5, .5);
	glVertex2f(x + ((int)position.x%xlen), z + ((int)position.z%zlen));

	
	glEnd();
	glPopMatrix();
}

// pillar vertical for overhead mini map
void pillarUpright::DrawOverhead(int x, int z, int xlen, int zlen)
{
	glPushMatrix();
	glPointSize(pilRadius + 3);
	glBegin(GL_POINTS);
	glColor3f(.3, .3, .3);
	glVertex2f(x + (((int)position.x%xlen)), z + (((int)position.z%zlen)));
	glEnd();
	glPopMatrix();
}



void Object::DrawOverhead(int x, int z, int xlen, int zlen)
{
	switch (type)
	{
	case TREE1:
		((tree1*)this)->DrawOverhead(x,z,xlen,zlen);
		break;
	case FLOOR:
		((Floor*)this)->DrawOverhead(x, z, xlen, zlen);	
		break;
	case TREE2:
		((tree2*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case TEAPOT:
		//((teapot*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case ROCK:
		((rock*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case PILLARUPRIGHT:
		((pillarUpright*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case PILLARSIDEWAYS:
		((pillarSideways*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case FLOWER:
		((flower*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	case BIRD:
		((hummingBird*)this)->DrawOverhead(x, z, xlen, zlen);
		break;
	}
}


