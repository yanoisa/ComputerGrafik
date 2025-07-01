#include "texture_loader.hpp"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <iostream> // For debug output

// request supported types
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STBI_NO_LINEAR
// get extensive error on failure
#define STBI_FAILURE_USERMSG
// create implementation here
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace texture_loader {
    pixel_data file(std::string const& file_name) {
        stbi_set_flip_vertically_on_load(true);

        uint8_t* data_ptr;
        int width = 0;
        int height = 0;
        int original_channels = 0; // This will store the *original* number of channels in the file

        // We explicitly request STBI_rgb_alpha (4 channels)
        // If the file has fewer channels (e.g., RGB), stb_image will convert it to RGBA.
        // If the file has more (unlikely for JPG/PNG), it would still load as RGBA.
        int desired_channels_count = STBI_rgb_alpha;
        data_ptr = stbi_load(file_name.c_str(), &width, &height, &original_channels, desired_channels_count);

        if (!data_ptr) {
            throw std::logic_error(std::string{ "stb_image: " } + stbi_failure_reason());
        }

        // DEBUG OUTPUT:
        std::cout << "Loading " << file_name << ": "
            << "Width=" << width << ", Height=" << height
            << ", Original Channels=" << original_channels
            << ", Loaded as=" << desired_channels_count << " channels (RGBA always)" << std::endl;


        // The pixel_format and num_components should reflect what we *requested* from stb_image,
        // not what the original file contained, because stb_image has already converted it.
        GLenum pixel_format = GL_RGBA; // We explicitly requested RGBA from stbi_load
        std::size_t num_components = 4; // Always 4 components (RGBA) as requested

        std::vector<uint8_t> texture_data(width * height * num_components);
        // Copy all the loaded data (which is guaranteed to be num_components per pixel)
        std::memcpy(texture_data.data(), data_ptr, texture_data.size()); // Use .data() for modern C++
        stbi_image_free(data_ptr);

        // Return the pixel_data struct with the correct format and component count
        return pixel_data{ texture_data, pixel_format, GL_UNSIGNED_BYTE, std::size_t(width), std::size_t(height) };
    }

} // namespace texture_loader