#include "scene.h"

using namespace Tracer;

Viewer::Viewer(float near, float far, float fov, float aspect)
{
	this->near = near;
	this->far  = far;

	this->_target = Vec3(0, 0, 1);

	dirtyProj = dirtyView = true;
	recalculate();
}

// view matrix
Viewer& Viewer::position(Vec3 pos)
{
	this->_pos = pos;
	return *this;
}

Viewer& Viewer::look_at(Vec3 loc)
{
	this->_target = loc;
	return *this;
}

Viewer& Viewer::up(Vec3 up)
{
	this->_up = up;
	return *this;
}

// perspective matrix
Viewer& Viewer::field_of_view(float fov)
{
	this->fov = fov;
	return *this;
}

void Viewer::recalculate()
{
	if(dirtyView)
	{
		mat4x4_look_at(this->view, _pos.v, _target.v, _up.v);
	}

	dirtyProj = dirtyView = false;
}
