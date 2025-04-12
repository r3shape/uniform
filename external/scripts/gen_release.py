import os
import json
import subprocess
import shutil

SEP = os.sep
CWD = os.getcwd()
CALL_DIR = CWD
RELEASE_VER = None

README = """
The r3engine is a pluggable, highly extendable game engine written in pure C,
designed to make the development and distribution of games and media simpler.

You can find the latest release and additional information at:
https://github.com/r3shape/r3engine

This library is distributed under the terms of the MIT license,
available in [LICENSE.txt](LICENSE.txt).

# Using this package
This package contains the `r3` engine built for x64 Windows.

To use this package, simply replace an existing 64-bit r3.dll with the one included here.

# Development packages
If you're looking for packages containing headers and static archives, check for this package:
-  r3-dev-2025.0.1-mingw-w64.zip (for development using mingw-w64)

Happy Coding!

Izaiyah Stokes (d34d0s.dev@gmail.com)
"""

LICENSE = """
MIT License

Copyright (c) 2025 Izaiyah Stokes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

def os_path(path: str = None) -> str:
    return None if path is None else path.replace("/", SEP).replace("\\", SEP).strip()

def fetch_files(extension: str, directory: str) -> list:
    matched = []
    for root, _, files in os.walk(directory):
        for f in files:
            if f.endswith(extension):
                matched.append(os.path.join(root, f))
    return matched

def fetch_files_multi(exts: list[str], directory: str) -> list:
    matched = []
    for ext in exts:
        matched += fetch_files(ext, directory)
    return matched

def gen_release() -> None:
    print("[*] Generating release...")

    # Navigate to r3make directory
    r3make_path = None
    for root, _, files in os.walk(CWD):
        if "r3make" in files:
            r3make_path = os.path.join(root, "r3make")
            os.chdir(root)
            break

    if not r3make_path:
        print("[-] Error: r3make not found.")
        return

    with open("r3make", "r") as f:
        config = json.load(f)

    try:
        artifact = config["c-targets"]["r3"]
        out_dir = os_path(artifact["out-dir"])
        out_name = artifact["out-name"]
        out_type = artifact["out-type"]
    except KeyError as e:
        print(f"[-] Error in r3make config: missing {e}")
        return

    # Build DLL first
    config["c-targets"]["r3"]["out-type"] = "dll"
    with open("r3make", "w") as f:
        json.dump(config, f, indent=4)
    subprocess.call(["r3make", "r3"])

    # Then build static lib
    config["c-targets"]["r3"]["out-type"] = "lib"
    with open("r3make", "w") as f:
        json.dump(config, f, indent=4)
    subprocess.call(["r3make", "r3"])

    # Get version from version file
    version_path = None
    for root, dirs, files in os.walk(CALL_DIR):
        if root.endswith(os_path("r3/core")) and "version" in files:
            version_path = os.path.join(root, "version")
            break

    if not version_path:
        print("[-] Error: version file not found.")
        return

    with open(version_path, "r") as f:
        RELEASE_VER = f.read().strip()

    release_base = os_path(f"{CALL_DIR}/release")
    std_dir = os_path(f"{release_base}/r3-{RELEASE_VER}-mingw-w64")
    dev_dir = os_path(f"{release_base}/r3-{RELEASE_VER}-dev-mingw-w64")

    os.makedirs(std_dir, exist_ok=True)
    os.makedirs(dev_dir, exist_ok=True)

    dll_path = os.path.join(out_dir, f"{out_name}.dll")
    lib_path = os.path.join(out_dir, f"{out_name}.lib")

    if not os.path.exists(dll_path):
        print("[-] Missing DLL build artifact:", dll_path)
        return
    if not os.path.exists(lib_path):
        print("[-] Missing LIB build artifact:", lib_path)
        return

    # === Standard release ===
    print("[+] Creating standard release...")
    shutil.copyfile(dll_path, os.path.join(std_dir, f"{out_name}.dll"))

    with open(os.path.join(std_dir, "README.md"), "w") as f:
        f.write(README.strip())

    with open(os.path.join(std_dir, "LICENSE.txt"), "w") as f:
        f.write(LICENSE.strip())
    
    with open(os.path.join(std_dir, "manifest.json"), "w") as mf:
        json.dump({
        "name": out_name,
        "version": RELEASE_VER,
        "type": out_type,
        "platform": "mingw-w64",
        "files": os.listdir(std_dir)
        }, mf, indent=4)

    # === Dev release ===
    print("[+] Creating dev release...")
    bin_dir = os.path.join(dev_dir, "bin")
    lib_dir = os.path.join(dev_dir, "lib")
    inc_dir = os.path.join(dev_dir, "r3")

    os.makedirs(bin_dir, exist_ok=True)
    os.makedirs(lib_dir, exist_ok=True)
    os.makedirs(inc_dir, exist_ok=True)

    # Copy DLL + lib
    shutil.copyfile(dll_path, os.path.join(bin_dir, f"{out_name}.dll"))
    shutil.copyfile(lib_path, os.path.join(lib_dir, f"{out_name}.lib"))

    # Copy public headers
    for root, _, files in os.walk(os_path("r3")):
        for f in files:
            if f.endswith(".h"):
                src = os.path.join(root, f)
                rel = os.path.relpath(root, os_path("r3"))
                dst_folder = os.path.join(inc_dir, rel)
                os.makedirs(dst_folder, exist_ok=True)
                shutil.copyfile(src, os.path.join(dst_folder, f))

    with open(os.path.join(dev_dir, "README.md"), "w") as f:
        f.write(README.strip())

    with open(os.path.join(dev_dir, "LICENSE.txt"), "w") as f:
        f.write(LICENSE.strip())

    with open(os.path.join(dev_dir, "manifest.json"), "w") as mf:
        json.dump({
        "name": out_name,
        "version": RELEASE_VER,
        "type": out_type,
        "platform": "mingw-w64",
        "files": os.listdir(dev_dir)
        }, mf, indent=4)

    print(f"[✓] Standard release generated at: {std_dir}")
    print(f"[✓] Dev release generated at: {dev_dir}")

if __name__ == "__main__":
    gen_release()