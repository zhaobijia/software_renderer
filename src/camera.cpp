#include "camera.h"

Camera::Camera()
{
	position = float3(0, 0, 0);
	lookat = float3(0, 0, -1);
	up = float3(0, 1, 0);

	fov = M_PI;

	near_plane = -1;
	far_plane = -100;
	left = -1;
	right = 1;
	bottom = -1;
	top = 1;

}

Camera::Camera(float3 _position, float3 _lookat, float3 _up):position(_position), lookat(_lookat), up(_up)
{
	fov = M_PI;
	near_plane = -1;
	far_plane = -1000;
	left = -1;
	right = 1;
	bottom = -1;
	top = 1;
}
Camera::~Camera(){}

void Camera::set_target(float3 new_target)
{
	target = new_target;
	lookat = (target - position).normalize();

}
void Camera::set_position(float3 new_pos)
{
	position = new_pos;
}
void Camera::set_fov(float rad)
{
	fov = rad;

}
void Camera::set_frustum(float l,float r, float b, float t, float n, float f)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
	near_plane = n;
	far_plane = f;
}
void Camera::update_camera(float3 new_pos, float3 new_lookat, float3 new_up)
{
	position = new_pos;
	lookat = new_lookat;
	up = new_up;
}

//free move:
void Camera::move(float3 dir)
{
	position.x += dir.x;
	position.y += dir.y;
	position.z += dir.z;
}
void Camera::pan_horizontal(float theta)
{
	//update lookat direction
	float4x4 rotate;
	rotate = rotate.rotate_round(theta, up);
	lookat = rotate * lookat;
}

void Camera::orbit_around_target(float2 screen_dist)
{
	float3 target_to_cam = position - target;
	float4x4 rotate;
	float3 right = lookat.cross(up);
	rotate = rotate.rotate_round(screen_dist.x, up);// 
	target_to_cam = rotate * target_to_cam;
	rotate = rotate.rotate_round(screen_dist.y, right);
	target_to_cam = rotate * target_to_cam;
	//update position
	position = target + target_to_cam;
	//update looking angle
	lookat = (target - position).normalize();
	up = right.cross(lookat).normalize();
}

void Camera::auto_orbit_horizontal(float unit_angle)
{
	float3 target_to_cam = position - target;
	float3 right = lookat.cross(up);
	float4x4 rotate;
	rotate = rotate.rotate_round(unit_angle, float3(0,1,0));
	target_to_cam = rotate * target_to_cam;
	//update position
	position = target + target_to_cam;
	lookat = (target - position).normalize();
	up = right.cross(lookat).normalize();
}

void Camera::reset()
{
	position = float3(0, 0, 0);
	lookat = float3(0, 0, -1);
	up = float3(0, 1, 0);

	fov = M_PI;

	near_plane = -1;
	far_plane = -1000;
	left = -1;
	right = 1;
	bottom = -1;
	top = 1;

}