#ifndef __MESH_H__
#define __MESH_H__
#include <vector>
#include "maths.h"
#include "texture.h"

class Mesh
{
private:
	std::vector<float3> vertices;
	std::vector<float3> normals;
	std::vector<float2> uvs;

	std::vector<int3> vert_faces;
	std::vector<int3> uv_faces;
	std::vector<int3> norm_faces;

	Texture* diffuse_texture;
	
public:
	Mesh();
	~Mesh();

	void read_obj_from_file(const char* filename);
	void Mesh::set_diffuse_texture(Texture* texture);
	int vertex_count();
	int face_count();
	bool has_diffuse_texture();
	float3 get_vertex(int idx);
	int2 get_uv_coord(int idx);
	int3 get_pos_idx(int idx);
	int3 get_uv_idx(int idx);

	Texture* get_diffuse_texture();
};


#endif //__MESH_H__