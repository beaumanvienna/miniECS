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

#pragma once

#include <bitset>
#include <vector>

#include "types.h"

namespace ECS
{
    static constexpr uint POOL_SIZE = 1000;
    typedef std::bitset<32> ComponentMask;
    typedef uint32_t EntityID;

    uint globalComponetCounter = 0;
    uint globalEntityCounter = 1;

    struct EntityDescription;
    std::vector<EntityDescription> entities;
    std::vector<uint> recycle; // entity IDs to be recycled

    struct ComponentPool;
    std::vector<ComponentPool> componentPools;

    template <typename T>
    std::vector<unsigned char> AllocatePool()
    {
        size_t size = sizeof(T) * POOL_SIZE;
        std::vector<unsigned char> pool(size);
        return pool;
    }

    template <typename T>
    uint GetComponentType()
    {
        static uint componetTypeIdentifier = 0;

        // 1st time for this type
        if (!componetTypeIdentifier)
        {
            componetTypeIdentifier = globalComponetCounter++;
            ComponentPool pool = {AllocatePool<T>()};
            componentPools.push_back(pool);
        }
        return componetTypeIdentifier;
    }

    struct EntityDescription
    {
        EntityID m_ID;
        ComponentMask m_Mask;
    };
    
    struct ComponentPool
    {
        std::vector<unsigned char> m_Pool;
    };

    EntityID CreateEntity()
    {
        uint id;
        if (recycle.size())
        {
            id = recycle.back();
            recycle.pop_back();
            entities[id].m_ID = id;
            entities[id].m_Mask = ComponentMask();
        }
        else
        {
            id = globalEntityCounter++;
            entities.push_back({id, ComponentMask()});
        }
        return id;
    }

    template <typename T>
    void Push(EntityID id, T component)
    {
        assert(id < globalEntityCounter);
        uint componentType = GetComponentType<T>();

        entities[id].m_Mask.set(id);

        // get pool
        T* pool = (T*) componentPools[componentType].m_Pool.data();
        pool[id] = component;
    }

    template <typename T>
    T* View()
    {
        uint componentType = GetComponentType<T>();
        return (T*) componentPools[componentType].m_Pool.data();
    }
}
