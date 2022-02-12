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

int2 Mesh::get_uv_coord(int idx)
{
	float2 uv_01 = uvs[idx];
	int2 uv_coord = int2(uv_01.x * diffuse_texture->get_width(), uv_01.y * diffuse_texture->get_height());
	return uv_coord;
}

float3 Mesh::get_normal(int idx)
{
	return normals[idx];
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


float3 Mesh::get_vertex_with_face_idx(int f_idx, int v_idx)
{
	std::vector<int> pos_idx = get_pos_idx(f_idx);

	float3 vert = get_vertex(pos_idx[v_idx]);

	return vert;
}
int2 Mesh::get_uv_with_face_idx(int f_idx, int u_idx)
{
	std::vector<int> uv_idx = get_uv_idx(f_idx);
	int2 uv = get_uv_coord(uv_idx[u_idx]);

	return uv;
}
float3 Mesh::get_normal_with_face_idx(int f_idx, int n_idx)
{
	std::vector<int> norm_idx = get_normal_idx(f_idx);
	float3 norm = get_normal(norm_idx[n_idx]);
	return norm;
}