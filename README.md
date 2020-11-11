# Glipus, to secure your files.
Glipus is an open-source encryption software that uses XOR cipher.
It helps you keep your files safe by simply clicking two buttons.

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

Glipus uses **lubcurl** (throught vcpkg) and **wxWidgets**. Be sure to install them before compiling.