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
#include <print>
#include <bit>

void write_ply(std::ostream& file, std::span<const vertex> vertices, std::span<const triangle> triangles)
{
	// Write header
	std::print
	(
		file,
		"ply\n"
		"format binary_{}_endian 1.0\n"
		"element vertex {}\n"
		"property float x\n"
		"property float y\n"
		"property float z\n"
		"property float nx\n"
		"property float ny\n"
		"property float nz\n"
		"element face {}\n"
		"property list uchar uint32 vertex_indices\n"
		"end_header\n",
		std::endian::native == std::endian::big ? "big" : "little",
		vertices.size(),
		triangles.size()
	);
	
	// Write vertex data
	if constexpr (sizeof(vertex) == sizeof(f32) * 6)
	{
		file.write(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(vertex));
	}
	else
	{
		for (const auto& v: vertices)
		{
			file.write(reinterpret_cast<const char*>(&v.p), sizeof(f32) * 3);
			file.write(reinterpret_cast<const char*>(&v.n), sizeof(f32) * 3);
		}
	}
	
	// Write face data
	for (const auto& t: triangles)
	{
		file.put(3);
		file.write(reinterpret_cast<const char*>(&t), sizeof(u32) * 3);
	}
}
