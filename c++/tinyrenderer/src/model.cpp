#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) :
    verts_(), faces_vert_()
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        std::istringstream iss(line.c_str());

        char trash;

        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vt "))
        {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++) iss >> uv.raw[i];
            uvs_.push_back({uv.x, uv.y});
        }
        else if (!line.compare(0, 2, "f "))
        {
            std::vector<int> vertex_idxs;
            std::vector<int> tex_idxs;

            int itrash, vert_idx, texture_coord_idx;
            iss >> trash;

            while (iss >> vert_idx >> trash >> texture_coord_idx >> trash >> itrash)
            {
                // in wavefront obj all indices start at 1, not zero
                vertex_idxs.push_back(--vert_idx);
                tex_idxs.emplace_back(--texture_coord_idx);
            }
            faces_vert_.push_back(vertex_idxs);
            faces_uv_.push_back(tex_idxs);
        }
    }
    std::cerr << "# v# " << verts_.size()
              << "# uv# " << uvs_.size()
              << " f# " << faces_vert_.size();
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)verts_.size();
}

int Model::nfaces()
{
    return (int)faces_vert_.size();
}

/// @brief return the 3 vertex idx within a face
/// @param idx
std::vector<int> Model::face_vert(int idx)
{
    return faces_vert_[idx];
}

/// @brief return the 3 texture uv within a face
/// @param idx
std::vector<int> Model::face_uv(int idx)
{
    return faces_uv_[idx];
}

/// @brief return vertice position
Vec3f Model::vert(int idx)
{
    return verts_[idx];
}

/// @brief return texture position
Vec2f Model::uv(int idx)
{
    return uvs_[idx];
}