#pragma once

namespace cubedemo
{
    // Implements a bloom effect
    // For this, there are 5 render passes:
    // 1 - Render the scene to a HDR texture
    // 2 - Run a bright-pass filter over the HDR texture from step 1 and downsample
    // 3 + 4 - Apply a gaussian blur to the downsampled texture
    // 5 - Apply tone mapping to the original HDR texture, and add the blurred bright-pass filter results
    class BloomEffect
    {
    private:

    public:
        BloomEffect();
        ~BloomEffect();

        void onWindowSizeChanged(size_t width, size_t height);

        void renderPass1();
    };
}
