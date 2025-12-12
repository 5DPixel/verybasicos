# Very Basic OS

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#building-from-source">Building from source</a></li>
      </ul>
    </li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

A "very basic" `x86_64` operating system supporting both legacy BIOS and UEFI systems, written by one person.

<!-- GETTING STARTED -->
## Getting Started

The following will provide instructions and prerequisites to build the operating system from scratch, and emulate it.

### Prerequisites

<br>Required libraries:</br>
* `build-essential`
* `git`
* `make`
* A MinGW GCC cross-compiler
* A GCC cross-compiler for your desired architecture (only x86-64 is currently supported)
* `qemu-system-[your arch]` for emulation
* `bdf2psf` for building font assets

### Building from source

1. Clone this repo
   ```sh
   git clone https://github.com/5DPixel/verybasicos.git --recurse-submodules
   ```
2. Build submodules
    ```sh
    make submodules CPUS=$(nproc)
    ```

3. Build the source
    ```sh
    make CPUS=$(nproc)
    ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>
