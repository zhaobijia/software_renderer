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

//reference:
//https://learnopengl.com/Advanced-Lighting/Normal-Mapping
void Mesh::calculate_tangents()
{
	//calculate tangents for each triangle face
	int faces = face_count();
	for (int i = 0; i < faces; i++)
	{
		float3 pos0 = get_vertex_with_face_idx(i, 0); 
		float3 pos1 = get_vertex_with_face_idx(i, 1);
		float3 pos2 = get_vertex_with_face_idx(i, 2);

		float2 uv0 = get_uv_with_face_idx(i, 0);
		float2 uv1 = get_uv_with_face_idx(i, 1);
		float2 uv2 = get_uv_with_face_idx(i, 2);
		//..........edge
		float3 edge01 = pos1 - pos0;
		float3 edge02 = pos2 - pos0;

		float3 edge11 = pos2 - pos1;
		float3 edge12 = pos0 - pos1;

		float3 edge21 = pos0 - pos2;
		float3 edge22 = pos1 - pos2;
		//..........uv
		float2 d_uv01 = uv1 - uv0;
		float2 d_uv02 = uv2 - uv0;

		float2 d_uv11 = uv2 - uv1;
		float2 d_uv12 = uv0 - uv1;

		float2 d_uv21 = uv0 - uv2;
		float2 d_uv22 = uv1 - uv2;

		float f0 = 1.f / (d_uv01.x * d_uv02.y - d_uv02.x * d_uv01.y);
		float f1 = 1.f / (d_uv11.x * d_uv12.y - d_uv12.x * d_uv11.y);
		float f2 = 1.f / (d_uv21.x * d_uv22.y - d_uv22.x * d_uv21.y);
		
		float3 tan0, tan1, tan2;
		tan0.x = f0 * (d_uv02.x * edge01.x - d_uv01.x * edge02.x);
		tan0.y = f0 * (d_uv02.x * edge01.y - d_uv01.x * edge02.y);
		tan0.z = f0 * (d_uv02.x * edge01.z - d_uv01.x * edge02.z);

		tan1.x = f1 * (d_uv12.x * edge11.x - d_uv11.x * edge12.x);
		tan1.y = f1 * (d_uv12.x * edge11.y - d_uv11.x * edge12.y);
		tan1.z = f1 * (d_uv12.x * edge11.z - d_uv11.x * edge12.z);

		tan2.x = f2 * (d_uv22.x * edge21.x - d_uv21.x * edge22.x);
		tan2.y = f2 * (d_uv22.x * edge21.y - d_uv21.x * edge22.y);
		tan2.z = f2 * (d_uv22.x * edge21.z - d_uv21.x * edge22.z);

		float3 tan = (tan0.normalize() + tan1.normalize() + tan2.normalize()) * 0.33333f;
		tangents.push_back(tan0.normalize());

	}

}