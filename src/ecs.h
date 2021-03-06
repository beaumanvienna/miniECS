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
#include <cstring>

#include "types.h"

namespace ECS
{
    static constexpr uint POOL_SIZE = 1000;
    typedef std::bitset<32> ComponentMask;
    typedef uint32_t EntityID;

    uint globalComponetCounter = 0;
    uint globalEntityCounter = 0;

    struct EntityDescription;
    struct TypeDescription;
    std::vector<EntityDescription> entities;
    std::vector<TypeDescription> types;
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
        static int componetTypeIdentifier = -1;

        // 1st time for this type
        if (componetTypeIdentifier == -1)
        {
            componetTypeIdentifier = globalComponetCounter++;
            ComponentPool pool = {AllocatePool<T>()};
            componentPools.push_back(pool);

            TypeDescription typeDescription = {sizeof(T)};
            types.push_back(typeDescription);
        }
        return componetTypeIdentifier;
    }

    struct EntityDescription
    {
        EntityID m_ID;
        ComponentMask m_Mask;
    };

    struct TypeDescription
    {
        size_t m_Size;
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

        entities[id].m_Mask.set(componentType);

        // get pool
        T* pool = (T*) componentPools[componentType].m_Pool.data();
        pool[id] = component;
    }

    void Push(EntityID id, uint componentType, void* obj)
    {
        assert(id < globalEntityCounter);
        assert(componentType < globalComponetCounter);

        entities[id].m_Mask.set(componentType);
        
        void* src = obj;
        unsigned char* dest = componentPools[componentType].m_Pool.data() + id*types[componentType].m_Size;
        memcpy(dest, src, types[componentType].m_Size);
    }

    void* Get(EntityID id, uint componentType)
    {
        assert(id < globalEntityCounter);
        assert(componentType < globalComponetCounter);
        unsigned char* dest = componentPools[componentType].m_Pool.data() + id*types[componentType].m_Size;
        return dest;
    }

    template <typename T>
    T& Get(EntityID id)
    {
        uint componentType = GetComponentType<T>();
        T* ptr = (T*) componentPools[componentType].m_Pool.data();
        return ptr[id];
    }

    template<typename... T>
    std::vector<EntityID> View()
    {
        std::vector<EntityID> view;
        uint componentTypes[] = { GetComponentType<T>() ... };
        std::bitset<32> mask = 0;
        for (uint i = 0; i < sizeof...(T); i++)
        {
            mask.set(componentTypes[i]);
        }

        for (uint i = 0; i < entities.size(); i++)
        {
            if (entities[i].m_Mask == mask)
            {
                view.push_back(i);
            }
        }
        return view;
    }
}
