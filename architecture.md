# The planned architecure:
## Game loop
The game loop is handled by the user of the API. A simple while should suffice.

## Core architecture
Main systems who made the core of the engine are all singletons / static. The engine user can use those through importing *Engine.h*, which should bring everything needed with it - pretty much like Unreal used to do with *Unreal.h*. 

## Entities and components
The components make a differente part of the engine and are not static. Those can be instanced and used through the code, and probably need to be imported separately together with the system they eventually use.

## Lua API
Planned.
