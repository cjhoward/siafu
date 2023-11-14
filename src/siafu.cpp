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
#include <iostream>
#include <print>
#include <stdexcept>

int main(int argc, char* argv[])
{
	auto usage = [](){std::println("usage: <volume_path> <isolevel> <output_file>");};
	
	if (argc != 4)
	{
		usage();
		return 1;
	}
	
	char* endptr;
	f32 isolevel = std::strtof(argv[2], &endptr);
	if (*endptr != '\0')
	{
		usage();
		return 1;
	}
	
	// Load volume
	u32 volume_w, volume_h, volume_d, bits_per_voxel;
	std::vector<std::byte> voxels;
	try
	{
		voxels = load_volume(argv[1], volume_w, volume_h, volume_d, bits_per_voxel);
	}
	catch (const std::exception& e)
	{
		std::println(std::cerr, "failed to load volume: {}", e.what());
		return 1;
	}
	
	// Select sampling function
	std::function<f32(u32, u32, u32)> sample;
	if (bits_per_voxel == 8)
	{
		sample = [&, vu8 = reinterpret_cast<const u8*>(voxels.data())](u32 x, u32 y, u32 z) -> f32
		{
			return vu8[x + volume_w * (y + volume_h * z)];		
		};
	}
	else if (bits_per_voxel == 16)
	{
		sample = [&, vu16 = reinterpret_cast<const u16*>(voxels.data())](u32 x, u32 y, u32 z) -> f32
		{
			return vu16[x + volume_w * (y + volume_h * z)];		
		};
	}
	
	// Extract isosurface
	std::vector<vertex> vertices;
	std::vector<triangle> triangles;
	polygonize(isolevel, sample, volume_w, volume_h, volume_d, vertices, triangles);
	
	// Open output file
	fs::path file_path(argv[3]);
	std::ofstream file(file_path, std::ios::binary);
	if (!file.is_open())
	{
		std::println(std::cerr, "failed to open output file");
		return 1;
	}
	
	// Output isosurface
	if (file_path.extension() == ".obj")
	{
		write_obj(file, vertices, triangles);
	}
	else if (file_path.extension() == ".stl")
	{
		write_stl(file, vertices, triangles);
	}
	else
	{
		write_ply(file, vertices, triangles);
	}
	
	return 0;
}
