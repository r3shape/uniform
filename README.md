![banner3x](https://github.com/user-attachments/assets/414cf8b5-ae3f-4469-bbf1-59ecae33bd28)

[![GitHub Releases Downloads](https://img.shields.io/github/downloads/d34d0s/Lotus/total)](https://github.com/d34d0s/Lotus/releases)
[![GitHub Stars](https://img.shields.io/github/stars/d34d0s/Lotus?style=flat&label=stars)](https://github.com/d34d0s/Lotus/stargazers)
[![GitHub commits since tagged version](https://img.shields.io/github/commits-since/d34d0s/Lotus/Lotus.1.0.2024)](https://github.com/d34d0s/Lotus/commits/master)
[![License](https://img.shields.io/badge/license-apache%2Flibpng-green.svg)](LICENSE)

#### Relevant Links

[  [Official Wiki](https://github.com/d34d0s/Lotus/wiki) | [API Documentation](https://github.com/d34d0s/Lotus/wiki/Lotus-API-Docs) | [Lotus Examples](https://github.com/d34d0s/Lotus/tree/refactor_01/examples) ]
  
[ [Official Discord](https://discord.gg/e43PDC45wu) | [Official Reddit](https://www.reddit.com/r/Lotus/) | [Lotus Discussions](https://github.com/d34d0s/Lotus/discussions) | [Lotus FAQ](https://github.com/d34d0s/Lotus/wiki/Lotus-FAQ) ]

</br>


## Building Engine Source

1. **Clone the repository**:
    ```bash
    git clone https://github.com/d34d0s/Lotus.git
    cd Lotus
    ```

2. **Compile the engine**:
    For Windows, you can use `gcc` to build the engine:
    ```bash
    gcc -c lotus.core/*.c -Ipath_to_lotus_vendors -Ipath_to_lotus_includes -DLOTUS_EXPORT -o lotus.core/bin/*.o
    gcc -shared lotus.core/bin/*.o -lopengl32 -lgdi32 -o lotus.core/bin/Lotus.dll
    ```

3. **Install compiled binaries**:
    Move the compiled binaries into your project's `lib` and `bin` directories:
    ```bash
    cp lotus.core/bin/*.dll your_project/bin/
    ```
    ```bash
    cp vendor/bin/*.dll your_project/bin/
    ```

## Building Projects/Mods

1. Navigate to your project's source directory.
2. Compile your mod:
    ```bash
    gcc my_project.c -Ipath_to_lotus_includes -lLotus -o my_project.exe
    ```

</br>

<h2> 🪷 Lotus' Contributors 🪷 </h2>

<a href="https://github.com/d34d0s/Lotus/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=d34d0s/Lotus&max=500&columns=20&anon=1" />
</a>

</br>

<h2> 🪷 License 🪷 </h2>

Lotus is licensed under the Apache 2.0 license.

