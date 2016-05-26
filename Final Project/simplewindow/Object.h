#pragma once

#include <GL\glew.h>
#include <windows.h>
#include <iostream>
#include <windowsx.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include "texture.h"

using namespace std;
const GLfloat Color[10][4] =
{ 0.13f,0.55f,0.13f,1.0f, //dark green
1.0f,0.0f,0.0f,1.0f, // red
0.0f,1.0f,0.0f,1.0f, // green
0.0f,0.0f,1.0f,1.0f, // blue
1.0f,1.0f,0.0f,1.0f, // cyan
1.0f,0.0f,1.0f,1.0f, // magenta
1.0f,1.0f,0.0f,1.0f, // yellow
1.0f,1.0f,1.0f,1.0f, // white
0.0f,0.0f,0.0f,1.0f,  // black
0.6f,0.4f,0.12f,1.0f  //brown  

};


// color definitions
#define DARK_GREEN      0
#define RED        1
#define GREEN      2
#define BLUE       3
#define CYAN       4
#define MAGENTA    5
#define YELLOW     6
#define WHITE      7
#define BLACK      8
#define BROWN	   9

const GLubyte HeightMapColors[26][4] =
{
	0,0,102,255, //dark blue
	0,0,153,255,
	0,0,204,255,
	0,0,204,255,
	0,0,204,255,
	0,0,255,255,
	51,51,255,255,
	255,255,102,255, //BEACH
	192,210,192,255,
	102,255,102,255,
	0,204,0,255,
	0,255,0,255,
	0,250,0,255,
	0,240,0,255,
	0,230,0,255,
	0,210,0,255,
	0,170,0,255,
	150,150,150,255,
	0,255,0,255,
	0,100,0,255,
	0,255,0,255,
	222,222,222,255,
	255,255,255,255,
	255,255,255,255,
	255,255,255,255,
	255,255,255,255


};


//define pi values for calculations
#define GE_PI		3.1415926535897932384626433832795
#define GE_PI_2		1.5707963267948966192313216916395
#define GE_2PI		6.283185307179586476925286766558

//define objects
#define BIRD 10
#define TREE1 11
#define TREE2 12
#define TREE3 13
#define ROCK 14
#define FLOOR 15
#define TEAPOT 16
#define PILLARUPRIGHT 17
#define PILLARSIDEWAYS 18
#define FLOWER 19

//define world constants
#define POINTS_IN_FLOOR 6
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900
#define WORLD_MAX_X 5000
#define WORLD_MAX_Y 500
#define WORLD_MAX_Z WORLD_MAX_X
#define GRID_DIVISIONS_X 250
#define GRID_DIVISIONS_Z 250
#define MAX_CAMERA_DISTANCE 800

#define GRID_SIZE_X (WORLD_MAX_X / GRID_DIVISIONS_X)
#define GRID_SIZE_Z (WORLD_MAX_Z / GRID_DIVISIONS_Z)
//#define GRID_SIZE_X 10
//#define GRID_SIZE_Z 10
#define MAX_CAMERA_GRID_DISTANCE (MAX_CAMERA_DISTANCE / GRID_SIZE_X)

#define OVERHEAD_VIEW_WIDTH (WINDOW_WIDTH/3)
#define OVERHEAD_VIEW_HEIGHT (WINDOW_HEIGHT/3 -5)
extern unsigned int OVERHEAD_VIEW_GRID_CELLS_VISIBLE;




struct Coord3 {
	float x;
	float y;
	float z;
	Coord3(float ix, float iy, float iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}
	Coord3() {};
	inline void DrawVertex()
	{
		glVertex3f(x, y, z);
	}
};

class Object
{
protected:
	int type;
	Coord3 position;

public:
	Object(int otype) { type = otype; }
	void setPosition(Coord3 pos);
	void setY(float y)
	{
		position.y = y;
	}
	void Draw();
	void DrawOverhead(int x,int z,int xlen,int zlen);
	int collision_detection(Coord3 cam_center, float cam_radius);
	int get_type() {
		return type;
	}
};



class hummingBird : public Object {

public:
	hummingBird() :Object(BIRD) {}
	~hummingBird();

	void		SetupProjection(int);
	void		Center(void);
	void		Look(void);
	void		AboveLook(void);
	void		MoveBack(float amount);
	void		MoveBackAngle(float amount);
	void		MoveDown(float amount);
	void		MoveForward(float amount);
	void		MoveForwardAngle(float amount);
	void		MoveLeft(float amount);
	void		MoveRight(float amount);
	void		Home();
	void		MoveUp(float amount);
	Coord3		Position(void);
	void		SetTarget(float x, float y, float z);
	void		SetPosition(float x, float y, float z);
	void		SetUp(float x, float y, float z);
	void		TiltDown(float amount);
	void		TiltUp(float amount);
	void		TurnLeft(float amount);
	void		TurnRight(float amount);
	Coord3		zPosition;
	Coord3		zTarget;
	Coord3		zUp;
	float		zAV;
	float		zAH;
	void DrawOverhead(int x, int z, int xlen, int zlen);
private:
	void		zMoveLR(float amount);
	void		zMoveFB(float amount);
	void		zMoveFBAngle(float amount);
	void		zTilt(float amount);
	void		zTurn(float amount);
	void		zMoveUD(float amount);
	void		zResetRelativeVectors();

	


};

//set standard tree 1 values
extern GLuint displayList_tree1,displayList_tree2;
extern GLuint displayList_pillarUpright, displayList_pillarSideways;
class tree1 : public Object {
protected:
	static const int trunkHeight =6;
	static const int trunkSize =2;
	static const int treeSize =14;
	static const int tRadius =5;
	static const int bRadius =0 ;

	

public:
	
	static void tree1::SetupDisplayList()
	{
		//texture mapping
		int pTex2 = TextureLoadBitmap("wool_colored_green.bmp");
		int pTex = TextureLoadBitmap("leaves_spruce_opaque.bmp");
		displayList_tree1 = glGenLists(1);
		glNewList(displayList_tree1, GL_COMPILE);
		
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3fv(Color[BROWN]);
		GLUquadricObj *qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		glTranslatef(0, -1, 0);
		glRotatef(270, 1.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluQuadricTexture(qobj, true);
		
		glBindTexture(GL_TEXTURE_2D, pTex2);
		gluCylinder(qobj, trunkSize, trunkSize, trunkHeight, 20, 2);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj);
		glDisable(GL_TEXTURE_2D);



		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3fv(Color[DARK_GREEN]);
		GLUquadricObj *qobj2 = gluNewQuadric();
		gluQuadricDrawStyle(qobj2, GLU_FILL);
		gluQuadricNormals(qobj2, GLU_SMOOTH);
		glTranslatef(0, 0 + (4 * trunkSize / 2), 0);
		glRotatef(270, 1.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		gluQuadricTexture(qobj2, true);
		
		glBindTexture(GL_TEXTURE_2D, pTex);
		gluCylinder(qobj2, tRadius, bRadius, treeSize, 20, 2);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj2);
		glDisable(GL_TEXTURE_2D);


		glEndList();
	}

	tree1() :Object(TREE1) {  }
	void Draw();
	void DrawOverhead(int x, int z, int xlen, int zlen);
};

//set values for tree 2
class tree2 : public Object {
protected:
	static const int trunkRadius = 1;;
	static const int topRadius = 5;
	static const int trunkHeight = 12;
public:
	static void SetupDisplayList()
	{
		//texture mapping tree 2
		int pTex = TextureLoadBitmap("wool_colored_green.bmp");
		displayList_tree2 = glGenLists(1);
		glNewList(displayList_tree2, GL_COMPILE);

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3fv(Color[BROWN]);
		GLUquadricObj *qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		glTranslatef(0, 0, 0);
		glRotatef(270, 1.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluQuadricTexture(qobj, true);
		
		glBindTexture(GL_TEXTURE_2D, pTex);
		gluCylinder(qobj, trunkRadius, trunkRadius, trunkHeight, 20, 2);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj);
		glDisable(GL_TEXTURE_2D);


		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3fv(Color[DARK_GREEN]);
		GLUquadricObj *qobj2 = gluNewQuadric();
		gluQuadricDrawStyle(qobj2, GLU_FILL);
		gluQuadricNormals(qobj2, GLU_SMOOTH);
		glTranslatef(0, 0 + (trunkHeight), 0);
		//glRotatef(270,1.0f,0.0f,0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		gluQuadricTexture(qobj2, true);
		int pTex2 = TextureLoadBitmap("wool_colored_green.bmp");
		glBindTexture(GL_TEXTURE_2D, pTex2);
		gluSphere(qobj2, topRadius, 20, 20);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj2);
		glDisable(GL_TEXTURE_2D);

		
		glEndList();


	}
	tree2() :Object(TREE2) {  }
	void Draw();
	void DrawOverhead(int x, int z, int xlen, int zlen);
};


//set color based off of height for flooring 
class Floor : public Object {
private:
	
	Coord3 ulc, llc, urc, lrc, middle;
	int cl_ulc, cl_llc, cl_urc, cl_lrc, cl_middle;

	inline void setv(Coord3 c, int cl, int index)
	{//GL_C4UB_V3F
		verts[index].a = HeightMapColors[cl][3];
		verts[index].b = HeightMapColors[cl][2];
		verts[index].g = HeightMapColors[cl][1];
		verts[index].r = HeightMapColors[cl][0];
		verts[index].x = c.x;
		verts[index].y = c.y;
		verts[index].z = c.z;
	}
public:
	struct vert
	{//GL_C4UB_V3F
		GLubyte r, g, b, a;
		float x, y, z;

	};
	void SetY(float y)
	{
		ulc.y = y;
		urc.y = y;
		lrc.y = y;
		llc.y = y;
		middle.y = y;
		SetColors();
	}
	vert verts[POINTS_IN_FLOOR];
	void SetColors()
	{
		
		cl_ulc = ulc.y / 10;
		cl_urc = urc.y / 10;
		cl_llc = llc.y / 10;
		cl_lrc = lrc.y / 10;
		cl_middle = middle.y / 10;

		if (POINTS_IN_FLOOR == 12)
		{
			setv(ulc, cl_ulc, 0);
			setv(middle, cl_middle, 1);
			setv(urc, cl_urc, 2);


			setv(urc, cl_urc, 3);
			setv(middle, cl_middle, 4);
			setv(lrc, cl_lrc, 5);

			setv(lrc, cl_lrc, 6);
			setv(middle, cl_middle, 7);
			setv(llc, cl_llc, 8);

			setv(llc, cl_llc, 9);
			setv(middle, cl_middle, 10);
			setv(ulc, cl_ulc, 11);
		}
		else if(POINTS_IN_FLOOR==6)
		{
			setv(middle, cl_middle, 0);
			setv(urc, cl_urc, 1);
			setv(ulc, cl_ulc, 2);
			setv(llc, cl_llc, 3);
			setv(lrc, cl_lrc, 4);
			setv(urc, cl_urc,5);
		}
	}

	Floor() :Object(FLOOR) {};
	void Set_ULC(Coord3 x) { ulc = x; }
	void Set_URC(Coord3 x) { urc = x; }
	void Set_LLC(Coord3 x) { llc = x; }
	void Set_LRC(Coord3 x) { lrc = x; }
	void Set_Middle(Coord3 x) { middle = x; }
	void DrawLines()
	{

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glBegin(GL_LINES);
		
		glColor4ubv(HeightMapColors[cl_ulc]);
		ulc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_urc]);
		urc.DrawVertex();


		glColor4ubv(HeightMapColors[cl_ulc]);
		ulc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_llc]);
		llc.DrawVertex();

		glColor4ubv(HeightMapColors[cl_urc]);
		urc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_lrc]);
		lrc.DrawVertex();

		glColor4ubv(HeightMapColors[cl_llc]);
		llc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_lrc]);
		lrc.DrawVertex();

		glColor4ubv(HeightMapColors[cl_llc]);
		llc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_middle]);
		middle.DrawVertex();


		glColor4ubv(HeightMapColors[cl_ulc]);
		ulc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_middle]);
		middle.DrawVertex();

		glColor4ubv(HeightMapColors[cl_urc]);
		urc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_middle]);
		middle.DrawVertex();

		glColor4ubv(HeightMapColors[cl_lrc]);
		lrc.DrawVertex();
		glColor4ubv(HeightMapColors[cl_middle]);
		middle.DrawVertex();

		glEnd();

		glPopMatrix();
		glPopAttrib();
	}
	inline void DoColor(int i)
	{
		glColor4ubv(HeightMapColors[i]);
	}
	inline void DrawULC()
	{
		DoColor(cl_ulc);
		ulc.DrawVertex();
	}
	inline void DrawURC()
	{
		DoColor(cl_urc);
		urc.DrawVertex();
	}
	inline void DrawLRC()
	{
		DoColor(cl_lrc);
		lrc.DrawVertex();
	}
	inline void DrawLLC()
	{
		DoColor(cl_llc);
		llc.DrawVertex();
	}
	inline void DrawMiddle()
	{
		DoColor(cl_middle);
		middle.DrawVertex();
	}

	void Draw()
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glInterleavedArrays(GL_C4UB_V3F, 0, verts);

		glDrawArrays(GL_TRIANGLE_FAN, 0, POINTS_IN_FLOOR);

		glPopMatrix();
		glPopAttrib();
	}

	void Draw2()
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glBegin(GL_TRIANGLES);


		DrawULC();
		DrawURC();
		DrawLLC();

		DrawLLC();
		DrawURC();
		DrawLRC();

		glEnd();


		glPopMatrix();
		glPopAttrib();
	}

	void DrawOverhead(int x, int z, int xlen, int zlen);

};


class teapot : public Object {

protected:
	double tSize;
	int rotation;
public:
	teapot() :Object(TEAPOT) { tSize = 3; rotation = 0; }
	void Draw();

};

//randomize values for rock clusters
extern GLuint rock_texture;
class rock : public Object {
protected:
	int rSize;
	int numSlices, numStacks;
public:
	rock() :Object(ROCK) {
		rSize = (rand()%20)/10.0f+0.5f;
		numSlices = rand() % 5 + 3;
		numStacks = rand() % 4 + 2;
	}
	//texture map rocks
	static void SetupTexture()
	{
		rock_texture= TextureLoadBitmap("stone.bmp");
	}
	void Draw();

	void DrawOverhead(int x, int z, int xlen, int zlen);


};

//set values for pillar - vertical
class pillarUpright : public Object {

protected:
	static const int pilRadius = 2;
	static const int pilHeight = 12;
public:
	pillarUpright() :Object(PILLARUPRIGHT) {  }
	void Draw();
	void DrawOverhead(int x, int z, int xlen, int zlen);
	static void SetupDisplayList()
	{
		displayList_pillarUpright = glGenLists(1);
		glNewList(displayList_pillarUpright, GL_COMPILE);

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3f(0.7f, 0.7f, 0.7f);
		GLUquadricObj *qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		glTranslatef(0, 0,0);
		glRotatef(270, 1.0f, 0.0f, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		gluQuadricTexture(qobj, true);

		//texture map pillar - vertical
		int pTex = TextureLoadBitmap("cobblestone_mossy.bmp");
		glBindTexture(GL_TEXTURE_2D, pTex);
		gluCylinder(qobj, pilRadius, pilRadius, pilHeight, 4, 2);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj);
		glDisable(GL_TEXTURE_2D);
		glEndList();
	}

};

//set values for pillar - horizontal
class pillarSideways : public Object {
protected:
	static const int pilRadius = 2;
	static const int pilHeight = 12;
	
public:
	pillarSideways() :Object(PILLARSIDEWAYS) {  }
	void Draw();
	static void SetupDisplayList()
	{
		displayList_pillarSideways = glGenLists(1);
		glNewList(displayList_pillarSideways, GL_COMPILE);

		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3f(0.5f, 0.5f, 0.5f);
		GLUquadricObj *qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_FILL);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		glTranslatef(0, 0, 0);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		gluQuadricTexture(qobj, true);

		//texture map pillar - horizontal
		int pTex = TextureLoadBitmap("cobblestone_mossy.bmp");
		glBindTexture(GL_TEXTURE_2D, pTex);
		gluCylinder(qobj, pilRadius, pilRadius, pilHeight, 20, 2);
		glFlush();
		glPopMatrix();
		glPopAttrib();
		gluDeleteQuadric(qobj);
		glDisable(GL_TEXTURE_2D);

		glEndList();

	}

};

//set flower values
class flower : public Object {

protected:
	float sRadius;
	float sHeight;
	float pRadius;
	float cRadius;

public:
	flower() :Object(FLOWER) { sRadius = .15f;  sHeight = 4.0f; pRadius = .75f; cRadius = .7f; }
	void Draw();

	void DrawOverhead(int x, int z, int xlen, int zlen);

};