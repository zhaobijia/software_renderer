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

	std::vector<std::vector<int>> vert_faces;
	std::vector<std::vector<int>> uv_faces;
	std::vector<std::vector<int>> norm_faces;

	Texture* diffuse_texture = NULL;
	bool has_diffuse = false;
	
public:
	Mesh();
	~Mesh();

	void read_obj_from_file(const char* filename);
	void Mesh::set_diffuse_texture(Texture* texture);
	int vertex_count();
	int face_count();
	bool has_diffuse_texture();

	float3 get_vertex(int idx);//get actual position value with idx in the read-in vertices array
	int2 get_uv_coord(int idx);//get actual uv value with idx in the read-in uvs array
	float3 get_normal(int idx);//get actual normal value with idx in the read-in normals array

	std::vector<int> get_pos_idx(int idx);//3 indices of position value for a certain triangle-face with index idx in vert_faces array
	std::vector<int> get_uv_idx(int idx);//3 indices of uv value for a certain triangle-face with index idx in uv_faces array
	std::vector<int> get_normal_idx(int idx); //3 indices of normal value for a certain triangle-face with index idx in norm_faces array

	float3 get_vertex_with_face_idx(int f_idx, int v_idx);
	int2 get_uv_with_face_idx(int f_idx, int u_idx);
	float3 get_normal_with_face_idx(int f_idx, int n_idx);

	Texture* get_diffuse_texture();
};


#endif //__MESH_H__