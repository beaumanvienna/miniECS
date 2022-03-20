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
        std::cout << "x: " << x << ",y: " << y << ",z: " << z << std::endl;
    }
};

struct NormalComponent
{
    float x,y,z;
    void Print()
    {
        std::cout << "x: " << x << ",y: " << y << ",z: " << z << std::endl;
    }
};

int main()
{
    std::cout << "type: " << ECS::GetComponentType<PositionComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<PositionComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<NormalComponent>() << std::endl;
    std::cout << "type: " << ECS::GetComponentType<NormalComponent>() << std::endl;

    ECS::EntityID obj01 = ECS::CreateEntity();
    ECS::EntityID obj02 = ECS::CreateEntity();
    ECS::EntityID obj03 = ECS::CreateEntity();
    
    std::cout << "obj01 ID: " << obj01 << std::endl;
    std::cout << "obj02 ID: " << obj02 << std::endl;
    std::cout << "obj03 ID: " << obj03 << std::endl;
    
    ECS::Push<PositionComponent>(obj01, {1.0, 2.0, 3.0});
    ECS::Push<NormalComponent>(obj01,   {4.0, 5.0, 6.0});
    ECS::Push<NormalComponent>(obj02,   {7.0, 8.0, 9.0});

    {
        auto view = ECS::View<PositionComponent>();
        view[obj01].Print();
    }

    {
        auto view = ECS::View<NormalComponent>();
        view[obj01].Print();
        view[obj02].Print();
    }    
}
