#!/bin/bash

source common.sh
source test.sh
source fs.sh

# $FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
#     --fs='^ext4$|^pmfs$|^nova$|^winefs$' \
#     --workload='^fio_global_seq-read-4K$|^fio_global_seq-read-2M$' \
#     --ncore="^1$|^2$|^4$|^6$|^8$|^10$|^12$|^16$|^20$|^24$" --iotype='bufferedio' --dthread='0' --dsocket='0' \
#     --rcore='False' --delegate='False' --confirm='True' \
#     --directory_name="$FIO_LOG_DIR" --log_name="fs-read-1.log" --duration=10

# $FXMARK_BIN_PATH/run-fxmark.py --media='pmem-local' \
#     --fs='^ext4$|^pmfs$|^nova$|^winefs$' \
#     --workload='^fio_global_seq-write-4K$|^fio_global_seq-write-2M$' \
#     --ncore="^1$|^2$|^4$|^8$|^16$|^24$" --iotype='bufferedio' --dthread='0' --dsocket='0' \
#     --rcore='False' --delegate='False' --confirm='True' \
#     --directory_name="$FIO_LOG_DIR" --log_name="fs-write-1.log" --duration=30


$FXMARK_BIN_PATH/run-fxmark.py --media='pm-char-array' --fs='^sufs$' \
    --workload='^fio_global_seq-read-4K$' \
    --ncore="^1$|^2$|^4$|^8$|^16$|^24$" --iotype='bufferedio' --dthread='0' --dsocket="1" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FIO_LOG_DIR" --log_name="sufs-read-4k-1.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pm-char-array' --fs='^sufs$' \
    --workload='^fio_global_seq-read-2M$' \
    --ncore="^1$|^2$|^4$|^8$|^16$|^24$" --iotype='bufferedio' --dthread="0" --dsocket="1" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FIO_LOG_DIR" --log_name="sufs-read-2m-1.log" --duration=10

$FXMARK_BIN_PATH/run-fxmark.py --media='pm-char-array' --fs='^sufs$' \
    --workload='^fio_global_seq-write-4K$|^fio_global_seq-write-2M$' \
    --ncore="^1$|^2$|^4$|^8$|^16$|^24$" --iotype='bufferedio' --dthread="0" --dsocket="1" \
    --rcore='False' --delegate='False' --confirm='True' \
    --directory_name="$FIO_LOG_DIR" --log_name="sufs-write-1.log" --duration=30


echo "Parsing fio results"
for i in `ls $FXMARK_LOG_PATH/$FIO_LOG_DIR/`
do
    echo "On $i"
    $FXMARK_PARSER_PATH/pdata.py --log="$FXMARK_LOG_PATH/$FIO_LOG_DIR/$i" \
    --type='fio' --out="$DATA_PATH/$FIO_DATA_DIR"
done
echo ""
