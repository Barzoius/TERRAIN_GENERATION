#include "Drawable.h"
#include "ElementBuffer.h"

#include <cassert>
#include <typeinfo>



void Drawable::Draw(glm::mat4& viewMatrix, glm::mat4& projMatrix) const noexcept
{
    if (pShaderProgram) {
        pShaderProgram->use(); 
    }

    glm::mat4 model = this->GetTransformMatrix();

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -6.0f));


    //MODEL

    unsigned int modelLoc = glGetUniformLocation(this -> GetShader()->GetID(), "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    //VIEW

    unsigned int viewLocBox = glGetUniformLocation(this->GetShader()->GetID(), "view");

    glUniformMatrix4fv(viewLocBox, 1, GL_FALSE, &viewMatrix[0][0]);


    //PROJ

    this->GetShader()->setMat4("projection", projMatrix);


    for (auto& b : bindables)
    {

        b->Bind();
    }
    int rez = 40;

    glDrawArrays(GL_PATCHES, 0, 4 * rez * rez);
    //glDrawElements(GL_TRIANGLES, (GLsizei)pElemBuffer->GetIndiciesCount(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

}


void Drawable::DrawIndexed(glm::mat4& viewMatrix, glm::mat4& projMatrix) const noexcept
{
    if (pShaderProgram) {
        pShaderProgram->use();
    }

    glm::mat4 model = this->GetTransformMatrix();

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -6.0f));


    //MODEL

    unsigned int modelLoc = glGetUniformLocation(this->GetShader()->GetID(), "model");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    //VIEW

    unsigned int viewLocBox = glGetUniformLocation(this->GetShader()->GetID(), "view");

    glUniformMatrix4fv(viewLocBox, 1, GL_FALSE, &viewMatrix[0][0]);


    //PROJ

    this->GetShader()->setMat4("projection", projMatrix);


    for (auto& b : bindables)
    {

        b->Bind();
    }
    int rez = 40;

    //glDrawArrays(GL_PATCHES, 0, 4 * rez * rez);
    glDrawElements(GL_TRIANGLES, (GLsizei)pElemBuffer->GetIndiciesCount(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
    bindables.push_back(std::move(bind));
}

void Drawable::AddElementBuffer(std::unique_ptr<ElementBuffer> ibuf) noexcept
{
    assert("Attempting to add index buffer a second time" && pElemBuffer == nullptr);
    pElemBuffer = ibuf.get();
    bindables.push_back(std::move(ibuf));
}

void Drawable::AddShaderProgram(std::unique_ptr<ShaderSuite> ss) noexcept
{
    assert("Attempting to add a second shader program" && pShaderProgram == nullptr);
    pShaderProgram = std::move(ss);
}


ShaderSuite* Drawable::GetShader() const noexcept
{
    return pShaderProgram.get();
}

