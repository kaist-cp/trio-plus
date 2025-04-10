#!/bin/bash

source common.sh
source test.sh
source fs.sh

$FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
    --fs='^ext4$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread='0' --dsocket="0" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="ext4-meta.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
    --fs='^nova$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread='0' --dsocket="0" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="nova-meta.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
    --fs='^winefs$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread='0' --dsocket="0" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="winefs-meta.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
    --fs='^pmfs$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread='0' --dsocket="0" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="pmfs-meta.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pm-array' --fs='^odinfs$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread="$DTHREADS" --dsocket="$MAX_SOCKETS" \
    --rcore='False' --delegate='True' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="odinfs-meta.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pm-char-array' --fs='^sufs$' \
    --workload='^MRPL$|^MRPM$|^MRPH$|^MWCL$|^MWCM$|^MWUL$|^MWUM$|^MWRM$' \
    --ncore="$core" --iotype='bufferedio' --dthread='0' --dsocket="$MAX_SOCKETS" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FM_LOG_DIR" --log_name="sufs-meta.log" --duration=10

echo "Parsing fxmark results"
for i in `ls $FXMARK_LOG_PATH/fxmark/`
do
    echo "On $i"
    $FXMARK_PARSER_PATH/pdata.py --log="$FXMARK_LOG_PATH/$FM_LOG_DIR/$i" \
    --type='fxmark' --out="$DATA_PATH/$FM_DATA_DIR"
done
echo ""

