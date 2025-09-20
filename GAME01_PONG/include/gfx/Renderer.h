#pragma once

namespace Graphics 
{

/* The renderer class manages stuff until the flush operation. 
 * The persistence of data between frames is handled by the user, 
 * however its just about textures, the other stuff is recomputed each time.
 * The internal pipeline is: User sets up the renderer state with its public interface
 * Then he calls Renderer::Flush(). From now on the flow is passed to the Graphics module.
 * The module does the following: 
 * Sorts the data based on shader and texture, or just shader. Notice that when there is no texture
 * We make as if there an empty one? Its just a different shader... However being 2D stuff, everything has a texture anyway.
 * The SoA that represents the sequence of things to draw can be easily managed?
 * */


class Renderer {
public:
    Renderer();
    SetColor();
    Clear();
    Flush();
private:
};


} // Graphics
