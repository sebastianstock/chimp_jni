# CHIMP_JNI

A JNI-wrapper for the hierarchical hybrid planner [CHIMP](https://github.com/sebastianstock/chimp).

## Installation

### Java

Install java
```bash
sudo apt-get install openjdk-8-jdk
```
and add the line `JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64"` to `/etc/environment`.


### CHIMP

**TODO:**

Clone CHIMP

Build and create .jar

Set CLASSPATH to CHIMP.

### chimp_jni

To compile and install chimp_jni:

```bash
cd ~
git clone http://git.hb.dfki.de/pbr/chimp_jni
mkdir chimp_jni_install
cd ~/chimp_jni
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=~/chimp_jni_install
make
make install
```

To be able to find chimp_jni in your projects, update your PKG_CONFIG_PATH:

```bash
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:~/chimp_jni_install/lib/pkgconfig
```

Because of the dependency to JNI, for which no pkg-config file is available, you have to add that dependency to your cmake:

```cmake
find_package (Java REQUIRED)
find_package (JNI REQUIRED)
include (UseJava)
include_directories (${JNI_INCLUDE_DIRS})
```

You can now use

```cmake
find_package(PkgConfig)
pkg_check_modules(CHIMPJNI REQUIRED chimp_jni)
include_directories(${CHIMPJNI_INCLUDE_DIRS})
link_directories(${CHIMPJNI_LIBRARY_DIRS})
target_link_libraries(foo ${CHIMPJNI_LIBRARIES} ${JNI_LIBRARIES})
add_definitions(${CHIMPJNI_CFLAGS_OTHER})
```

in your own cmake-project.