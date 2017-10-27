#!/bin/sh
make clean && make && sudo rmmod flex && sudo insmod flex.ko && sudo dmesg -c
