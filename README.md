# Siafu

[![build](https://github.com/cjhoward/siafu/actions/workflows/build.yml/badge.svg)](https://github.com/cjhoward/siafu/actions/workflows/build.yml)

## Overview

Siafu is a tiny utility program for extracting isosurfaces from volumetric data. The program loads a 3D volume from a sequence of uncompressed TIFF files, extracts an isosurface using the marching cubes algorithm, and outputs a model in `.ply`, `.obj`, or `.stl` format. Siafu is written in C++23 with zero dependencies.

## Usage

```bash
siafu <volume_path> <isolevel> <output_file>
```

- `volume_path`: Directory containing a sequence of uncompressed TIFF files.

- `isolevel`: Threshold value for isosurface extraction.

- `output_file`: Name of the output file. If the file extension is not recognized, the `.ply` format will be used.

### Example Usage

The following command loads a volume from the `data` directory, extracts an isosurface at isolevel `500`, and saves the isosurface as `output.obj`:

```bash
siafu ./data 500 output.obj
```

## Building

1. Clone the repository:

   ```bash
   git clone https://github.com/cjhoward/siafu.git
   cd siafu
   ```

2. Configure the project with CMake:

   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```

3. Build the project:

   ```bash
   cmake --build build --config Release
   ```

The generated `siafu` executable will be located in the `build/bin` directory.

## License

Siafu is licensed under the MIT license. See [LICENSE.md](./LICENSE.md) for details.

## References

- Bourke, P. (1994). Polygonising a scalar field. <https://paulbourke.net/geometry/polygonise/>
