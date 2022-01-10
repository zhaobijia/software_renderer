#ifndef __MESH_H__
#define __MESH_H__
#include <vector>
#include "maths.h"

class Mesh
{
private:
	std::vector<float3> vertices;
	std::vector<float3> normals;
	std::vector<float2> uvs;

	std::vector<int3> vert_faces;
	std::vector<int3> uv_faces;
	std::vector<int3> norm_faces;


	int tex_height, tex_width;

	
public:
	Mesh();
	~Mesh();

	void read_obj_from_file(const char* filename);
	void read_texture_from_file(const char* filename);
	int vertex_count();
	int face_count();

};


#endif //__MESH_H__