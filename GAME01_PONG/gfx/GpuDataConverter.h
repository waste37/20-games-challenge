#pragma once


#include <algorithm>
#include <vector>
#include "SpriteStorage.h"
#include "RenderCommandQueue.h"

namespace gfx
{

template <typename MaxSprites>
class GpuDataConverter {
public:
    constexpr static size_t VerticesCount = MaxSprites * SpriteStorage::FloatsPerSprite();
    GpuDataConverter(SpriteStorage &sprites)
    {
        Convert(sprites);
    }

    std::vector<float> RenderData() noexcept { return m_RenderData; }
    RenderCommandQueue DrawingData() noexcept { return m_DrawingData; }

private:
    struct DataBucket {
        size_t Start;
        size_t Length;
    };

    void Convert(SpriteStorage &sprites) noexcept
    {
        auto buckets = GroupData(sprites);
        ProcessBuckets(buckets, sprites);
    }

    std::vector<DataBucket> GroupData(SpriteStorage &sprites)
    {
        std::vector<DataBucket> buckets{};
        std::sort(sprites.begin(), sprites.end());

        size_t bucket_beginning = 0;
        size_t bucket_length = 0;

        while (bucket_beginning + bucket_length < sprites.Size()) {
            if (SpriteStorage::InSameBucket(sprites[bucket_beginning], sprites[bucket_beginning + bucket_length + 1])) {
                bucket_length++;
            } else {
                buckets.push_back({ bucket_beginning, bucket_length });
                bucket_beginning = bucket_length;
                bucket_length = 0;
            }
        }

        return buckets;
    }

    void ProcessBuckets(std::vector<DataBucket> &&buckets, SpriteStorage &sprites)
    {
        size_t render_data_offset = 0;
        for (auto bucket : buckets) {
            std::vector<int> firsts;
            std::vector<GLsizei> counts{ bucket.Length, SpriteStorage::FloatsPerSprite() };
            std::vector<GLenum> modes;

            for (size_t i = bucket.Start; i < bucket.Length; ++i) {
                modes.append(sprites.Modes[i])
                    firsts.append(render_data_offset);
                render_data_offset = PushInterleavedData(render_data_offset, sprites.BBoxes[i]);
            }

            auto color = sprites.Colors[bucket.Start];
            auto draw_count = buckets.size();
            auto shader_id = -1;
            m_DrawingData.AddCommand(shader_id, projection, color, modes, firsts, counts, draw_count);
        }
    }

    constexpr size_t PushInterleavedRectData(size_t current_offset, geom::Bbox bbox)
    {
        m_RenderData[current_offset++] = bbox.Pos.x();                 m_RenderData[current_offset++] = bbox.Pos.y();                  // top-left
        m_RenderData[current_offset++] = bbox.Pos.x();                 m_RenderData[current_offset++] = bbox.Pos.y() + bbox.Size.y();  // bottom-left
        m_RenderData[current_offset++] = bbox.Pos.x() + bbox.Size.x(); m_RenderData[current_offset++] = bbox.Pos.y();                  // top-right
        m_RenderData[current_offset++] = bbox.Pos.x() + bbox.Size.x(); m_RenderData[current_offset++] = bbox.Pos.y() + bbox.Size.y();  // bottom-right
        return current_offset;
    }

    std::array<float, VerticesCount> m_RenderData;
    RenderCommandQueue m_DrawingData;
};

}
