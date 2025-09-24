
# 20 Games Challenge - Part 1

The current folder hosts the first part of the 20 Games Challenge: Making the game Pong.

## Platform abstraction

The development start with a lot of scaffolding code.
I started by making a wrapper aroung GLFW and glad (the `Platform.h` header), which allows a nicer interaction with 
those libraries.

## Geometry

The other big thing i needed was math: vectors and matrices being the bare minimum.
I started implementing a vector with a generic storage class: `VecImpl<T, N>` that gives base storage to the vectors,
and then creating specialized versions of the `Vec<T, N>` class as childs.
This will allow me eradicate the naive std::array storage and incorporate intrinsics when performance requirements will lead me to do so.
I tipically implement the generic functions specializing per size, and often using the CRTP pattern.
This time, being in c++20 i tried to see if the concepts could help me.