## XDial widget
This is a simple widget showing a gauge with a pointer
that can be moved (set to values) in some range mix..max.

## Build
Create Makefile:

```shell
-bash-5.3$ xmkmf
mv -f Makefile Makefile.bak
imake -DUseInstalled -I/usr/local/lib/X11/config
```

Create Library with single widget code:
```shell
-bash-5.3$ make
```

Create test program:
```shell
-bash-5.3$ make dial
```

## Run
```shell
-bash-5.3$ ./dial
```
## Credits
The code is a small step away from its original source from the Young-book.

* *The X Window System : programming and applications with Xt*. Young, Douglas A., Online version,
  https://archive.org/details/xwindowsystempro0000youn