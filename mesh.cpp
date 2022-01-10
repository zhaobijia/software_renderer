#include <fstream>
#include<sstream>
#include <assert.h>
#include "mesh.h"

Mesh::Mesh() {}
Mesh::~Mesh() {}

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
			int3 v_read, vt_read, vn_read;
			int read_count = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v_read.x, &vt_read.x, &vn_read.x, &v_read.y, &vt_read.y, &vn_read.y, &v_read.z, &vt_read.z, &vn_read.z);
			vert_faces.push_back(v_read);
			uv_faces.push_back(vt_read);
			norm_faces.push_back(vn_read);

		}
	}
	std::cout << "vert faces:" << vert_faces.size() << ",";
	std::cout << "uv faces:" << uv_faces.size() << ",";
	std::cout << "normal faces:" << norm_faces.size() << ",";

}
void Mesh::read_texture_from_file(const char* filename)
{

}
void Mesh::create_frome_vertices(std::vector<float3> _verts)
{

}
int Mesh::vertex_count()
{
	return vertices.size();
}


int Mesh::face_count()
{
	return vert_faces.size();
}