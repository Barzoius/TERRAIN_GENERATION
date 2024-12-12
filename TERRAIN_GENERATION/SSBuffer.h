#pragma once


#include "Bindable.h"


class SSBuffer : public Bindable
{

public:
    SSBuffer();
    ~SSBuffer();

    void Bind() override;
    //void Unbind() override;


    GLuint GetID();
private:
    GLuint SS_ID;

};