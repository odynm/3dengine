#include "RenderSet.h"

RenderSet** REN_CreateRenderSetLayers(int numLayers, int const sizes[])
{
    RenderSet** layers = (RenderSet**)MEM_Alloc(sizeof(RenderSet*) * numLayers);
    for (int i = 0; i < numLayers; i++)
    {
        RenderSet* set = (RenderSet*)MEM_Alloc(sizeof(RenderSet) * sizes[i]);
        set->length = sizes[i];
        set->rObjs = (RenderingObject*)MEM_Alloc(sizeof(RenderingObject) * set->length);

        layers[i] = set;
    }

    return layers;
}

RenderingObject* REN_GetAvailableRenderingObject(RenderSet** layers, int layer)
{
    return &layers[layer]->rObjs[layers[layer]->lastSlot];
}