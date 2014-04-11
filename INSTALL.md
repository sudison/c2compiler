
## Packages for Arch Linux (AUR)
* [llvm-c2](https://aur.archlinux.org/packages/llvm-c2/)
* [c2c-git](https://aur.archlinux.org/packages/c2c-git/)

Howto build without an AUR helper:
```
$ curl -O https://aur.archlinux.org/packages/ll/llvm-c2/llvm-c2.tar.gz
$ tar xvf llvm-c2.tar.gz
$ cd llvm-c2
$ makepkg -is

$ curl -O https://aur.archlinux.org/packages/c2/c2c-git/c2c-git.tar.gz
$ tar xxf c2c-git.tar.gz
$ cd c2c-git
$ makepkg -is
```

* To build with an AUR helper, for example [packer](https://aur.archlinux.org/packages/packer/):

```$ packer -S c2c-git```


## Installation of LLVM/Clang (C2 version)
C2 is based on LLVM 3.3 and some parts of Clang 3.3.
To install C2, follow the steps below. The example shows
how to install in **$HOME/llvm-c2**, but any other dir should work.

* download Compiler RT sources (http://llvm.org/releases/3.3/compiler-rt-3.3.src.tar.gz)

To build:
```
git clone git://github.com/llvm-mirror/llvm.git
cd llvm/
git checkout -b release_33 origin/release_33
cd projects
tar -xf <path>/compiler-rt-3.3.src.tar.gz
mv compiler-rt-3.3.src compiler-rt
cd ../tools
git clone git://github.com/c2lang/clang.git
cd clang
git checkout -b c2master_33 origin/c2master_33
cd ../../..
mkdir llvm_build
export CC=gcc (optional)
export CXX=g++ (optional)
../llvm/configure --enable-optimized --prefix=$HOME/llvm-c2/
make -j4
make install
```

Voila! When adding **$HOME/llvm-c2/bin** to the your $PATH, you should be able
to build C2.
```
export PATH=$HOME/llvm-c2/bin:$PATH
```

## Installation of C2
To build C2: (llvm-c2/bin must be in PATH)
```
git clone git://github.com/c2lang/c2compiler.git
cd c2compiler/c2c
make -j4
```
If all goes well, the **c2c** executable should appear.

If you get an error with some Clang/C2 errors, try updating your clang C2 archive.

## Getting and Building C2C
To run the unit tests:
```
cd tools/test
make
cd ../../c2c
make test
```
