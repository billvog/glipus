# Glipus, to secure your files.
Glipus is a tool to keep your files safe with strong AES 256-bit encryption.
It's free and open-source software, licensed under GPLv3.

Download from https://billvog.ga/#software

To build project files run:
```
// Visual Studio (2019)
vendor/bin/premake/premake5 vs2019

// Codelite
vendor/bin/premake/premake5 codelite

// Xcode
vendor/bin/premake/premake5 xcode4
```
For additional information, see https://premake.github.io

Glipus uses **lubcurl**, **Crypto++** (throught vcpkg) and **wxWidgets**. Be sure to install them before compiling.