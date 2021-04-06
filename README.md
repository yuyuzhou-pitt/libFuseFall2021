# ntapfuse

ntapfuse is a FUSE filesystem that mirrors a directory.

## Dependencies

The following packages need to be installed using

    $ sudo apt-get install <pkg-name>
* fuse
* libfuse-dev
* sqlite3
* libsqlite3-dev

## Building and Installing

In order to build and install ntapfuse, run the following command from the
src directory:

    $ ./run.sh

## Mounting the Filesystem

The filesystem can be mounted with the following command:

    $ ntapfuse mount <basedir> <mountpoint>
    
The basedir is the directory that is being mirrored. The mountpoint is the
directory that will act as a normal linux filesystem.

## Unmounting the filesystem

The filesystem can be unmounted with the following command:

    $ sudo umount <mountpoint>

## Unit tests

Unit tests are located in the unit_tests folder and can be executed with the
following commands:

    $ ruby --version (must be 2.7 or higher)
    $ git submodule update --init --recursive
    $ ruby create_makefile.rb
    $ make -f MakefileTestSupport test
