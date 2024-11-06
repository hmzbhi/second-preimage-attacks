# TP2 Crypo_Eng

The goal of this project is to implement a generic second preimage attack for long messages for Merkle-Damgård hash functions, in the specific case where the compression function used within the hash function follows a so-called Davies-Meyer construction. This is a particular case of the attacks (re-)designed by Kelsey and Schneier.

## Directory Structure

- **`src/`**: Contains source code
- **`test/`**: Contains test files to validate the implementation.
- **`tools/`**: Contains additional tools and helper functions.
- **`doc/`**: Documentation files including the subject of the project and the specifications.
- **`Makefile`**: Script to compile the project and run tests.
- **`README.md`**: This file.
- **`rendu.md`**: Summary or notes related to the project.

## Requirements

- **Compiler**: `gcc`
- **Flags**: The project uses `-Wall` and `-Wextra` flags for additional warnings.

## Compilation Instructions

To compile the project and build all tests:

```bash
make
```

To run all test executables:

```bash
make test
```

To remove all compiled object files and executables:

```bash
make clean
```
