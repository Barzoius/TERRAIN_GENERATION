#pragma once

#include "IndexedTrigList.h"
#include "Math.h"

class Sphere
{
public:
    template <typename VERTEX>
    static IndexedTrigList<VERTEX> MakeUVSphere(int stacks, int sectors)
    {
        assert(stacks >= 2);
        assert(sectors >= 2);

        constexpr float radius = 1.0f;

        const float lattitudeAngle = FPI / stacks;
        const float longitudeAngle = 2.0f * FPI / sectors;

        float stackAngle, sectorAngle;

        std::vector<glm::vec3> vertices;

        for (int i = 0; i <= stacks; i++)
        {

            stackAngle = FPI / 2 - i * lattitudeAngle;

            float z = radius * sinf(stackAngle);

            float xy = radius * cosf(stackAngle);


            for (int j = 0; j <= sectors; j++)
            {


                glm::vec3 calculatedPos;

                sectorAngle = j * longitudeAngle;

                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);

                calculatedPos = glm::vec3(x, y, z);

                vertices.emplace_back(calculatedPos);
            }

        }


        std::vector<VERTEX> verts(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            verts[i].pos = vertices[i];
        }



        std::vector<unsigned int> indices;
        int k1, k2;
        for (int i = 0; i < stacks; i++)
        {
            k1 = i * (sectors + 1);     // beginning of current stack
            k2 = k1 + sectors + 1;      // beginning of next stack

            for (int j = 0; j < sectors; j++, k1++, k2++)
            {
                /// k1--k1+1
                /// |  / |
                /// | /  |
                /// k2--k2+1

                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }

            }
        }

        return { std::move(verts),std::move(indices) };

    }



    template <typename VERTEX>
    static IndexedTrigList<VERTEX> Make()
    {
        return MakeUVSphere<VERTEX>(32, 32);
    }
};