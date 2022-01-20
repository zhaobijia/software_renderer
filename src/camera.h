#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "maths.h"

class Camera
{
private:
	float fov; //the angle between view frustum's upper plane and bottom plane in randians.
	float near_plane, far_plane;
	float left, right, bottom, top;
	//TODO: shadow buffer

public:
	float3 position;
	float3 lookat;
	float3 up;
	float3 target;

	Camera();//default
	Camera(float3 _position, float3 _lookat, float3 _up);
	~Camera();

	void set_target(float3 new_target);
	void set_position(float3 new_pos);
	void set_fov(float rad);
	void set_frustum(float l, float r, float b, float t, float n, float f);
	void update_camera(float3 new_pos, float3 new_lookat, float3 new_up);

	void move(float3 dir);
	void pan_horizontal(float theta);
	void orbit_around_target(float2 angle);
	void auto_orbit_horizontal(float unit_angle);
	
	void reset();
};

#endif //__CAMERA_H__