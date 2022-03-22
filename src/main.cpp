/* Engine Copyright (c) 2021 Engine Development Team
   https://github.com/beaumanvienna/gfxRenderEngine

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <iostream>

#include "types.h"
#include "ecs.h"

struct PositionComponent
{
    float x,y,z;
    void Print()
    {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
    }
};

struct NormalComponent
{
    float x,y,z;
    void Print()
    {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
    }
};

int main()
{
    std::cout << "type: " << ECS::GetComponentType<PositionComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<PositionComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<NormalComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<NormalComponent>() << std::endl;

    ECS::EntityID obj00 = ECS::CreateEntity();
    ECS::EntityID obj01 = ECS::CreateEntity();
    ECS::EntityID obj02 = ECS::CreateEntity();
    
    std::cout << "obj00 ID: " << obj00 << std::endl;
    std::cout << "obj01 ID: " << obj01 << std::endl;
    std::cout << "obj02 ID: " << obj02 << std::endl;
    
    ECS::Push<PositionComponent>(obj00, { 1.0,  2.0,  3.0});
    ECS::Push<PositionComponent>(obj01, {-1.0, -2.0, -3.0});
    ECS::Push<PositionComponent>(obj02, {-4.0, -5.0, -6.0});
    ECS::Push<NormalComponent>(obj00,   { 4.0,  5.0,  6.0});
    ECS::Push<NormalComponent>(obj01,   { 7.0,  8.0,  9.0});

    {
        auto position = ECS::Get<PositionComponent>(obj01);
        position.Print();
    }

    {
        auto normal = ECS::Get<NormalComponent>(obj01);
        normal.Print();
    }

    {
        auto normal = ECS::Get<NormalComponent>(obj02);
        normal.Print();
    }

    {
        std::cout << "positionView alone, should be: obj02" << std::endl;
        auto positionView = ECS::View<PositionComponent>();
        for (auto obj: positionView)
        {
            std::cout << "EnityID: " << obj<< std::endl;
            auto position = ECS::Get<PositionComponent>(obj);
            position.Print();
        }
    }

    {
        std::cout << "normalView alone, should be empty" << std::endl;
        auto normalView = ECS::View<NormalComponent>();
        for (auto obj: normalView)
        {
            std::cout << "EnityID: " << obj<< std::endl;
            auto normal = ECS::Get<NormalComponent>(obj);
            normal.Print();
        }
    }

    {
        std::cout << "position + normal view, should obj00 & obj01" << std::endl;
        auto positionNormalView = ECS::View<PositionComponent,NormalComponent>();
        for (auto obj: positionNormalView)
        {
            std::cout << "EnityID: " << obj<< std::endl;
            auto position = ECS::Get<PositionComponent>(obj);
            position.Print();
            auto normal = ECS::Get<NormalComponent>(obj);
            normal.Print();
        }
    }

    // test component push based on component type ID
    auto obj03 = ECS::CreateEntity();
    auto componentTypeID = ECS::GetComponentType<PositionComponent>();

    PositionComponent positionComponent = { 10.0, 20.0, 30.0}; // constructor is called for stack-allocated object
    // push via component type ID (without template)
    ECS::Push(obj03, componentTypeID, &positionComponent);

    // and read back
    std::cout << "Get obj03 via component type ID" << std::endl;
    auto comp =  (PositionComponent*)ECS::Get(obj03, componentTypeID);
    comp->Print();

    {
        std::cout << "positionView alone, should be: obj02 & obj03" << std::endl;
        auto positionView = ECS::View<PositionComponent>();
        for (auto obj: positionView)
        {
            std::cout << "EnityID: " << obj<< std::endl;
            auto position = ECS::Get<PositionComponent>(obj);
            position.Print();
        }
    }
}
