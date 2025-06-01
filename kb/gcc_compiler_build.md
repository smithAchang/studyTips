# 前言
**Linux发行版**是`Linux kernel`、`glibc & glibc++`、`gcc compiler`，以及以此三者组件为基础编译出来其它组件的完美森林。

为此，我们应该像**Linux发行版**一样，保持我们的开发的应用，在什么环境下编译，就在什么环境下部署，并且尽可能地使用**容器**技术，来保证这种依赖关系，和隔离可能的环境相互冲突。

甚至，换种视角，可以说发行版内的软件，都由`gcc compiler`确立了出生证！

**需要指出的是，此观点未经充分证实，但有一些强有力的依据。**

+ 高版本编译器带来更新的`cpu`指令支持，以及更多的代码缺陷检查、代码编译优化和更高质量的机器指令生成

> 新的`Linux kernel`通常需要这些特征

那么，`gcc compiler`是可以自举编译完成的么？**是，又不是！**


为此，重新阅读了[GCC编译安装官网介绍](https://gcc.gnu.org/install/)，不过仅摘抄了部分重点，和仅提供少量要点的中文翻译。


---
# ISO C++14 compiler
+ 需要一个支持自举过程的**前置编译器**

Necessary to bootstrap GCC. GCC 5.4 or newer has sufficient support for used C++14 features.

Versions of GCC prior to 15 allow bootstrapping with an ISO C++11 compiler, versions prior to 10.5 allow bootstrapping with an ISO C++98 compiler, and versions prior to 4.8 allow bootstrapping with an ISO C89 compiler.

If you need to build an intermediate version of GCC in order to bootstrap current GCC, consider GCC 9.5: it can build the current Ada and D compilers, and was also the version that declared C++17 support stable.

To build all languages in a cross-compiler or other configuration where 3-stage bootstrap is not performed, you need to start with an existing GCC binary (of a new enough version) because source code for language frontends other than C might use GCC extensions.

# C standard library and headers
+ 需要新的`c`标准库和头文件

In order to build GCC, the C standard library and headers must be present for all target variants for which target libraries will be built (and not only the variant of the host C++ compiler).

+ 可以仅支持x86-64
This affects the popular ‘x86_64-pc-linux-gnu’ platform (among other multilib targets), for which 64-bit (‘x86_64’) and 32-bit (‘i386’) libc headers are usually packaged separately. 
If you do a build of a native compiler on ‘x86_64-pc-linux-gnu’, make sure you either have the 32-bit libc developer package properly installed (the exact name of the package depends on your distro) or you must build GCC as a 64-bit only compiler by configuring with the option ** --disable-multilib **. 
Otherwise, you may encounter an error such as ‘fatal error: gnu/stubs-32.h: No such file’

# Options

+ 可以定制不同安装目录，特别是多版本编译器环境

 ** --prefix=dirname **

We use srcdir to refer to the toplevel source directory for GCC; we use objdir to refer to the toplevel build/object directory.

If you obtained the sources by cloning the repository, srcdir must refer to the top gcc directory, the one where the **MAINTAINERS** file can be found, and not its gcc subdirectory, otherwise the build will fail.


First, we highly recommend that GCC be built into a separate directory from the sources which does not reside within the source tree. This is how we generally build GCC; building where objdir is a subdirectory of srcdir should work as well; building where objdir == srcdir is unsupported.


Specify the toplevel installation directory. This is the recommended way to install the tools into a directory other than the default. The **toplevel installation directory** defaults to /usr/local
We highly recommend against dirname being the same or a subdirectory of objdir or vice versa. 
If specifying a directory beneath a user’s home directory tree, some shells will not expand dirname correctly if it contains the ‘~’ metacharacter; use $HOME instead.

## Cross-Compiler-Specific Options

> 头文件和系统库不一致的跨平台编译选项

+ --with-sysroot
+ --with-sysroot=dir
Tells GCC to consider dir as the root of a tree that contains (a subset of) the root filesystem of the target operating system. Target system headers, libraries and run-time object files will be searched for in there. More specifically, this acts as if --sysroot=dir was added to the default options of the built compiler. The specified directory is not copied into the install tree, unlike the options --with-headers and --with-libs that this option obsoletes. The default value, in case --with-sysroot is not given an argument, is ${gcc_tooldir}/sys-root. If the specified directory is a subdirectory of ${exec_prefix}, then it will be found relative to the GCC binaries if the installation tree is moved.

This option affects the system root for the compiler used to build target libraries (which runs on the build system) and the compiler newly installed with make install; it does not affect the compiler which is used to build GCC itself.

If you specify the --with-native-system-header-dir=dirname option then the compiler will search that directory within dirname for native system headers rather than the default /usr/include.


--with-build-sysroot
--with-build-sysroot=dir
Tells GCC to consider dir as the system root (see --with-sysroot) while building target libraries, instead of the directory specified with --with-sysroot. This option is only useful when you are already using --with-sysroot. You can use --with-build-sysroot when you are configuring with --prefix set to a directory that is different from the one in which you are installing GCC and your target libraries.

This option affects the system root for the compiler used to build target libraries (which runs on the build system); it does not affect the compiler which is used to build GCC itself.

If you specify the --with-native-system-header-dir=dirname option then the compiler will search that directory within dirname for native system headers rather than the default /usr/include.

# Building a native compiler
+ 即使需要一个前置编译器来协助自举，后面过程依然**有明显的自举过程**
+ 前置编译器简单来看，可以是编译器版本历史上，某个符合编译前置要求的版本

For a native build, the default configuration is to perform **a 3-stage bootstrap** of the compiler when ‘make’ is invoked

# Building a cross compiler
+ 暗含了首先编译出来一个`native compiler`，所以，可以将重点放在制作`native compiler`上
+ 交叉编译中，有一种特殊类型就是编译出来`相同内核、glibc依赖`的交叉**高版本编译器**编译，对于此种情况并不需要设置那些交叉编译选项

When building a cross compiler, it is not generally possible to do a 3-stage bootstrap of the compiler. This makes for an interesting problem as parts of GCC can only be built with GCC.

To build a cross compiler, **we recommend first building and installing a native compiler**. You can then use the native GCC compiler to build the cross compiler. The installed native compiler needs to be GCC version 2.95 or later.

Assuming you have already installed a native copy of GCC and configured your cross compiler, issue the command make

# Building in parallel

GNU Make 3.80 and above, which is necessary to build GCC, support building in parallel. To activate this, you can use ‘make -j 2’ instead of ‘make’

```bash
cd objdir && make -j 2
```
# Installing GCC: Final installation
Now that GCC has been built (and optionally tested), you can install it with

```bash
cd objdir && make install
```
# Building again

If you have previously built GCC in the same directory for a different target machine, do ‘make distclean’ to delete all files that might be invalid. One of the files this deletes is Makefile; if ‘make distclean’ complains that Makefile does not exist or issues a message like “don’t know how to make distclean” it probably means that the directory is already suitably clean. However, with the recommended method of building in a separate objdir, you should simply use a different objdir for each target.

```bash
cd objdir && make distclean
```
---
# 相同内核、glibc依赖的交叉编译`native cross compiler`脚本
```bash
srcdir=/path/to/srcdir
objdir=/path/to/objdir

rm -rf  $objdir
mkdir -p $objdir/install
cd $objdir
$srcdir/configure [options] [target]

#
$srcdir/configure --host=x86_64-pc-linux-gnu --prefix=$objdir/install --disable-multilib --enable-languages=c,c++
```
# 对于发行版猜测
+ 在完成必要的多次交叉编译和自举编译后，最终建立依赖一致的环境，最后再编译其它组件

# 参考
+ [如何编译安装GCC编译器](https://gcc.gnu.org/install/)