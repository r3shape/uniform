![Lotus](lotus/assets/logo.png)

![GitHub Stars](https://img.shields.io/github/stars/d34d0s/Lotus?style=flat&label=stars)  
![License](https://img.shields.io/badge/license-apache%2Flibpng-green.svg)  

---

### 🪷 Relevant Links 🪷

|[CBuild](https://github.com/d34d0s/CBuild) |  
| [Module Reference](https://github.com/d34d0s/Lotus/blob/main/docs/module_ref/module_ref.md) | [API Reference](https://github.com/d34d0s/Lotus/blob/main/docs/api_ref/api_ref.md) | [Example Guides](https://github.com/d34d0s/Lotus/blob/main/docs/examples/examples.md) |  [Example Code](https://github.com/d34d0s/Lotus/tree/main/examples) |   
| [Official Discord](https://discord.gg/kreGBCVsQQ) | [Official Reddit](https://www.reddit.com/r/LotusEngine/) | [Lotus Discussions](https://github.com/d34d0s/Lotus/discussions) | [Lotus FAQ](https://github.com/d34d0s/Lotus/wiki/Lotus-FAQ) |  


## 🪷 Building the Engine Source 🪷  

> **NOTE**: If you're using the `CBuild` CLI tool, ensure **Python** is installed. For more information, check out the [CBuild Repository](https://github.com/d34d0s/CBuild).

Lotus includes **pre-configured `.cbuild` files** for easy setup and compilation. **[CBuild](https://github.com/d34d0s/CBuild)** is the recommended tool for fast, efficient builds.

---

### 🪷 Option 1: Build with **CBuild** (Recommended)

1. **Install CBuild**:  
   Install via **PyPI**:  
   ```bash
   pip install cbuild
   ```

2. **Clone the Repository**:  
   ```bash
   git clone https://github.com/d34d0s/Lotus.git
   cd Lotus
   ```

3. **Build the Engine**:  
   ```bash
   cbuild scripts/lotus.cbuild
   ```

4. **Output**:  
   The compiled `Lotus.dll` will be located in the `build` directory.

---

### 🪷 Option 2: Manual Compilation

For those who wish to **wield the source** themselves:

1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/d34d0s/Lotus.git
   cd Lotus
   ```

2. **Compile the Engine**:  
   Manually compile using **GCC**:  
   ```bash
   gcc -c lotus/modules/core/src/*.c -Ilotus/include -DLOTUS_DLL_EXPORT -o build/*.o
   gcc -shared build/*.o -lopengl32 -lgdi32 -o build/Lotus.dll
   ```

3. **Install Compiled Binaries**:  
   ```bash
   cp build/Lotus.dll your_project/bin/
   ```

<br>

## 🪷 Building Projects with **Lotus**

1. **Prepare Your Project**:  
   Set up your source files, including necessary paths.

2. **Compile with CBuild or GCC**:  
   Example using **GCC**:  
   ```bash
   gcc my_project.c -Ilotus/include -lLotus -o my_project.exe
   ```

<br>

## 🪷 Lotus Contributors 🪷

<a href="https://github.com/d34d0s/Lotus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=d34d0s/Lotus&max=500&columns=20&anon=1" />
</a>

<br>

## ⚖️ License

Lotus is licensed under the **Apache 2.0 License**.
