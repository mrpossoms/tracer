#include "scene.h"

using namespace Tracer;

Viewer::Viewer(float near, float far, float fov, float aspect)
{
	this->near = near;
	this->far  = far;

	mat4x4_identity(_view);
	view(_view);

	dirtyProj = dirtyView = true;
	recalculate();
}

// view matrix
Viewer& Viewer::position(Vec3 pos)
{
	vec3_copy(_view[3], pos.v);
	return *this;
}

Viewer& Viewer::look_at(Vec3 loc)
{
	return *this;
}

Viewer& Viewer::up(Vec3 up)
{
	return *this;
}

// perspective matrix
Viewer& Viewer::field_of_view(float fov)
{
	this->fov = fov;
	return *this;
}

Ray3 Viewer::ray_at_coord(float u, float v)
{
	Ray3 ray;

	const Vec3 scl(-1, -1, 1);
	// u = u / tanf(u * M_PI / 4);
	// v = v / tanf(v * M_PI / 4);

	//ray.pos = Vec3(u, v, near);
	//ray.dir = ray.pos;
	//ray.dir *= scl;

	vec4 p_tmp = {}, d_tmp = {}, left_tmp, up_tmp;

	// viewer left and right for this ray
	vec3_scale(left_tmp, _left, -u);
	vec3_scale(up_tmp, _up, -v);

	vec3_scale(d_tmp, _target, near);
	vec3_add(d_tmp, d_tmp, left_tmp);
	vec3_add(d_tmp, d_tmp, up_tmp);

	vec3_norm(ray.dir.v, d_tmp);
	vec3_copy(ray.pos.v, _view[3]);

	return ray;
}

Viewer& Viewer::view(mat4x4 v)
{
	mat4x4_scale(_view, v, 1);

	mat4x4_mul_vec3(_up, _view, VEC3_UP.v);
	mat4x4_mul_vec3(_left, _view, VEC3_LEFT.v);
	mat4x4_mul_vec3(_target, _view, VEC3_FORWARD.v);
}

void Viewer::recalculate()
{
	if(dirtyView)
	{
		//mat4x4_look_at(this->view, _pos.v, _target.v, _up.v);
	}

	dirtyProj = dirtyView = false;
}
