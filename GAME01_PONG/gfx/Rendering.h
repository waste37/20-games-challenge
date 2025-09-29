
namespace Gfx
{

/*
 *
 * 1. game produces stuff to draw and uses renderer apis to communicate it.
 * the renderer then stores this stuff.
 *
 * 2. it then organizes all the data in the most efficient way
 * that is the way that minimizes driver changes i think ( binding of the various stuff)
 * Its pretty much a sorting based on shader then texture.
 *
 * 3. convert all the data in a GPU friendly format
 *
 * 4. upload it to GPU
 *
 * 5. calls the draw commands needed
 *
 * 6. delete all?
 *
 */

 // public module APIs
class Renderer2D {
public:
    Renderer2D(size_t MaxSprites);
    void SetRenderTarget();
    void SetColor();
    void Clear();
    void Flush();

    void DrawRect();
    void FillRect();
    void DrawLine();
    void DrawPoint();
    void RenderTexture();
private:
    // this thing allows to:
    // Create the buffers when created.
    // Put all the data into them when needed 
    GpuUploader uploader;
    CommandExecutor executor;
};

// Calls to the Renderer would create the data used for then doing buckets.
// Each bucket is then put into interleaved format for the VBO, EBO etc.
// After this the CommandExecutor runs all the  


RenderCommandQueue *TransformGameDataAndLoadGPUBuffers(GpuUploader *uploader, renderer.GetData());

void Renderer::Flush()
{
    GpuUploader loader;
    auto command_queue = TransformGameDataAndLoadGPUBuffers(loader, m_Data);
    DrawExecutor executor{ command_queue };
    executor.Draw();
}

using GpuVertex = float[8]; // pos, uv, rgba

struct RenderBucketData {
    GpuVertex *vertices;
    size_t vertex_count;
    int *indices;
    size_t index_count;
};

class RenderBucket {
    GpuVertex *vertices;
    size_t vertex_count;
    int *indices;
    size_t index_count;
    int texture;
    int shader;
};

// uploads data on gpu
class GpuUploader {
public:
    GpuUploader(size_t reserved_bytes_count);
    void UploadBucket(RenderBucket &&bucket);
};

// list of draw commands in SoA format
class RenderCommandQueue {
public:
    class BucketRenderParameters {
    public:
        BucketRenderParameters();
        AddParameter(const GLsizei count, const void *index);
        constexpr GLsizei Count() const noexcept;
        constexpr GLsizei DrawCount() const noexcept;
    private:
        std::vector<GLsizei> Count;
        std::vector<void const *> Indices;
        GLenum Mode;
        GLenum Type;
    };

    RenderCommandQueue(size_t length);
    void PushCommand(int texture, int shader, BucketRenderParameters &&parameters);
    constexpr size_t Size() noexcept;
    constexpr GLint GetTexture(size_t i) const noexcept;
    constexpr GLint GetShader(size_t i) const noexcept;
    constexpr const MultiRenderParameter &GetParameters(size_t i) const noexcept;
private:
    std::vector<GLint> m_BoundTexture;
    std::vector<GLint> m_BoundShader;
    std::vector<RenderParameterBucket >> m_CallParameters;
    // let's say that the queue could be made of a sequence of states, each determined by: 
    // - render target (can only be default or framebuffer with and attached texture)
    // - attached texture (1 at the time)
    // - in use shader
    // - arrays of drawElements parameters (so that if i have to dispatch multiple draws per bucket i can have them)
};

// executes DrawQueue
class RenderCommandExecutor {
    CommandExecutor(RenderCommandQeue &&queue);
    void Execute()
    {
        for (size_t i = 0; i < queue.Size(); ++i) {
            auto texture = queue.GetTexture(i);
            auto shader = queue.GetShader(i);
            auto params = queue.GetParameters(i);

            glBindTexture(texture, 0);
            glBindShader(queue.GetTexture(), 0);
            glMultiDrawElements(params.Mode, params.c)
        }
    }
private:
    RenderCommandQueue queue;
};

}


