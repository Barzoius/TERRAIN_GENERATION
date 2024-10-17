#include <vector>

#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

template <class VERTEX>
class IndexedTrigList
{
public:

    IndexedTrigList() = default;

    IndexedTrigList(std::vector<VERTEX> vertices, std::vector<unsigned int> indices)
        :
        vertices( std::move(vertices) ),
        indices( std::move(indices) )
    {
        //assert(vertices.size() > 2);
        //assert(indices.size() % 3 == 0);
    }

    void Treansform(glm::vec3 scaleFactors)
    {

        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scaleFactors);

        for (auto& vertex : vertices)
        {
            auto& pos = vertex.pos;

            pos = scaleMatrix * glm::vec4(pos, 1.0f);
        }
    }

public:
    std::vector<VERTEX> vertices;
    std::vector<unsigned int> indices;
};
