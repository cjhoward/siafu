# Siafu

## Overview

Siafu is a tiny utility program for extracting isosurfaces from volumetric data. The program loads volumes from sequences of uncompressed TIFF files, and outputs models in `.ply`, `.obj`, or `.stl` formats. Siafu is written in C++23 and has no external dependencies.

## Usage

```bash
siafu <volume_path> <isolevel> <output_file>
```

- **volume_path**: Directory containing a sequence of uncompressed TIFF files.

- **isolevel**: Threshold value for isosurface extraction.

- **output_file**: Name of the output file. If the file extension is not recognized, the `.ply` format will be used.

### Example Usage

```bash
# Load volume from `data` directory, extract isosurface at isolevel `500`, and save as `output.obj`
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
   cmake -B build
   ```

3. Build the project:

   ```bash
   cmake --build build
   ```

The generated `siafu` executable will be located in the `build/bin` directory.

## License

Siafu is licensed under the MIT license. See [LICENSE.md](./LICENSE.md) for details.

## References

- Bourke, P. (1994). Polygonising a scalar field. <https://paulbourke.net/geometry/polygonise/>
