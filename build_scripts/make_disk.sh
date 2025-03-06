#!/bin/bash
set -e

TARGET=$1
SIZE=$2

STAGE1_STAGE2_LOCATION_OFFSET=480

DISK_SECTOR_COUNT=$((( ${SIZE} + 511) / 512 ))

DISK_PART1_BEGIN=2048
DISK_SECTOR_END=$(( ${DISK_SECTOR_COUNT} - 1 ))

dd if=/dev/zero of=$TARGET bs=512 count=${DISK_SECTOR_COUNT} >/dev/null

#create partition table
parted -s $TARGET mklabel msdos
parted -s $TARGET mkpart primary ${DISK_PART1_BEGIN}s ${DISK_SECTOR_END}s
parted -s $TARGET set 1 boot on

STAGE2_SIZE=$(stat -c%s ${BUILD_DIR}/stage2.bin)
echo ${STAGE2_SIZE}
STAGE2_SECTORS=$((( ${STAGE2_SIZE} + 511 ) / 512 ))
echo ${STAGE2_SECTORS}

if [ ${STAGE2_SECTORS} \> $((${DISK_PART1_BEGIN} - 1)) ]; then
    echo "Stage2 is too big for the disk"
    exit 2
fi

dd if=${BUILD_DIR}/stage2.bin of=$TARGET conv=notrunc bs=512 seek=1 >/dev/null

#create loop device
DEVICE=$(losetup -fP --show ${SOURCE_DIR}/${TARGET})
TARGET_PART="${DEVICE}p1"

#create fs
mkfs.fat -n "PROTOS" $TARGET_PART >/dev/null

dd if=${BUILD_DIR}/stage1.bin of=$TARGET_PART conv=notrunc bs=1 count=3 >/dev/null
dd if=${BUILD_DIR}/stage1.bin of=$TARGET_PART conv=notrunc bs=1 seek=90 skip=90 >/dev/null

echo "01 00 00 00" | xxd -r -p | dd of=$TARGET_PART conv=notrunc bs=1 seek=$STAGE1_STAGE2_LOCATION_OFFSET
printf "%x" ${STAGE2_SECTORS} | xxd -r -p | dd of=$TARGET_PART conv=notrunc bs=1 seek=$(( $STAGE1_STAGE2_LOCATION_OFFSET + 4 ))


# copy files
# mcopy -i $TARGET_PART ${BUILD_DIR}/oskrnl.bin "::oskrnl.bin"
# mcopy -i $TARGET_PART test.txt "::test.txt"
# mmd -i $TARGET_PART "::mydir"
# mcopy -i $TARGET_PART test.txt "::mydir/test.txt"

#destroy loop device
losetup -d ${DEVICE}
