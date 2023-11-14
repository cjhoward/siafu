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

void write_obj(std::ostream& file, std::span<const vertex> vertices, std::span<const triangle> triangles)
{
	for (const auto& v: vertices)
	{
		std::print(file, "v {} {} {}\n", v.p.x, v.p.y, v.p.z);
	}
	for (const auto& v: vertices)
	{
		std::print(file, "vn {} {} {}\n", v.n.x, v.n.y, v.n.z);
	}
	for (const auto& t: triangles)
	{
		std::print(file, "f {0}//{0} {1}//{1} {2}//{2}\n", t.a + 1, t.b + 1, t.c + 1);
	}
}