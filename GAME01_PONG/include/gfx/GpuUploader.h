#pragma once 

namespace Graphics
{

/* Aggregates the data required for a single batch call */
class RenderBucket {
};

/* Manages GPU buffers and loads data into them */
class GpuUploader {
public:
    GpuUploader(int reserved_bytes_count);
    void Upload(RenderBucket &&bucket);
};

} // Graphics
