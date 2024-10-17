#pragma once


#include "IndexedTrigList.h"
#include "Math.h"

#include <vector>
#include <array>


class Plane
{
public:

    template<typename VERTEX>
    static IndexedTrigList<VERTEX> MakeTesselated(int xDivisions, int yDivisions)
    {
        assert(xDivisions >= 1);
        assert(yDivisions >= 1);

        constexpr float width = 2.0f;
        constexpr float height = 2.0f;

        const int nXVertices = xDivisions + 1;
        const int nYVertices = yDivisions + 1;


        std::vector<glm::vec3> vertices(nXVertices * nYVertices);

        const float xSide = width / 2.0f;
        const float ySide = height / 2.0f;

        /// Grid cell/quad width and height sizes
        const float xDivisionSize = width / float(xDivisions);
        const float yDivisionSize = height / float(yDivisions);

        glm::vec3 bottomLeft = glm::vec3(-xSide, -ySide, 0.0f);

        for (int y = 0, i = 0; y < nYVertices; y++)
        {
            const float yPos = float(y) * yDivisionSize;

            for (int x = 0; x < nXVertices; x++, i++)
            {
                const float xPos = float(x) * xDivisionSize;

                /// The postion values of the vertex relative to the origin
                glm::vec3 relativeVertex = glm::vec3(xPos, yPos, 0.0f);
                /// The absoulte postion values of the vertex in 3D Space
                glm::vec3 vertex = bottomLeft + relativeVertex;

                vertices[i] = vertex;
            }
        }

        std::vector<VERTEX> verts(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            verts[i].pos = vertices[i];
        }


        std::vector<unsigned int> indices;

        indices.reserve(sq(xDivisions * yDivisions) * 6);

        /// returns the index of a vetex in the vertices vector
        const auto v2i = [nXVertices](size_t x, size_t y)
            {
                return (unsigned int)(y * nXVertices + x);
            };

        for (size_t y = 0; y < yDivisions; y++)
        {
            for (size_t x = 0; x < xDivisions; x++)
            {
                const std::array<unsigned int, 4> indexArray =
                {
                    v2i(x,y),          // bottom left
                    v2i(x + 1, y),     // bottom right
                    v2i(x,y + 1),      // top left
                    v2i(x + 1, y + 1)  // top right
                };

                indices.push_back(indexArray[0]); /// V0   V2 +-----+ V1
                indices.push_back(indexArray[2]); /// V2      |\    |
                indices.push_back(indexArray[1]); /// V1      | \   |
                indices.push_back(indexArray[1]); /// V1      |  \  |
                indices.push_back(indexArray[2]); /// V2      |   \ |
                indices.push_back(indexArray[3]); /// V3   V0 +-----+ V3

            }
        }

        return{ std::move(verts),std::move(indices) };
    }


    template<typename V>
    static IndexedTrigList<V> Make()
    {
        return MakeTesselated<V>(8, 8);
    }
};