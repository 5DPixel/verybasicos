# Very Basic OS
)
```                   _               _               
__   _____ _ __ _   _| |__   __ _ ___(_) ___ ___  ___ 
\ \ / / _ \ '__| | | | '_ \ / _` / __| |/ __/ _ \/ __|
 \ V /  __/ |  | |_| | |_) | (_| \__ \ | (_| (_) \__ \
  \_/ \___|_|   \__, |_.__/ \__,_|___/_|\___\___/|___/
                |___/
```
(an ASCII art text rendering made in figlet!)

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
* GNU-compatible make, such as `make` or `gmake`
* A MinGW GCC cross-compiler
* A GCC cross-compiler for your desired architecture (only x86-64 is currently supported)
* `ld`
* `qemu-system-[your arch]` for emulation
* `bdf2psf` for building font assets
* `python3` for scripts

### Building from source

1. Clone this repo
   ```sh
   git clone https://github.com/5DPixel/verybasicos.git --recurse-submodules
   ```
2. Build submodules
    ```sh
    make submodules CPUS=$(nproc)
    ```
3. Create a configuration to use for building
	```sh
	make defconfig
	```

4. Build the source
    ```sh
    make CPUS=$(nproc)
    ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>
