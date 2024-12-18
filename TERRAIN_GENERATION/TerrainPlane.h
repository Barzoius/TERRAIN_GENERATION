#pragma once


#include "IndexedTrigList.h"
#include "Math.h"

#include <vector>
#include <array>


class TerrainPlane
{
public:

    template<typename VERTEX>
    static IndexedTrigList<VERTEX> MakeForTesselation(int resolution)
    {

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;

        constexpr float width = 2.0f;
        constexpr float height = 2.0f;

        const float xSide = width / 2.0f;
        const float ySide = height / 2.0f;

        float FRES = (float)resolution;

        for (int i = 0; i <= resolution - 1; i++)
        {
            for (int j = 0; j <= resolution - 1; j++)
            {
                vertices.push_back(glm::vec3((-xSide + width * i / FRES),
                    (0.0f),
                    (-ySide + height * j / FRES)));

                uvs.push_back(glm::vec2(i / FRES, j / FRES));


                vertices.push_back(glm::vec3((-xSide + width * (i + 1) / FRES),
                    (0.0f),
                    (-ySide + height * j / FRES)));

                uvs.push_back(glm::vec2((i + 1) / FRES, j / FRES));


                vertices.push_back(glm::vec3((-xSide + width * i / FRES),
                    (0.0f),
                    (-ySide + height * (j + 1) / FRES)));

                uvs.push_back(glm::vec2(i / FRES, (j + 1) / FRES));


                vertices.push_back(glm::vec3((-xSide + width * (i + 1) / FRES),
                    (0.0f),
                    (-ySide + height * (j + 1) / FRES)));

                uvs.push_back(glm::vec2((i + 1) / FRES, (j + 1) / FRES));

            }
        }

        std::vector<VERTEX> verts(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            verts[i].pos = vertices[i];
            verts[i].tex = uvs[i];
        }


        std::vector<unsigned int> indices;

        return{ std::move(verts),std::move(indices) };
    }



    template<typename V>
    static IndexedTrigList<V> Make()
    {
        return  MakeForTesselation<V>(40);
    }

};