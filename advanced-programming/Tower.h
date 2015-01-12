#ifndef TOWER_H
#define TOWER_H
#include "VisualObject.h"
class Tower : public VisualObject
{
public:
	Tower(float x, float y, float z);
	virtual ~Tower(void);
	void Draw();
	bool turretAlive;
};

#endif

