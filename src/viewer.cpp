#include "scene.h"

using namespace Tracer;

Viewer::Viewer(float near, float far, float fov)
{
	this->near = near;
	this->far  = far;

	mat4x4_identity(_view);

	dirtyProj = dirtyView = true;
	recalculate();
}

// view matrix
Viewer& Viewer::position(Vec3 pos)
{
	_pos = pos; 
	return *this;
}

Viewer& Viewer::look_at(Vec3 loc)
{
	_target = loc;
	return *this;
}

Viewer& Viewer::up(Vec3 up)
{
	_up = up;
	return *this;
}

void Viewer::apply()
{
	mat4x4_look_at(_view, VEC3_ZERO.v, _target.v, _up.v);
}

// perspective matrix
Viewer& Viewer::field_of_view(float fov)
{
	this->fov = fov;
	return *this;
}

/**
 * @brief Generates the appropriate ray at the appropriate position and
 *        in the appropriate direction for the viewer's state
 * @param u - Cannonical horizontal screen coordinate on the closed interval [-1,1]
 * @param v - Cannonical vertical screen coordinate on the closed interval [-1,1]
 * @return Ray at the correct position, pointing in the correct direction for
 *         screen coordinate u,v
 */
Ray3 Viewer::ray_at_coord(float u, float v)
{
	Ray3 ray;

	const Vec3 scl(-1, -1, 1);
	// u = u / tanf(u * M_PI / 4);
	// v = v / tanf(v * M_PI / 4);

	ray.pos = Vec3(u, v, 0);
	//ray.dir = ray.pos;
	//ray.dir *= scl;

	vec4 tmp = {}, d_tmp = { 0, 0, 1 };
	vec4 left_tmp { u, 0, 0 }, up_tmp = { 0, -v, 0 };

	// viewer left and right for this ray
	
	vec3_add(d_tmp, d_tmp, up_tmp);
	vec3_add(d_tmp, d_tmp, left_tmp);

	d_tmp[3] = 1;
	//mat4x4_identity(_view);
	mat4x4_mul_vec4(tmp, _view, d_tmp);

	vec3_norm(ray.dir.v, tmp);
	ray.pos += _pos;

	return ray;
}

void Viewer::recalculate()
{
	if(dirtyView)
	{
		//mat4x4_look_at(this->view, _pos.v, _target.v, _up.v);
	}

	dirtyProj = dirtyView = false;
}
