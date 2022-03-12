# gmsv_gsimd
[![Build status](https://ci.appveyor.com/api/projects/status/nqh6xy8et08re5ok?svg=true)](https://ci.appveyor.com/project/storm37000/gmsv-gsimd)

Use SIMD functions of modern CPUs to greatly accelerate your LUA code. <br>

## Installation
**Requires c++ 2019 runtimes for windows and an in support version of Linux (Ubuntu 2004+ | Debian 10+)** <br>
Click on releases and download the dll for your OS and bit of srcds that you are running. <br>
Dont forget to download and install the lua file from the zip or tar.gz file in the release.

## Use
Module is in early testing phase, instructions will come when they are ready.

## Compiling
Compiling out of the box is only supported on appveyor. <br>
Simply run the build.ps1 file in your VS2019 or Linux compiler env and it will handle everything automatically and output both 32 and 64 bit binaries. <br>
You can take the blocks of code out of the if (top block for Windows, bottom for Linux) if you do not want to use appveyor. <br>
For Linux this would require manually installing the dependencies to run the ps1 file and things like git, svn, and premake5. <br>
