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
#include <cmath>

namespace stl
{
	struct face
	{
		f32vec3 n;
		f32vec3 a;
		f32vec3 b;
		f32vec3 c;
		f32vec3 padding;
	};
}

void write_stl(std::ostream& file, std::span<const vertex> vertices, std::span<const triangle> triangles)
{
	// Write header
	char header[80] = {};
	file.write(header, sizeof(header));
	
	// Write triangle count
	u32 triangle_count = static_cast<u32>(triangles.size());
	if constexpr (std::endian::native != std::endian::little)
	{
		triangle_count = std::byteswap(triangle_count);
	}
	file.write(reinterpret_cast<const char*>(&triangle_count), sizeof(u32));
	
	// Write triangles
	stl::face f = {};
	for (const auto& t: triangles)
	{
		f.a = vertices[t.a].p;
		f.b = vertices[t.b].p;
		f.c = vertices[t.c].p;
		
		// Calculate faceted normal
		f.n.x = (f.b.y - f.a.y) * (f.c.z - f.a.z) - (f.b.z - f.a.z) * (f.c.y - f.a.y);
		f.n.y = (f.b.z - f.a.z) * (f.c.x - f.a.x) - (f.b.x - f.a.x) * (f.c.z - f.a.z);
		f.n.z = (f.b.x - f.a.x) * (f.c.y - f.a.y) - (f.b.y - f.a.y) * (f.c.x - f.a.x);
		const f32 sqr_l = f.n.x * f.n.x + f.n.y * f.n.y + f.n.z * f.n.z;
		const f32 inv_l = (sqr_l > 1e-6f) ? 1.0f / std::sqrt(sqr_l) : 0.0f;
		f.n = {f.n.x * inv_l, f.n.y * inv_l, f.n.z * inv_l};
		
		if constexpr (std::endian::native != std::endian::little)
		{
			f.n.x = byteswapf(f.n.x); f.n.y = byteswapf(f.n.y); f.n.z = byteswapf(f.n.z);
			f.a.x = byteswapf(f.a.x); f.a.y = byteswapf(f.a.y); f.a.z = byteswapf(f.a.z);
			f.b.x = byteswapf(f.b.x); f.b.y = byteswapf(f.b.y); f.b.z = byteswapf(f.b.z);
			f.c.x = byteswapf(f.c.x); f.c.y = byteswapf(f.c.y); f.c.z = byteswapf(f.c.z);
		}
		
		file.write(reinterpret_cast<const char*>(&f), 50);
	}
}
