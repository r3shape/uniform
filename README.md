![Lotus Logo](engine/assets/logo.png)

[![GitHub Stars](https://img.shields.io/github/stars/d34d0s/Lotus?style=flat&label=stars)](https://github.com/d34d0s/Lotus/stargazers)  
[![License](https://img.shields.io/badge/license-apache%2Flibpng-green.svg)](LICENSE)  

### Relevant Links

[ [Official Wiki](https://github.com/d34d0s/Lotus/wiki) | [API Documentation](https://github.com/d34d0s/Lotus/wiki/Lotus-API-Docs) | [Lotus Examples](https://github.com/d34d0s/Lotus/tree/main/examples) | [CBuild](https://github.com/d34d0s/CBuild) ]  
[ [Official Discord](https://discord.gg/kreGBCVsQQ) | [Official Reddit](https://www.reddit.com/r/LotusEngine/) | [Lotus Discussions](https://github.com/d34d0s/Lotus/discussions) | [Lotus FAQ](https://github.com/d34d0s/Lotus/wiki/Lotus-FAQ) ]  

<br>

## Building the Engine Source 🛠️
(**NOTE**): Using the `CBuild` CLI build-tool requires you to have python installed! More info can be found on the [CBuild Repository](https://github.com/d34d0s/CBuild)

Lotus now includes pre-configured `.cbuild` files for easy setup and compilation! Users are encouraged to use the **[CBuild](https://github.com/d34d0s/CBuild)** CLI tool, which is lightweight and tailored for projects like Lotus.


### Option 1: Build with CBuild (Recommended)

1. **Install CBuild**:  
   Install the tool via PyPI:
   ```bash
   pip install cbuild
   ```

2. **Clone the Repository**:  
   ```bash
   git clone https://github.com/d34d0s/Lotus.git
   cd Lotus
   ```

3. **Build the Engine**:  
   Use the provided `.cbuild` configuration to build Lotus:  
   ```bash
   cbuild scripts/lotus.cbuild
   ```

4. **Output**:  
   The compiled `Lotus.dll` will be located in the `build` directory.

<br>

### Option 2: Manual Compilation

If you prefer manual setup, follow these steps:

1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/d34d0s/Lotus.git
   cd Lotus
   ```

2. **Compile the Engine**:  
   Use GCC to build Lotus:  
   ```bash
   gcc -c lotus/modules/core/src/*.c -Ilotus/include -DLOTUS_DLL_EXPORT -o build/*.o
   gcc -shared build/*.o -lopengl32 -lgdi32 -o build/Lotus.dll
   ```

3. **Install the Compiled Binaries**:  
   Move the compiled files to your project's directories as needed:  
   ```bash
   cp build/Lotus.dll your_project/bin/
   ```

<br>

## Building Projects with Lotus

1. **Prepare Your Project**:  
   Set up your source files and include paths.

2. **Compile with CBuild or GCC**:  
   - Example using GCC:  
     ```bash
     gcc my_project.c -Ilotus/include -lLotus -o my_project.exe
     ```

<br>

## 🪷 Lotus' Contributors 🪷  

<a href="https://github.com/d34d0s/Lotus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=d34d0s/Lotus&max=500&columns=20&anon=1" />
</a>

<br>

## 🪷 License 🪷  

Lotus is licensed under the Apache 2.0 license.  

<br>