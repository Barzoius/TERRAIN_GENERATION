//#include "VertexBuffer.h"
//
//
////VertexBuffer::VertexBuffer(std::vector<glm::vec3>& vertices)
////    :
////    mVertices(vertices)
////{
////    glGenBuffers(1, &ID);
////    //glBindBuffer(GL_ARRAY_BUFFER, ID);
////   // glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
////}
//
//VertexBuffer::~VertexBuffer()
//{
//}
//
//
//void VertexBuffer::Bind()
//{
//    glBindBuffer(GL_ARRAY_BUFFER, ID);
//    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeVERTEX, mVertices.data(), GL_STATIC_DRAW);
//}
//
//
//size_t VertexBuffer::GetVertexSize() const
//{
//    return sizeVERTEX;
//}