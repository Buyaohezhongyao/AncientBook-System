#!/bin/sh

cd "$(dirname "$0")/.." || exit 1

mkdir -p build
cp data/books.txt tests/books_backup.txt
cp data/sales_log.txt tests/sales_log_backup.txt

gcc -std=c99 -Wall -Wextra -pedantic src/main.c src/book.c src/fileio.c src/utils.c -o build/AncientBookSystem
if [ $? -ne 0 ]; then
    echo "Compile failed."
    cp tests/books_backup.txt data/books.txt
    cp tests/sales_log_backup.txt data/sales_log.txt
    rm -f tests/books_backup.txt
    rm -f tests/sales_log_backup.txt
    exit 1
fi

./build/AncientBookSystem < tests/test_input.txt > tests/test_output_actual.txt 2>&1
status=$?

cp tests/books_backup.txt data/books.txt
cp tests/sales_log_backup.txt data/sales_log.txt
rm -f tests/books_backup.txt
rm -f tests/sales_log_backup.txt

echo "Test finished. Output file: tests/test_output_actual.txt"
exit $status
