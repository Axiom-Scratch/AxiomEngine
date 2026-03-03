#pragma once

#include "Core/Application.h"
#include "Core/Base.h"

int main()
{
    auto app = Axiom::CreateApplication();
    if (app)
    {
        app->Run();
    }
    return 0;
}
