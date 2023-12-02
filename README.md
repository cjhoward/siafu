<!--
SPDX-FileCopyrightText: 2023 C. J. Howard
SPDX-License-Identifier: CC0-1.0
-->

<div align="center">

# Siafu

[![build](https://github.com/cjhoward/siafu/actions/workflows/build.yml/badge.svg)](https://github.com/cjhoward/siafu/actions/workflows/build.yml)
[![code quality](https://app.codacy.com/project/badge/Grade/23dc62d0303f4d20a8f15ec8d6a1eea2)](https://app.codacy.com/gh/cjhoward/siafu/dashboard)

</div>

Siafu is a tiny utility program for extracting isosurfaces from volumetric data. The program loads a 3D volume from a sequence of uncompressed TIFF files, extracts an isosurface using the marching cubes algorithm, and outputs a model in `.ply`, `.obj`, or `.stl` format. Siafu is written in C++23 with zero dependencies.

## Table of Contents

- [Usage](#usage)
- [Building](#building)
- [Maintainers](#maintainers)
- [License](#license)
- [References](#references)

## Install

```bash
git clone https://github.com/cjhoward/siafu.git && cd siafu
cmake -B build
cmake --build build --config Release --target install
```

## Usage

```bash
siafu <volume_path> <isolevel> <output_file>
```

- `volume_path`: Directory containing a sequence of uncompressed TIFF files.
- `isolevel`: Threshold value for isosurface extraction.
- `output_file`: Name of the output file. If the file extension is not recognized, the `.ply` format will be used.

### Examples

Load a volume from the `data` directory, extract an isosurface at isolevel `500`, and save the isosurface as `output.obj`:

```bash
siafu ./data 500 output.obj
```

## Maintainers

- [C. J. Howard](https://github.com/cjhoward)

## Contributing

Contributions are welcome! Feel free to [open an issue](https://github.com/cjhoward/siafu/issues) or [submit a pull request](https://github.com/cjhoward/siafu/pulls).

## License

[![REUSE compliance](https://github.com/cjhoward/siafu/actions/workflows/reuse.yml/badge.svg)](https://github.com/cjhoward/siafu/actions/workflows/reuse.yml)

- Siafu source code is licensed under [MIT](./LICENSES/MIT.txt).
- Siafu documentation is licensed under [CC0-1.0](./LICENSES/CC0-1.0.txt).

## References

- Bourke, P. (1994). Polygonising a scalar field. <https://paulbourke.net/geometry/polygonise/>
