# ntapfuse

ntapfuse is a FUSE filesystem that mirrors a directory.

## Dependencies

The following packages need to be installed using

    $ sudo apt-get install <pkg-name>
* fuse
* libfuse-dev

## Building and Installing

In order to build and install ntapfuse, run the following commands from the
src directory:

    $ autoreconf --install
    $ ./configure
    $ make
    $ sudo make install




Install mysql
    $ sudo apt install mysql-server
    $ sudo mysql_secure_installation

Select your password strength and create a password
Answer yes to all prompts

    $ sudo mysql -u root -p
    $ CREATE DATABASE quota;
    $ USE quota;
    $ CREATE TABLE user_data (user int, data bigInt DEFAULT 0);
    $ quit

## Mounting the Filesystem

The filesystem can be mounted with the following command:

    $ ntapfuse mount <basedir> <mountpoint>
    
The basedir is the directory that is being mirrored. The mountpoint is the
directory that will act as a normal linux filesystem.

## Unmounting the filesystem

The filesystem can be unmounted with the following command:

    $ sudo umount <mountpoint>

