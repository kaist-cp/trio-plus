#!/bin/bash

source common.sh
source test.sh
source fs.sh

$FXMARK_BIN_PATH/run-fxmark.py --media='pm-char-array' --fs='^sufs$' \
    --workload='^MRDL$|^MRDM$|^MWRL$' \
    --ncore="^1$|^2$|^4$|^6$|^8$|^10$|^12$|^16$|^20$|^24$|^28$|^32$|^36$|^40$|^44$|^48$" --iotype='bufferedio' --dthread='0' --dsocket="$MAX_SOCKETS" \
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