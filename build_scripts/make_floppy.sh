#!/bin/bash

TARGET=$1
STAGE1_STAGE2_LOCATION_OFFSET=480

dd if=/dev/zero of=$TARGET bs=512 count=2880

STAGE2_SIZE=$(stat -c%s ${BUILD_DIR}/stage2.bin)
STAGE2_SECTORS=$((( ${STAGE2_SIZE} + 511 ) / 512 ))
RESERVED_SECTORS=$((1 + ${STAGE2_SECTORS}  ))

echo ${STAGE2_SIZE}
echo ${STAGE2_SECTORS}
echo ${RESERVED_SECTORS}

mkfs.fat -F 12 -R ${RESERVED_SECTORS} -n "PROTOS" $TARGET >/dev/null

dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 count=3 >/dev/null
dd if=${BUILD_DIR}/stage1.bin of=$TARGET conv=notrunc bs=1 seek=90 skip=90 >/dev/null
dd if=${BUILD_DIR}/stage2.bin of=$TARGET conv=notrunc bs=512 seek=1 >/dev/null

echo "01 00 00 00" | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$STAGE1_STAGE2_LOCATION_OFFSET
printf "%x" ${STAGE2_SECTORS} | xxd -r -p | dd of=$TARGET conv=notrunc bs=1 seek=$(( $STAGE1_STAGE2_LOCATION_OFFSET + 4 ))


mcopy -i $TARGET ${BUILD_DIR}/oskrnl.bin "::oskrnl.bin"
mcopy -i $TARGET test.txt "::test.txt"
mmd -i $TARGET "::mydir"
mcopy -i $TARGET test.txt "::mydir/test.txt"