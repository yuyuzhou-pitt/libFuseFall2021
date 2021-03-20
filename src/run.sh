#!/bin/bash
fusermount -u mountpoint
sudo make install
make clean
rm log.txt
rm basedir/*
ntapfuse mount basedir mountpoint
