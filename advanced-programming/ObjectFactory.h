#ifndef OBJECT FACTORY_H
#define OBJECT_FACTORY_H
#include <vector>
#include "openGL\glut.h"
#include "GameObject.h"
#include "VisualObject.h"
#include "Tower.h"
#include "Terrain.h"

// #include "TieFighter.h"

class ObjectFactory
{
private:
	GameObject *objects[25];
	int objectSize;
	Terrain terrain;
public:
	ObjectFactory() 
	{
		objectSize = 0;
	}
	// add an object to the list
	void AddObject(GameObject *_newObject)
	{
		if (objectSize == -1)
			objectSize = 0;
		objects[objectSize] = _newObject;
		objectSize++;
	}
	// draw all objects
	void DrawTerrain(int stage)
	{
		if (stage == 1)
			terrain.DrawSurface();
		else if (stage == 2)
			terrain.DrawTrench();
	}
	void DrawAll()
	{		
		for (int i = 0; i < objectSize; i++)
		{
			if (objects[i] != nullptr && objects[i]->GetScale() > 0)
				objects[i]->Draw();
		}
	}
	//  update positions of objects, destroy if too close to screen
	void Update()
	{
		terrain.IncreaseScale();
		for (int i = 0; i < objectSize; i++)
		{
			if (objects[i] != nullptr)
			{
				objects[i]->IncreaseScale();
				if (objects[i]->GetScale() > 35)
				{
					DestroyAndRepack(i);
				}
			}
		}
	}
	// destroy target object and repack array
	void DestroyAndRepack(int _index)
	{
		GameObject *backup[25];
		for (int i = _index; i < objectSize; i++)
		{
			backup[i] = objects[i];
		}
		for (int i = _index; i < objectSize; i++)
		{
			if (backup[i + 1] == nullptr)
				break;
			else
				objects[i] = backup[i + 1];
		}
		objects[objectSize] = nullptr;
		objectSize--;
	}
	// load objects for fighter stage
	void SetupFighterStage()
	{
		AddObject(new Tower(25, 0, 1));
	}
	// load objects for surface stage
	void SetupSurfaceStage()
	{
		AddObject(new Tower(25, 0, 1));
		AddObject(new Tower(40, 0, 10));
		AddObject(new Tower(60, 0, 3));
		AddObject(new Tower(80, 0, 6));
		AddObject(new Tower(180, 0, 3));
		AddObject(new Tower(230, 0, 5));
		AddObject(new Tower(290, 0, 8));
		AddObject(new Tower(110, 0, 5));

		AddObject(new Tower(7, 0, -5));
		AddObject(new Tower(134, 0, -2));
		AddObject(new Tower(48, 0, -13));
		AddObject(new Tower(174, 0, -10));
		AddObject(new Tower(109, 0, -7));
		AddObject(new Tower(150, 0, -5));
		AddObject(new Tower(220, 0, -8));
		AddObject(new Tower(270, 0, -5));
	}
	// load objects for trench stage
	void SetupTrenchStage()
	{
		AddObject(new Tower(25, 0, 1));
	}
	virtual ~ObjectFactory(void) {}

};

#endif
