#pragma once
#include "Object.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
//#include <GL/GL.h>
#include <GL\glew.h>
//#include <GL\glew.h>


#include "ppm.h"
#include "PerlinNoise.h"
using namespace std;


class World
{

private:
	//GLuint floorbuffer[2] = { 500,1000 };
	GLuint floorbuffer[2];
	
	

	

	hummingBird* bird;
	static const int lower_detailed_render_grid_distance = MAX_CAMERA_GRID_DISTANCE - 20;
	static const int lower_detailed_render_grid_distance_squared = lower_detailed_render_grid_distance * lower_detailed_render_grid_distance;
	int detailed_render_grid_distance; //= lower_detailed_render_grid_distance;
	int detailed_render_grid_distance_squared; //= detailed_render_grid_distance * detailed_render_grid_distance;

	int render_grid_distance;// = MAX_CAMERA_GRID_DISTANCE;


	struct coord2
	{
		int x,z;
		coord2(int ix, int iz)
		{
			x=ix;
			z=iz;
		}
	};
	vector<Object*> object_grid[GRID_DIVISIONS_Z][GRID_DIVISIONS_X];
	coord2 containing_grid(float x, float z)
	{
		//cout << "x is " << x << " AND z is " << z << endl;
		//cout << " X / GRID_SIZE_X  is " << x/GRID_SIZE_X << endl;
		coord2 co(x/(float)GRID_SIZE_X, z/(float)GRID_SIZE_Z);
		if(co.x>=GRID_DIVISIONS_X) co.x=GRID_DIVISIONS_X-1;
		if(co.z>=GRID_DIVISIONS_Z) co.z=GRID_DIVISIONS_Z-1;
		//cout << "CO x is " << co.x << " AND CO z is " << co.z << endl;
		return co;
	}
	inline float object_height(float x, float z)
	{
		return GetPHeight(x,z);
	}
	
	float distance(coord2 c1, coord2 c2)
	{
		//    c2 -= c1;
		c2.z-=c1.z;
		c2.x-=c1.x;
		return sqrtf((float)((c2.x * c2.x) + (c2.z * c2.z)));
	}
	int squared_distance(coord2 c1, coord2 c2)
	{
		c2.z -= c1.z;
		c2.x -= c1.x;
		return ((c2.x*c2.x) + (c2.z*c2.z));

	}

	int get_angle_of_cell(coord2 cell, coord2 birdcenter)
	{
		cell.x -= birdcenter.x;
		cell.z -= birdcenter.z;
		float rads = atan2(cell.z, cell.x);
		if (rads < 0) rads += 2 * 3.1415f;
		rads *= (180.0f / 3.1415f);
		return (int)rads;

	}

	void get_drawable_grid_cells(vector<coord2>& points)
	{
		Coord3 bpos = bird->Position();
		coord2 bcen = containing_grid(bpos.x,bpos.z);
		coord2 target = coord2(bird->zTarget.x, bird->zTarget.z);

		int ang = (int)((bird->zAH<0?bird->zAH+(2.0f*3.1415f):bird->zAH) * (180.0f/3.1415f));

		ang -= 90;
		ang += 360;
		ang = ang % 360;
		ang += 360;
		

		bool isBirdFacingGround = ((bird->zAV<-0.5 &&bird->zAV>-3.1415f));
		const int max_camera_grid_distance_squared = render_grid_distance * render_grid_distance;
		//To start, just a circle
		for(int z=0;z<GRID_DIVISIONS_Z;z++)
		{
			for(int x=0;x<GRID_DIVISIONS_X;x++)
			{
				int tempdist = (squared_distance(bcen, coord2(x, z)));
				if(tempdist<=max_camera_grid_distance_squared)
				{
					int gan = get_angle_of_cell(coord2(x, z), bcen);
					//gan += 360;
					gan += 270;
					gan %= 360;
					
					//DO NOT TOUCH
					
					if(((abs(gan - ang)%360)<=170)||(isBirdFacingGround&&(tempdist<=lower_detailed_render_grid_distance_squared)))
					{
						points.push_back(coord2(x, z));
					}
				}
			}
		}
	}

	void draw_overhead_map()
	{
		//Need to set up projection for the overhead map
		coord2 bcen = containing_grid(bird->Position().x, bird->Position().z);
		int bmidx = bcen.x;
		if (bmidx < OVERHEAD_VIEW_GRID_CELLS_VISIBLE/2) bmidx = OVERHEAD_VIEW_GRID_CELLS_VISIBLE/2;
		int bmidz = bcen.z;
		if (bmidz < OVERHEAD_VIEW_GRID_CELLS_VISIBLE/2) bmidz = OVERHEAD_VIEW_GRID_CELLS_VISIBLE/2;
		if (bmidx > GRID_DIVISIONS_X - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2)) bmidx = GRID_DIVISIONS_X - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2);
		if (bmidz > GRID_DIVISIONS_Z - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2)) bmidz = GRID_DIVISIONS_Z - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2);
		int xlen = OVERHEAD_VIEW_HEIGHT / OVERHEAD_VIEW_GRID_CELLS_VISIBLE, zlen = OVERHEAD_VIEW_HEIGHT / OVERHEAD_VIEW_GRID_CELLS_VISIBLE;
		int bz=0, bx=0;

		for (int iz=0, z = bmidz - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); z < bmidz + (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2);iz++,z++)
		{
			for (int ix=0,x = bmidx - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); x < bmidx + (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); ix++,x++)
			{
				if (z == bcen.z && x==bcen.x)
				{
					bz = iz;
					bx = ix;
				}
				{
					object_grid[z][x][0]->DrawOverhead(ix*xlen,iz*zlen,xlen,zlen);
				}
			}
		}
		if (bz == 0 && bx == 0)
		{
			cout << "bcenx: " << bcen.x << ", bcenz: " << bcen.z << endl;
		}

		for (int iz = 0, z = bmidz - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); z < bmidz + (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); iz++, z++)
		{
			for (int ix = 0, x = bmidx - (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); x < bmidx + (OVERHEAD_VIEW_GRID_CELLS_VISIBLE / 2); ix++, x++)
			{
				for (int ind = 1; ind<object_grid[z][x].size(); ind++)
				{
					object_grid[z][x][ind]->DrawOverhead(ix*xlen, iz*zlen, xlen, zlen);
				}
			}
		}
		bird->DrawOverhead(bx*xlen,bz*zlen, xlen, zlen);
	}

	void draw_objects()
	{
		bird->SetupProjection(render_grid_distance * GRID_SIZE_X);
		bird->Look();
		vector<coord2> drawableGridCells;
		get_drawable_grid_cells(drawableGridCells);

		
		coord2 bcen = containing_grid(bird->Position().x, bird->Position().z);
		for(int i=0;i<drawableGridCells.size();i++) //Go through every drawable cell
		{
			for(int ind = 0;ind<object_grid[drawableGridCells[i].z][drawableGridCells[i].x].size();ind++) //Draw every object in the cell
			{
				object_grid[drawableGridCells[i].z][drawableGridCells[i].x][ind]->Draw();
				if (squared_distance(bcen, drawableGridCells[i]) >= detailed_render_grid_distance_squared) break;
			}
			
		}

	}
	
	void draw_UI()
	{

	}

	


	
public:
	void ResetDetailedRenderDistance()
	{
		detailed_render_grid_distance = lower_detailed_render_grid_distance;
		detailed_render_grid_distance_squared = detailed_render_grid_distance * detailed_render_grid_distance;
		render_grid_distance = MAX_CAMERA_GRID_DISTANCE;
	}


	double GetPRandValue(float x, float z, int type)
	{
		//cout << pn.noise(x / 200.0f, z / 200.0f, type) << endl;;
		return pn.noise(x/200.0f , z/200.0f , type);
	}


	double GetPHeight(float x, float z)
	{
		//cout << pn.noise(x/10.0,z/10.0,0) << endl;;
		return 255*pn.noise(x/200.0f,z/200.0f,0);
	}




	void DoFloor()
	{
		
		for(int iz=0;iz<GRID_DIVISIONS_Z;iz++)
		{
			for(int ix=0;ix<GRID_DIVISIONS_X;ix++)
			{
				int z = iz*GRID_SIZE_Z;
				int x = ix*GRID_SIZE_X;
				Floor* fl = new Floor();
				fl->Set_LLC(Coord3(x,GetPHeight(x,z),z));
				fl->Set_ULC(Coord3(x,GetPHeight(x,z+GRID_SIZE_Z),z+GRID_SIZE_Z));
				fl->Set_LRC(Coord3(x+GRID_SIZE_X,GetPHeight(x+GRID_SIZE_X,z),z));
				fl->Set_URC(Coord3(x+GRID_SIZE_X,GetPHeight(x+GRID_SIZE_X,z+GRID_SIZE_Z),z+GRID_SIZE_Z));
				fl->Set_Middle(Coord3(x+(GRID_SIZE_X/2),GetPHeight(x+(GRID_SIZE_X/2),z+(GRID_SIZE_Z/2)),z+(GRID_SIZE_Z/2)));
				fl->SetColors();
				object_grid[iz][ix].push_back(fl);
				
				if(GetPRandValue(x,z,2*TREE1)<=.3)
				{
					if(GetPHeight(x+(GRID_SIZE_X/2),z+(GRID_SIZE_Z/2))>=80) //not below water level
					AddObject(new tree1(),x+(rand()%GRID_SIZE_X),z+(rand()%GRID_SIZE_Z));
					
				}
				else if(GetPRandValue(x,z,2*TREE2)<=.3)
				{
					if (GetPHeight(x + (GRID_SIZE_X / 2), z + (GRID_SIZE_Z / 2)) >= 80) //not below water level
						AddObject(new tree2(), x + (rand() % GRID_SIZE_X), z + (rand() % GRID_SIZE_Z));
					
				}
				else if (GetPRandValue(x, z, 2 * FLOWER) <= .3)
				{
					if (GetPHeight(x + (GRID_SIZE_X / 2), z + (GRID_SIZE_Z / 2)) >= 80) //not below water level
						AddObject(new flower(), x + (rand() % GRID_SIZE_X), z + (rand() % GRID_SIZE_Z));
				}
			}
		}
	}



	void SetBird(hummingBird* b)
	{
		bird = b;
	}
	PerlinNoise pn;
	void AddRocksToGridCell(int x, int z, int numRocks)
	{
		if (GetPHeight(x + (GRID_SIZE_X / 2), z + (GRID_SIZE_Z / 2)) < 80) //not below water level
			return;
		while (numRocks--)
		{
			
			AddObject(new rock(),x+(rand()%GRID_SIZE_X),z+(rand()%GRID_SIZE_Z));
			
		}
	}

	//drawing the mini map
	void DrawOverhead()
	{
		draw_overhead_map();
	}
	void AddObjects()
	{
		//Starting with rocks
		int numRockPatches = (rand() % 450) + 150;
		while (--numRockPatches)
		{
			int gridCellX = rand() % GRID_DIVISIONS_X, gridCellZ = rand() % GRID_DIVISIONS_Z;
			
			int patchSize = rand() % 8 + 1;
			for (int iz = 0; iz < patchSize; iz++)
			{
				for (int ix = 0; ix < patchSize; ix++)
				{
					if (GetPHeight(gridCellX + (GRID_SIZE_X / 2), gridCellZ + (GRID_SIZE_Z / 2)) <= 80) //not below water level
						continue;
					if (rand() % 10 < 3) continue; // 30% chance of skipping a patch
					int numRocks = rand() % 7 + 3; //Between 3 and 10 rocks
					AddRocksToGridCell((ix + gridCellX)*GRID_SIZE_X, (iz + gridCellZ)*GRID_SIZE_Z, numRocks);
				}
			}
		}
		//Now pillars
		int numPillars = (rand() % 1000) + 450;
		while (--numPillars)
		{
			int gridCellX = rand() % GRID_DIVISIONS_X, gridCellZ = rand() % GRID_DIVISIONS_Z;
			if (GetPHeight( gridCellX + (GRID_SIZE_X / 2), gridCellZ + (GRID_SIZE_Z / 2)) <= 80) //not below water level
				continue;
			AddObject(new pillarUpright(), gridCellX*GRID_SIZE_X, gridCellZ*GRID_SIZE_Z);
		}

		int gx, gz;
		
		while (true)
		{
			gx = rand() % GRID_DIVISIONS_X;
			gz = rand() % GRID_DIVISIONS_Z;
			float px = GetPHeight((gx )*GRID_SIZE_X, (gz )*GRID_SIZE_Z);
			if (px>150&& gx>5 && gx<GRID_DIVISIONS_X-5 && gz>5 && gz < GRID_DIVISIONS_Z-5)
			{
				
				for (float ra = 0.0f; ra < 2.0*3.1415f; ra += 3.1415f / 5.0f)
				{
					if (abs(px - GetPHeight((gx*GRID_SIZE_X) + (20 * cos(ra)), (gz*GRID_SIZE_Z) + (20 * sin(ra))))>5.0f)
					{
						px = 0; break;
					}
					
				}
				if (px == 0) continue;
				cout << "X: " << gx*GRID_SIZE_X << ", Z: " << gz*GRID_SIZE_Z << endl;
				for (float ra = 0.0f; ra < 2.0*3.1415f; ra += 3.1415f / 5.0f)
				{
					RemoveAllButFloorAndPillarsAndAddObject(new pillarUpright(), (gx*GRID_SIZE_X) + (20 * cos(ra)), (gz*GRID_SIZE_Z) + (20 * sin(ra)));
				}
				AddObject(new teapot(), (gx)*GRID_SIZE_X, (gz )*GRID_SIZE_Z);
				break;
			}

		}

	}

	void RemoveAllButFloorAndPillarsAndAddObject(Object* obj, float x, float z)
	{
		coord2 co = containing_grid(x, z);
		for (int i = 1; i < object_grid[co.z][co.x].size(); i++)
		{
			if(object_grid[co.z][co.x][i]->get_type()!=PILLARUPRIGHT)
				object_grid[co.z][co.x].erase(object_grid[co.z][co.x].begin() + i);
		}
		AddObject(obj, x, z);
	}


	void Init()
	{
		
		DoFloor();
	
	}
	//generate perlinNoise floor grids
	World()
	{
		unsigned int seed = 238;
		pn = PerlinNoise(seed);
		for(unsigned int z=0;z<GRID_DIVISIONS_Z;z++)
		{
			for(unsigned int x=0;x<GRID_DIVISIONS_X;x++)
			{
				object_grid[z][x] =  vector<Object*>();
				
			}
		}

	}


	void AddObject(Object* obj, float x, float z)
	{
		obj->setPosition(Coord3(x,object_height(x,z),z));
		coord2 co = containing_grid(x,z);
		object_grid[co.z][co.x].push_back(obj);

	}
	void HandleKey(unsigned char ch)
	{

	}

	//determine if bird is contacting the ground
	void BirdCollisionMovementAgainstFloor()
	{
		Coord3 bpos = bird->Position();
		if (bpos.y < GetPHeight(bpos.x, bpos.z))
		{
			bpos.y = GetPHeight(bpos.x, bpos.z) + .01;
		}
		if (bpos.x < 0)
			bpos.x = 0;
		if (bpos.z < 0)
			bpos.z = 0;
		if (bpos.y > WORLD_MAX_Y)
			bpos.y = WORLD_MAX_Y;
		if (bpos.z > WORLD_MAX_Z)
			bpos.z = WORLD_MAX_Z;
		if (bpos.x > WORLD_MAX_X)
			bpos.x = WORLD_MAX_X;
		
		bird->SetPosition(bpos.x,bpos.y,bpos.z);

	}
	bool NeedsDrawing()
	{
		return detailed_render_grid_distance != MAX_CAMERA_GRID_DISTANCE;
	}
	void Draw(bool inc)
	{
		GLint i;
		glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &i);
		//cout << "MaxSIZE: " << i << endl;
		glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &i);
		//cout << "CurrentSize: " << i << endl;
		//if (detailed_render_grid_distance == MAX_CAMERA_GRID_DISTANCE) return;
		detailed_render_grid_distance_squared = detailed_render_grid_distance * detailed_render_grid_distance;

		//Collision detection!
		BirdCollisionMovementAgainstFloor();

		glEnable(GL_FOG);


		glFogi(GL_FOG_START, lower_detailed_render_grid_distance*GRID_SIZE_X);
		glFogi(GL_FOG_END, render_grid_distance*GRID_SIZE_X);
		glFogi(GL_FOG_MODE, GL_LINEAR);


		//Draw the objects
		draw_objects();
		glDisable(GL_FOG);

		//determine if need to continue to render more of the world
		if(!inc) return;
		if(detailed_render_grid_distance < MAX_CAMERA_GRID_DISTANCE) detailed_render_grid_distance+=1;
		if (render_grid_distance < 100) render_grid_distance+=1;
	}


};
