/*
 * Copyright (c) 2023 C. J. Howard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "siafu.hpp"
#include <fstream>
#include <stdexcept>
#include <ranges>
#include <execution>
#include <print>

namespace tiff
{
	/// TIFF header.
	struct header
	{
		u16 byte_order;
		u16 magic;
		u32 ifd_offset;
	};
	
	/// TIFF image file directory (IFD) entry.
	struct ifd_entry
	{
		u16 tag;
		u16 type;
		u32 count;
		u32 offset;
	};
	
	/// TIFF header constants. @{
	inline constexpr u16 little_endian = 0x4949;
	inline constexpr u16 big_endian = 0x4d4d;
	inline constexpr u16 magic = 42;
	/// @}
	
	/// TIFF IFD entry constants. @{
	inline constexpr u16 image_width = 0x0100;
	inline constexpr u16 image_height = 0x0101;
	inline constexpr u16 bits_per_sample = 0x0102;
	inline constexpr u16 compression = 0x0103;
	inline constexpr u16 strip_offsets = 0x0111;
	inline constexpr u16 samples_per_pixel = 0x0115;
	inline constexpr u16 rows_per_strip = 0x0116;
	inline constexpr u16 strip_byte_counts = 0x0117;
	inline constexpr u16 x_resolution = 0x011a;
	inline constexpr u16 y_resolution = 0x011b;
	inline constexpr u16 planar_config = 0x011c;
	inline constexpr u16 resolution_unit = 0x0128;
	inline constexpr u16 uncompressed = 1;
	/// @}
	
	/// Returns a sequence of TIFF files in a directory.
	[[nodiscard]] std::vector<fs::path> find_files(const fs::path& path)
	{
		std::vector<fs::path> files;
		
		if (fs::exists(path))
		{
			fs::path dir = fs::is_directory(path) ? path : path.has_parent_path() ? path.parent_path() : fs::current_path();
			
			for (const auto& entry : fs::directory_iterator(dir))
			{
				if (fs::is_regular_file(entry) && (entry.path().extension() == ".tif" || entry.path().extension() == ".tiff"))
				{
					files.push_back(entry.path());
				}
			}
			
			std::sort(files.begin(), files.end());
		}
		
		return files;
	}
}

std::vector<std::byte> load_volume(const fs::path& path, u32& width, u32& height, u32& depth, u32& bits_per_voxel)
{
	const auto files = tiff::find_files(path);
	if (files.empty())
	{
		throw std::runtime_error("file not found");
	}
	
	// Open first TIFF file in the sequence
	std::ifstream file(files.front(), std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file");
	}
	
	// Read the TIFF header
	tiff::header header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	
	// Check byte order
	if (header.byte_order != tiff::little_endian && header.byte_order != tiff::big_endian)
	{
		throw std::runtime_error("unsupported byte order");
	}
	
	bool native_endian = true;
	if ((std::endian::native == std::endian::little) == (header.byte_order == tiff::big_endian))
	{
		native_endian = false;
		header.magic = std::byteswap(header.magic);
		header.ifd_offset = std::byteswap(header.ifd_offset);
	}

	// Check magic number
	if (header.magic != tiff::magic)
	{
		throw std::runtime_error("invalid magic number");
	}
	
	// Set the file position to the IFD offset
	file.seekg(header.ifd_offset, std::ios::beg);

	// Read IFD entry count
	u16 ifd_entry_count;
	file.read(reinterpret_cast<char*>(&ifd_entry_count), sizeof(u16));
	if (!native_endian)
	{
		ifd_entry_count = std::byteswap(ifd_entry_count);
	}
	
	// Read IFD entries
	std::vector<tiff::ifd_entry> entries(ifd_entry_count);
	file.read(reinterpret_cast<char*>(entries.data()), entries.size() * sizeof(tiff::ifd_entry));
	if (!native_endian)
	{
		for (auto& entry: entries)
		{
			entry.tag = std::byteswap(entry.tag);
			entry.type = std::byteswap(entry.type);
			entry.count = std::byteswap(entry.count);
			entry.offset = std::byteswap(entry.offset);
		}
	}
	
	width = 0;
	height = 0;
	depth = static_cast<u32>(files.size());
	bits_per_voxel = 0; 
	u32 compression = tiff::uncompressed;
	u32 strips_offset = sizeof(header);
	
	// Process entries
	for (const auto& entry: entries)
	{
		switch (entry.tag)
		{
			case tiff::image_width:
				width = entry.offset;
				break;
			case tiff::image_height:
				height = entry.offset;
				break;
			case tiff::bits_per_sample:
				bits_per_voxel = entry.offset;
				break;
			case tiff::compression:
				compression = entry.offset;
				break;
			case tiff::strip_offsets:
				if (entry.count == 1)
				{
					strips_offset = entry.offset;
				}
				else
				{
					file.seekg(entry.offset, std::ios::beg);
					file.read(reinterpret_cast<char*>(&strips_offset), sizeof(u32));
				}
				break;
			default:
				break;
		}
	}
	
	if (!width || !height)
	{
		throw std::runtime_error("image has invalid dimensions");
	}
	if (compression != tiff::uncompressed)
	{
		throw std::runtime_error("compressed images not supported");
	}
	
	const auto bytes_per_voxel = bits_per_voxel >> 3;
	std::size_t slice_size = width * height * bytes_per_voxel;
	
	// Allocate voxels
	std::vector<std::byte> voxels(slice_size * depth);
	
	// Load first Z-slice
	char* slice_data = reinterpret_cast<char*>(voxels.data());
	file.seekg(strips_offset, std::ios::beg);
	file.read(slice_data, slice_size);
	file.close();
	
	// Load remaining Z-slices in parallel
	const std::ranges::iota_view file_indices(std::size_t{1}, files.size());
	std::for_each
	(
		std::execution::par_unseq,
		file_indices.begin(),
		file_indices.end(),
		[&](std::size_t i)
		{
			std::ifstream file(files[i], std::ios::binary);
			if (!file.is_open())
			{
				throw std::runtime_error("failed to open file");
			}
			
			file.seekg(strips_offset, std::ios::beg);
			file.read(slice_data + slice_size * i, slice_size);
		}
	);
	
	if (!native_endian && bytes_per_voxel > 1)
	{
		for (std::size_t i = 0; i < voxels.size(); i += bytes_per_voxel)
		{
			std::reverse(voxels.begin() + i, voxels.begin() + i + bytes_per_voxel);
		}
	}
	
	return voxels;
}
