// SPDX-FileCopyrightText: 2023 C. J. Howard
// SPDX-License-Identifier: MIT

#include "siafu.hpp"
#include <bit>
#include <format>

void write_ply(std::ostream& file, std::span<const vertex> vertices, std::span<const triangle> triangles)
{
	// Write header
	file << std::format
	(
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
