#include <fstream>
#include<sstream>
#include <assert.h>
#include "mesh.h"



Mesh::Mesh() {}

Mesh::~Mesh() 
{
}

void Mesh::read_obj_from_file(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ifstream::in);
	if (file.fail())
	{
		std::cerr << "Error: " << strerror(errno);
	}
	assert(file.good());
	std::string line;
	while (!file.eof())
	{
		std::getline(file, line);
		std::istringstream iss(line);
		if (line.compare(0,2,"v ") == 0)
		{
			float3 v_read;
			int read_count = sscanf(line.c_str(), "v %f %f %f", &v_read.x, &v_read.y, &v_read.z);
			assert(read_count == 3);
			vertices.push_back(v_read);
		}
		else if (line.compare(0, 3, "vt ") == 0)
		{
			//uv/texture load
			float2 uv_read;
			int read_count = sscanf(line.c_str(), "vt %f %f", &uv_read.x, &uv_read.y);
			assert(read_count == 2);
			uvs.push_back(uv_read);

		}
		else if (line.compare(0, 3, "vn ") == 0)
		{
			float3 n_read;
			int read_count = sscanf(line.c_str(), "vn %f %f %f", &n_read.x, &n_read.y, &n_read.z);
			assert(read_count == 3);
			normals.push_back(n_read);
		}
		else if (line.compare(0, 2, "f ") == 0)
		{
			std::vector<int> v_read(3), vt_read(3), vn_read(3);

			int v_ext, vt_ext, vn_ext; //4th vertex for the quad representation
			int read_count = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v_read[0], &vt_read[0], &vn_read[0], &v_read[1], &vt_read[1], &vn_read[1], &v_read[2], &vt_read[2], &vn_read[2], &v_ext, &vt_ext, &vn_ext);

			if (read_count <= 9)
			{
				if (read_count < 9)
				{
					read_count = sscanf(line.c_str(), "f %d %d %d", &v_read[0], &v_read[1], &v_read[2]);
				}
				for (int i = 0; i < 3; i++)
				{
					v_read[i]--;
					vt_read[i]--;
					vn_read[i]--;
				}
				vert_faces.push_back(v_read);
				uv_faces.push_back(vt_read);
				norm_faces.push_back(vn_read);
			}
			else
			{
				std::vector<int> v_read_ext(3), vt_read_ext(3), vn_read_ext(3);
				for (int i = 0; i < 3; i++)
				{
					v_read[i]--;
					vt_read[i]--;
					vn_read[i]--;
				}
				v_ext--;
				vt_ext--;
				vn_ext--;

				v_read_ext[0] = v_read[1];
				v_read_ext[1] = v_ext;
				v_read_ext[2] = v_read[2];

				vt_read_ext[0] = vt_read[1];
				vt_read_ext[1] = vt_ext;
				vt_read_ext[2] = vt_read[2];

				vn_read_ext[0] = vn_read[1];
				vn_read_ext[1] = vn_ext;
				vn_read_ext[2] = vn_read[2];

				vert_faces.push_back(v_read);
				uv_faces.push_back(vt_read);
				norm_faces.push_back(vn_read);
				
				vert_faces.push_back(v_read_ext);
				uv_faces.push_back(vt_read_ext);
				norm_faces.push_back(vn_read_ext);
			}

		}
	}
	std::cout << "vert faces:" << vert_faces.size() << ",";
	std::cout << "uv faces:" << uv_faces.size() << ",";
	std::cout << "normal faces:" << norm_faces.size() << ",";


}

Texture* Mesh::get_diffuse_texture()
{
	return diffuse_texture;
}
void Mesh::set_diffuse_texture(Texture* texture)
{
	diffuse_texture = texture;
	has_diffuse = true;
	calculate_tangents();
}

bool Mesh::has_diffuse_texture()
{
	return has_diffuse;
}

Texture* Mesh::get_normal_texture()
{
	return normal_texture;
}

void Mesh::set_normal_texture(Texture* texture)
{
	normal_texture = texture;
	has_normal = true;
}

bool Mesh::has_normal_texture()
{
	return has_normal;
}

int Mesh::vertex_count()
{
	return vertices.size();
}

int Mesh::face_count()
{
	return vert_faces.size();
}

float3 Mesh::get_vertex(int idx)
{
	return vertices[idx];
}

float2 Mesh::get_uv_coord(int idx)
{
	return uvs[idx];
}

float3 Mesh::get_normal(int idx)
{
	return normals[idx];
}

float3 Mesh::get_tangent(int idx)
{
	return tangents[idx];
}
std::vector<int> Mesh::get_pos_idx(int idx)
{

	return vert_faces[idx];
}

std::vector<int> Mesh::get_uv_idx(int idx)
{

	return uv_faces[idx];
}

std::vector<int> Mesh::get_normal_idx(int idx)
{

	return norm_faces[idx];
	
}

std::vector<int> Mesh::get_tangent_idx(int idx)
{
	//return at face index idx, what would be tangent idx
	return tangent_faces[idx];

}

float3 Mesh::get_vertex_with_face_idx(int f_idx, int v_idx)
{
	std::vector<int> pos_idx = get_pos_idx(f_idx);

	float3 vert = get_vertex(pos_idx[v_idx]);

	return vert;
}
float2 Mesh::get_uv_with_face_idx(int f_idx, int u_idx)
{
	std::vector<int> uv_idx = get_uv_idx(f_idx);
	float2 uv = get_uv_coord(uv_idx[u_idx]);

	return uv;
}
float3 Mesh::get_normal_with_face_idx(int f_idx, int n_idx)
{
	std::vector<int> norm_idx = get_normal_idx(f_idx);
	float3 norm = get_normal(norm_idx[n_idx]);
	return norm;
}

float3 Mesh::get_tangent_with_face_idx(int f_idx, int t_idx)
{
	std::vector<int> tan_idx = get_tangent_idx(f_idx);
	float3 tangent = get_tangent(tan_idx[t_idx]);
	return tangent;
}

//reference:
//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
void Mesh::calculate_tangents()
{

	int faces = face_count();
	std::unordered_map<int, std::vector<float3>> vert_tangent_dict;
	for (int i = 0; i < faces; i++)
	{
		std::vector<int> pos_idx = get_pos_idx(i);
		int vert_idx_0 = pos_idx[0];
		int vert_idx_1 = pos_idx[1];
		int vert_idx_2 = pos_idx[2];

		std::vector<int> tangent_indices;

		tangent_indices.push_back(vert_idx_0);
		tangent_indices.push_back(vert_idx_1);
		tangent_indices.push_back(vert_idx_2);
		tangent_faces.push_back(tangent_indices);

		float3 pos0 = get_vertex_with_face_idx(i, 0); 
		float3 pos1 = get_vertex_with_face_idx(i, 1);
		float3 pos2 = get_vertex_with_face_idx(i, 2);

		float2 uv0 = get_uv_with_face_idx(i, 0);
		float2 uv1 = get_uv_with_face_idx(i, 1);
		float2 uv2 = get_uv_with_face_idx(i, 2);
		//..........edge
		float3 edge1 = pos1 - pos0;
		float3 edge2 = pos2 - pos0;
		//..........uv
		float2 d_uv1 = uv1 - uv0;
		float2 d_uv2 = uv2 - uv0;

		float f0 = 1.f / (d_uv1.x * d_uv2.y - d_uv2.x * d_uv1.y);

		float3 tan;
		tan.x = f0 * (d_uv2.x * edge1.x - d_uv1.x * edge2.x);
		tan.y = f0 * (d_uv2.x * edge1.y - d_uv1.x * edge2.y);
		tan.z = f0 * (d_uv2.x * edge1.z - d_uv1.x * edge2.z);
		//for each vertex, it might have multiple possible tangent and normal values depending on how many triangles are around it.
		//try to store multiple tangents on one vertex, and average/interpolate later
		
		vert_tangent_dict[vert_idx_0].push_back(tan.normalize());
		vert_tangent_dict[vert_idx_1].push_back(tan.normalize());
		vert_tangent_dict[vert_idx_2].push_back(tan.normalize());

	}
	int vertices = vertex_count();
	//average tan for each vertex
	for (int i = 0; i <vertices ; i++)
	{
		int size = vert_tangent_dict[i].size();
		float3 temp_t;
		for (int j = 0; j < size; j++)
		{
			temp_t = temp_t + vert_tangent_dict[i][j];
		}
		tangents.push_back(temp_t.normalize());
	}
	


}