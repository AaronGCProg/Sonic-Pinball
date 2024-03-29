#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)
 

//Colliders Enum to manipulate the collisions
enum COLLIDER_TYPE
{
	COLLIDER_GENERAL = -1,
	COLLIDER_BALL, 
	COLLIDER_WALL,
	COLLIDER_BOUNCER,
	COLLIDER_TRIGGER,
	COLLIDER_BALLTORAIL,
	COLLIDER_RAILTOBALL,
	COLLIDER_BALLTOENTRANCE,
	COLLIDER_ENTRANCETORAIL,
	COLLIDER_BOOSTER,
	COLLIDER_DEATH,
	COLLIDER_EGG_1,
	COLLIDER_EGG_2,
	COLLIDER_LAUNCHER,
	COLLIDER_BALL_SHOOTER,
	COLLIDER_BALLTOENTRANCETORAIL,
	COLLIDER_EGG_NONE,
	COLLIDER_LOLIPOP,

	COLLIDER_MAX
};

//Enum to Control Lights in the Map
enum subgroup
{
	SUBG_NONE = 0,
	SUBG_1,
	SUBG_2,
	SUBG_3,
	SUBG_4,

	SUBGROUP_MAX
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	COLLIDER_TYPE colType = COLLIDER_GENERAL;
	subgroup subG = SUBG_NONE;
	Module* listener;

};



// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener 
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	//Physical objects creation---------------------
	PhysBody* CreateCircle(int x, int y, int radius, float bouncing, bool staticObject = false, COLLIDER_TYPE colType = COLLIDER_GENERAL, uint16 mask = 0x0016, uint16 cat = 0x0016, int groupIndex = 0);
	PhysBody* CreateRectangle(int x, int y, int width, int height, bool staticObject = false, COLLIDER_TYPE colType = COLLIDER_GENERAL, uint16 mask = 0x0016, uint16 cat = 0x0016, int groupIndex = 0);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, COLLIDER_TYPE colType = COLLIDER_GENERAL, uint16 mask = 0x0016, uint16 cat = 0x0016, int groupIndex = 0, subgroup subgr = SUBG_NONE);
	PhysBody* CreateChain(int x, int y, int* points, int size, bool staticObject = false, COLLIDER_TYPE colType = COLLIDER_GENERAL, uint16 mask = 0x0016, uint16 cat = 0x0016, int groupIndex = 0);
	PhysBody* CreateFlipper(int x, int y, flipper_direction dir, b2Vec2 mesure, COLLIDER_TYPE colType = COLLIDER_GENERAL, uint16 mask = 0x0016, uint16 cat = 0x0016, int groupIndex = 0);
	b2PrismaticJoint* CreateBallShooter(int x, int y, int w, int h, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex = 1);

	//Joints List---------------
	void checkMouseJoint();
	p2List<b2RevoluteJoint*> flipperJoints;
	b2MouseJoint* mouse_joint;


	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	bool debug = false;


private:
	//World Control---------------
	b2World* world;
	b2Body* ground;

	//Joint Control-------------------
	b2Body* mouseJointBody = nullptr;

};