## Table of Contents
- [what is it](#UniverseDE)
- [Installation](#installation)
- [License](#license)

# UniverseDE
Desktop Environment with mutter purposelly made for BakeryOS,
a Linux Distribution(now) based on fedora in development by PiekarniaEklerki dicodrd community.:
# License
see [GNU GPL3](LICENSE)
# Installation
now only fedora linux installation:
first clone repository
`git clone https://github.com/BakeryOS/UniverseDE`
next you need to go to folder where code it is
`cd UniverseDE`
after that you compile it
`sh build.sh`
now you need to open this file
`nvim /usr/share/xsessions/Universe.desktop`
and copy this in
```
[Desktop Entry]
  Name=Universe
  Comment=UniverseDE
  Exec=/usr/local/bin/start-universe
  Type=Application
  X-LightDM-DesktopName=universe
```
and save it,
now open
`nvim /usr/local/bin/start-universe`
and copy this
```
#!/bin/sh
  mutter &
  exec (path to your Universe.elf)
```