// SPDX-FileCopyrightText: 2023 C. J. Howard
// SPDX-License-Identifier: MIT

#include "siafu.hpp"
#include <print>

void write_obj(std::ostream& file, std::span<const vertex> vertices, std::span<const triangle> triangles)
{
	for (const auto& v: vertices)
	{
		file << std::format("v {} {} {}\n", v.p.x, v.p.y, v.p.z);
	}
	for (const auto& v: vertices)
	{
		file << std::format("vn {} {} {}\n", v.n.x, v.n.y, v.n.z);
	}
	for (const auto& t: triangles)
	{
		file << std::format("f {0}//{0} {1}//{1} {2}//{2}\n", t.a + 1, t.b + 1, t.c + 1);
	}
}
