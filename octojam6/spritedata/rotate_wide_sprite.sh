#!/bin/bash

BYTES_PER_REG_SPRITE=32

ORIG_SPRITE=$1
DEST_SPRITE=$2

ROTATE=./rotate

dd if=$ORIG_SPRITE bs=$BYTES_PER_REG_SPRITE count=1 of=pt1-temp.bin
dd if=$ORIG_SPRITE bs=$BYTES_PER_REG_SPRITE count=1 skip=1 of=pt2-temp.bin

$ROTATE 2 pt1-temp.bin pt1-temp.rot
$ROTATE 2 pt2-temp.bin pt2-temp.rot

cat pt1-temp.rot pt2-temp.rot > $DEST_SPRITE

rm pt1-temp.bin pt2-temp.bin pt1-temp.rot pt2-temp.rot



