#!/usr/bin/bash

rm -rf test.ext2 test.ext3 test.ext4 test_1k.ext4

mkfs.ext2 test.ext2 1024
mkfs.ext3 test.ext3 1024
mkfs.ext4 test.ext4 1024
mkfs.ext4 -b 1024 test_1k.ext4 1024
