#ifndef MATH_H
#define MATH_H

/// Index access defines, probably not needed once we have vectors
#define i2(r,c) (r*2 + c)
#define i3(r,c) (r*3 + c)
#define i4(r,c) (r*4 + c)

typedef struct SVector2 {
    float x, y;
} Vector2;

typedef struct SVector3 {
    float x, y, z;
} Vector2;

typedef struct SVector4 {
    float x, y, z, w;
} Vector2;

#endif