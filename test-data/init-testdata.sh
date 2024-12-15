#!/usr/bin/bash

if [[ "0" != "$(id -u)" ]] ; then
    echo "re-run as root"
    exit 1
fi


rm -rf temp
mkdir temp

for name in test.ext2 test.ext3 test.ext4 test_1k.ext4 ; do
    mount $name temp
    echo 42 > temp/answer.txt
    mkdir -p temp/foo/bar
    echo foo > temp/foo/foo.txt
    echo bar > temp/foo/bar/bar.txt
    ln -s answer.txt temp/foo.txt

    umount temp
done


rm -rf temp
