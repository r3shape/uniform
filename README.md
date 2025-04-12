![koncept](koncept/assets/r3-sticker.png)

![GitHub Stars](https://img.shields.io/github/stars/r3shape/koncept?style=for-the-badge&label=stars&labelColor=black&color=white)
![License](https://img.shields.io/badge/mit-badge?style=for-the-badge&logo=mit&logoColor=white&label=License&labelColor=black&color=white)

---

### Relevant Links
| [Discord](https://discord.gg/kreGBCVsQQ) | [Reddit](https://www.reddit.com/r/r3shape/) | [Discussions](https://github.com/r3shape/koncept/discussions) | [FAQ](https://github.com/r3shape/koncept/wiki/koncept-FAQ) | [Example Code](https://github.com/r3shape/koncept/tree/main/examples) |  


## Building the Engine Source

> **NOTE**: If you're using the `r3make` CLI tool, ensure **Python** is installed. For more information, check out the [r3make Repository](https://github.com/r3shape/r3make).

koncept includes **pre-configured `.r3make` files** for easy setup and compilation. **[r3make](https://github.com/r3shape/r3make)** is the recommended tool for fast, simple, efficient builds.

---

### Option 1: Build with **r3make** (Recommended)

1. **Install r3make**:  
   Install via **PyPI**:  
   ```bash
   pip install r3make
   ```

2. **Clone the Repository**:  
   ```bash
   git clone https://github.com/r3shape/koncept
   ```

3. **Navigate to the `.r3make` directory:
   ```bash
   cd koncept
   ```

4. **Build the Engine**:  
   - Target: `koncept` builds the `koncept.dll` from `koncept.core`
   ```bash
   r3make koncept
   ```
5. **Build the Engine Demos**:  
   - Target: `examples` builds the `examples` directory into corresponding `exe` files in the same output directory as the `koncept` build target.
   ```bash
   r3make examples
   ```

5. **Output**:  
   The compiled `koncept.dll`, along with example exe files will be located in the `build` directory.

---

### Option 2: Manual Compilation

For those who wish to **wield the source** themselves:

1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/r3shape/koncept
   cd koncept
   ```

2. **Compile the Engine**:  
   Manually compile using **GCC**:  
   ```bash
   gcc -c koncept/koncept.core/src/*.c -Iengine/include -o bin/ofiles/*.o
   gcc -shared bin/ofiles/*.o -lopengl32 -lgdi32 -llibx -Lexternal/libx/bin -o build/koncept.dll
   ```

3. **Install Compiled Binaries**:  
   ```bash
   cp bin/koncept.dll your_project/bin/
   ```

<br>

## Building Projects with **koncept**

1. **Prepare Your Project**:  
   Set up your source files, including necessary paths.

2. **Compile with r3make or GCC**:  
   Example using **GCC**:  
   ```bash
   gcc my_project.c -Iengine/include -lr3 -o my_project.exe
   ```

<br>

## koncept Contributors

<a href="https://github.com/r3shape/koncept/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=r3shape/koncept"/>
</a>

<br>

## License

koncept is licensed under the **MIT License**.
