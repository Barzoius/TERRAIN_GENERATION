#include <vector>

#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

template <class VERTEX>
class IndexedTrigList
{
public:

    IndexedTrigList() = default;

    IndexedTrigList(std::vector<VERTEX> vertices, std::vector<unsigned short> indices)
        :
        vertices( std::move(vertices) ),
        indices( std::move(indices) )
    {
        assert(vertices.size() > 2);
        assert(indices.size() % 3 == 0);
    }

    void Treansform(glm::mat4 matrix)
    {
        for (auto& vertex : vertices)
        {
            const glm::vec3 pos = vertex.pos;
        }
    }
private:
    std::vector<VERTEX> vertices;
    std::vector<unsigned short> indices;
};
