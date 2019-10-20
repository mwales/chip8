#!/bin/sh

# Path to my program that converts file to raw hex
HEXDUMP=../../../utils/bintools/bin2hex

echo "spinner-frame-0:" > generated.asm
$HEXDUMP frame0.bin >> generated.asm

echo "" >> generated.asm
echo "spinner-frame-1:" >> generated.asm
$HEXDUMP frame1.bin >> generated.asm

./rotate_wide_sprite.sh frame0.bin frame2.bin

echo "" >> generated.asm
echo "spinner-frame-2:" >> generated.asm
$HEXDUMP frame2.bin >> generated.asm

./rotate_wide_sprite.sh frame1.bin frame3.bin

echo "" >> generated.asm
echo "spinner-frame-3:" >> generated.asm
$HEXDUMP frame3.bin >> generated.asm

./rotate_wide_sprite.sh frame2.bin frame4.bin

echo "" >> generated.asm
echo "spinner-frame-4:" >> generated.asm
$HEXDUMP frame4.bin >> generated.asm

./rotate_wide_sprite.sh frame3.bin frame5.bin

echo "" >> generated.asm
echo "spinner-frame-5:" >> generated.asm
$HEXDUMP frame5.bin >> generated.asm

./rotate_wide_sprite.sh frame4.bin frame6.bin

echo "" >> generated.asm
echo "spinner-frame-6:" >> generated.asm
$HEXDUMP frame6.bin >> generated.asm

./rotate_wide_sprite.sh frame5.bin frame7.bin

echo "" >> generated.asm
echo "spinner-frame-7:" >> generated.asm
$HEXDUMP frame7.bin >> generated.asm

./rotate_wide_sprite.sh frame6.bin frame8.bin

echo "" >> generated.asm
echo "spinner-frame-8:" >> generated.asm
$HEXDUMP frame8.bin >> generated.asm

rm frame2.bin frame3.bin frame4.bin frame5.bin frame6.bin frame7.bin frame8.bin






