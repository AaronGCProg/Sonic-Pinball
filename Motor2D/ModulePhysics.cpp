#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);


	return true;
}

// World Stepping & Collision
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}
	
	return UPDATE_CONTINUE;
}
PhysBody* ModulePhysics::CreateFlipper(int x, int y, flipper_direction dir, b2Vec2 mesure, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex)
{
	b2Body* CircleFlipper = nullptr;;
	b2Body* rectangleFlipper = nullptr;
	b2RevoluteJointDef jointFlipperDef;
	jointFlipperDef.enableMotor = true;
	jointFlipperDef.maxMotorTorque = 7.5f;

	CircleFlipper = CreateCircle(x, y, 4, 0.0f, true, colType, REGULAR_MAP, BALL)->body;
	CircleFlipper->SetType(b2BodyType::b2_staticBody);

	if (dir == 1)
	{
		rectangleFlipper = App->physics->CreateRectangle(x + mesure.x / 2, y, mesure.x, mesure.y, false, colType, mask, cat)->body;

		rectangleFlipper->SetType(b2BodyType::b2_dynamicBody);
		jointFlipperDef.motorSpeed = -40.0f;


		jointFlipperDef.enableLimit = true;
		jointFlipperDef.lowerAngle = -25 * DEGTORAD;
		jointFlipperDef.upperAngle = 45 * DEGTORAD;
	}
	else if (dir == 2)
	{
		rectangleFlipper = App->physics->CreateRectangle(x - mesure.x / 2, y, mesure.x, mesure.y, false, colType, mask, cat)->body;

		rectangleFlipper->SetType(b2BodyType::b2_dynamicBody);
		jointFlipperDef.motorSpeed = 40.0f;

		jointFlipperDef.enableLimit = true;
		jointFlipperDef.lowerAngle = -45 * DEGTORAD;
		jointFlipperDef.upperAngle = 25 * DEGTORAD;
	}

	jointFlipperDef.Initialize(rectangleFlipper, CircleFlipper, CircleFlipper->GetWorldCenter());

	b2RevoluteJoint* joint = nullptr;
	joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&jointFlipperDef);

	flipperJoints.add(joint);

	PhysBody* newBody = new PhysBody();;
	newBody->body = rectangleFlipper;
	newBody->width = mesure.x;
	newBody->height = mesure.y;
	rectangleFlipper->SetUserData(newBody);

	return newBody;
}


b2PrismaticJoint* ModulePhysics::CreateBallShooter(int x, int y,int w, int h, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex)
{
	int launcherRect[8] = {
	x, y,
	x+w, y,
	x+w, y-h,
	x, y-h
	};

	b2Body* ballLauncher = App->physics->CreateChain(0, 0, launcherRect,8,false, colType, mask, cat)->body;

	b2PrismaticJointDef jointBallShDef;
	jointBallShDef.Initialize(ground, ballLauncher, ground->GetWorldCenter(), { 0.0f, 1.0f });

	jointBallShDef.enableMotor = true;
	jointBallShDef.maxMotorForce = 200.0f;

	jointBallShDef.enableLimit = true;
	jointBallShDef.lowerTranslation = -0.1f;
	jointBallShDef.upperTranslation = 0.5f;

	b2PrismaticJoint* BSHJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&jointBallShDef);


	return BSHJoint;
}


PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float bouncing, bool staticObject, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex)
{
	b2BodyDef body;

	if (!staticObject)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = bouncing;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = cat;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	pbody->colType = colType;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, bool staticObject, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex)
{
	b2BodyDef body;

	if (!staticObject)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = cat;


	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;
	pbody->colType = colType;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex, subgroup subgr)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = cat;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;
	pbody->colType = colType;
	pbody->subG = subgr;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool staticObject, COLLIDER_TYPE colType, uint16 mask, uint16 cat, int groupIndex)
{
	b2BodyDef body;

	if (!staticObject)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.maskBits = mask;
	fixture.filter.categoryBits = cat;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	pbody->colType = colType;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(!debug)
		return UPDATE_CONTINUE;

	b2Vec2 mouse_position;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// test if the current body contains mouse position

			

			mouse_position.x = PIXEL_TO_METERS(App->input->GetMouseX());
			mouse_position.y = PIXEL_TO_METERS(App->input->GetMouseY());


			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (f->TestPoint(mouse_position) && f->GetType() == b2Shape::e_circle)
				{
					b2MouseJointDef def;
					def.bodyA = ground;
					def.bodyB = b;
					def.target = mouse_position;
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * b->GetMass();
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
					
					mouseJointBody = b;
				}
			}
		}
	}
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr && mouseJointBody != nullptr)
		{
			mouse_joint->SetTarget(mouse_position);
			App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorA().x), METERS_TO_PIXELS(mouse_joint->GetAnchorA().y), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 0, 0);
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr && mouseJointBody != nullptr)
		{
			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
			mouseJointBody = nullptr;
		}
		checkMouseJoint();


	return UPDATE_CONTINUE;
}

void ModulePhysics::checkMouseJoint()
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE && mouse_joint != nullptr && mouseJointBody != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		mouseJointBody = nullptr;

	}

	if (!debug && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr && mouseJointBody != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		mouseJointBody = nullptr;
	}
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}