#ifndef RENDERSET_H
#define RENDERSET_H

#include "../SystemTypes/Typedefs.h"
#include "../Memory.h"

//TODO:
/*
- We will probably want to render crop fields in big batches
instead of drawing it sprite by sprite
A good approach will be to batch the crop sprites in bitmaps
and use those bitmaps as the texture. That will reduce the draw-calls
to one for crop-field, although we would not be able to reuse the same bitmap
for various fields.

- We will want to sort by texture too

- Roads will probably be batches

* * *
The general idea here is to make chunks of RenderingObjects arrays
Those chunks can then be conected if they grow out of range
It will probably be good if those chunks have some 'cache-friendly' size (L2 maybe)

The layers have different sizes depending on the usability
Empty RenderingObjects inside the set have zeroed VAOs and they index is added "availableSlots"
*/

typedef struct sRenderingObject {
    uint VAO;
    uint VBO;
    float x, y;
} RenderingObject;

typedef struct sRenderSet {
    int length;
    int lastSlot; //Point to last open space
    struct sRenderSet* nextSet; //If this get full, we'll allocate another one
    RenderingObject* rObjs;
} RenderSet;

typedef struct __sLinkedList {
    void* next;
    int setIndex; //Which set it is located
    int setPlace; //Index inside the set
} __LinkedList;

internal __LinkedList* availableSlots;

RenderSet** REN_CreateRenderSetLayers(int numLayers, int const sizes[]);
RenderingObject* REN_GetAvailableRenderingObject(RenderSet** layers, int layer);

#endif