![Lotus Nightshade](lotus/assets/logo-nightshade.png)

![GitHub Stars](https://img.shields.io/github/stars/d34d0s/Lotus?style=flat&label=stars)  
![Nightshade Build](https://img.shields.io/github/actions/workflow/status/d34d0s/Lotus/nightshade.yaml?branch=nightshade)  
![License](https://img.shields.io/badge/license-apache%2Flibpng-green.svg)  

### 🌑 Welcome to **Lotus Nightshade** 🌑  
This is the **nightly** branch of Lotus, where developers build and explore **experimental features**, **engine architecture**, and **performance optimizations**.

> **Nightshade** is for developers who want to explore the engine; adding, removing, breaking, and fixing. Features developed on the `nightshade` branch are more than capable of making it onto `main`, and thus **pull-requests to `main` from this branch require a unit-test for the proposed code**.
<br>

> ## Build Schedule: **MON-FRI 12:00 AM EST**.

---

### 🌌 Relevant Links 🌌

| [Lotus Nightshade](https://github.com/d34d0s/Lotus/tree/nightshade) |  
| [Official Wiki](https://github.com/d34d0s/Lotus/wiki) | [API Documentation](https://github.com/d34d0s/Lotus/wiki/Lotus-API-Docs) | [Lotus Examples](https://github.com/d34d0s/Lotus/tree/nightshade/examples) | [CBuild](https://github.com/d34d0s/CBuild) |  
| [Official Discord](https://discord.gg/kreGBCVsQQ) | [Official Reddit](https://www.reddit.com/r/LotusEngine/) | [Lotus Discussions](https://github.com/d34d0s/Lotus/discussions) | [Lotus FAQ](https://github.com/d34d0s/Lotus/wiki/Lotus-FAQ) |  


## 🌌 Building the Engine Source 🌌  
(*For the skilled and daring.*)

> **NOTE**: If you're using the `CBuild` CLI tool, ensure **Python** is installed. For more information, check out the [CBuild Repository](https://github.com/d34d0s/CBuild).

Lotus includes **pre-configured `.cbuild` files** for easy setup and compilation. **[CBuild](https://github.com/d34d0s/CBuild)** is the recommended tool for fast, efficient builds.

---

### 🌑 Option 1: Build with **CBuild** (Recommended for Advanced Users)

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

### 🌌 Option 2: Manual Compilation (For the Brave)

For those who wish to **wield the tools** themselves:

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

## 🌌 Building Projects with **Lotus**

1. **Prepare Your Project**:  
   Set up your source files, including necessary paths.

2. **Compile with CBuild or GCC**:  
   Example using **GCC**:  
   ```bash
   gcc my_project.c -Ilotus/include -lLotus -o my_project.exe
   ```

<br>

## 🌑 Lotus Contributors 🌑

<a href="https://github.com/d34d0s/Lotus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=d34d0s/Lotus&max=500&columns=20&anon=1" />
</a>

<br>

## ⚖️ License

Lotus is licensed under the **Apache 2.0 License**.
