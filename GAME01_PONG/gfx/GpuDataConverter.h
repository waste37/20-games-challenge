#pragma once


#include <iostream>
#include <algorithm>
#include <vector>
#include "SpriteStorage.h"
#include "RenderCommandQueue.h"

namespace gfx
{

template <size_t MaxSprites>
class GpuDataConverter {
public:
    constexpr static size_t VertexCount = MaxSprites * SpriteStorage<MaxSprites>::FloatsPerSprite();
    GpuDataConverter(SpriteStorage<MaxSprites>& sprites)
    {
        if (sprites.Size() == 0) return;
        Convert(sprites);
    }

    std::array<float, VertexCount> VertexData() noexcept { return m_RenderData; }
    RenderCommandQueue DrawingData() noexcept { return m_DrawingData; }

private:
    struct DataBucket {
        size_t Start;
        size_t Length;
    };

    void Convert(SpriteStorage<MaxSprites>& sprites) noexcept
    {
        auto buckets = GroupData(sprites);
        ProcessBuckets(buckets, sprites);
    }

    std::vector<DataBucket> GroupData(SpriteStorage<MaxSprites>& sprites)
    {
        sprites.Sort();

        std::vector<DataBucket> buckets{};
        size_t bucket_beginning = 0;
        size_t bucket_length = 0;

        while (bucket_beginning + bucket_length < sprites.Size() - 1) {
            if (sprites.InSameBucket(bucket_beginning, bucket_beginning + bucket_length + 1)) {
                bucket_length++;
                //std::cout << "in same bucket\n";
            } else {
                buckets.push_back({ bucket_beginning, bucket_length + 1 });
                bucket_beginning = bucket_beginning + bucket_length + 1;
                bucket_length = 0;
            }
        }

        buckets.push_back({ bucket_beginning, bucket_length + 1 });

        return buckets;
    }

    void ProcessBuckets(const std::vector<DataBucket>& buckets, SpriteStorage<MaxSprites>& sprites)
    {
        //for (const auto& bucket : buckets) {
        //    std::cout << "bucket" << bucket.Start << ", " << bucket.Length << std::endl;
        //}
        int render_data_offset = 0;
        for (const auto& bucket : buckets) {
            auto type = sprites.Type(bucket.Start);
            GLenum mode;
            if (type == SpriteType::TexturedRect) {
                mode = GL_TRIANGLE_STRIP;
            }

            auto texture = sprites.Texture(bucket.Start);
            auto draw_count = static_cast<GLsizei>(bucket.Length);

            std::vector<int> firsts;
            std::vector<GLsizei> counts(bucket.Length, (GLsizei)SpriteStorage<MaxSprites>::VerticesPerSprite());
            for (size_t i = bucket.Start; i < bucket.Start + bucket.Length; ++i) {
                firsts.push_back(static_cast<int>(i) * static_cast<int>(SpriteStorage<MaxSprites>::VerticesPerSprite()));
                //std::cout << "command" << i << "first " << firsts.back() << std::endl;
                if (sprites.Type(i) == gfx::SpriteType::TexturedRect) {
                    render_data_offset = PushInterleavedTexturedRect(render_data_offset, sprites.Bbox(i), sprites.Depth(i));
                }
            }
            m_DrawingData.Push(RenderCommand{ texture, mode, firsts, counts, draw_count });
        }

        //for (int i = 0; i < render_data_offset; ++i) {
        //    std::cout << m_RenderData[i] << " ";
        //    if (i % 3 == 2) std::cout << std::endl;
        //}
        //exit(0);
    }

    constexpr int PushInterleavedTexturedRect(int current_offset, math::Bbox bbox, float depth)
    {
        // top-left
        m_RenderData[current_offset++] = bbox.Pos.x();
        m_RenderData[current_offset++] = bbox.Pos.y();
        m_RenderData[current_offset++] = depth;
        m_RenderData[current_offset++] = 0.0f;
        m_RenderData[current_offset++] = 0.0f;

        // bottom-left
        m_RenderData[current_offset++] = bbox.Pos.x();
        m_RenderData[current_offset++] = bbox.Pos.y() + bbox.Size.y();
        m_RenderData[current_offset++] = depth;
        m_RenderData[current_offset++] = 0.0f;
        m_RenderData[current_offset++] = 1.0f;

        // top-right
        m_RenderData[current_offset++] = bbox.Pos.x() + bbox.Size.x();
        m_RenderData[current_offset++] = bbox.Pos.y();
        m_RenderData[current_offset++] = depth;
        m_RenderData[current_offset++] = 1.0f;
        m_RenderData[current_offset++] = 0.0f;

        // bottom-right
        m_RenderData[current_offset++] = bbox.Pos.x() + bbox.Size.x();
        m_RenderData[current_offset++] = bbox.Pos.y() + bbox.Size.y();
        m_RenderData[current_offset++] = depth;
        m_RenderData[current_offset++] = 1.0f;
        m_RenderData[current_offset++] = 1.0f;

        return current_offset;
    }

    std::array<float, VertexCount> m_RenderData;
    RenderCommandQueue m_DrawingData;
};

}
