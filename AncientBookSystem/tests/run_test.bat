@echo off
cd /d "%~dp0\.."

if not exist build mkdir build
copy /Y data\books.txt tests\books_backup.txt >nul
copy /Y data\sales_log.txt tests\sales_log_backup.txt >nul

gcc -std=c99 -Wall -Wextra src\main.c src\book.c src\fileio.c src\utils.c -o build\AncientBookSystem.exe
if errorlevel 1 (
    echo Compile failed.
    copy /Y tests\books_backup.txt data\books.txt >nul
    copy /Y tests\sales_log_backup.txt data\sales_log.txt >nul
    del tests\books_backup.txt
    del tests\sales_log_backup.txt
    exit /b 1
)

build\AncientBookSystem.exe < tests\test_input.txt > tests\test_output_actual.txt 2>&1
set TEST_STATUS=%ERRORLEVEL%

copy /Y tests\books_backup.txt data\books.txt >nul
copy /Y tests\sales_log_backup.txt data\sales_log.txt >nul
del tests\books_backup.txt
del tests\sales_log_backup.txt

echo Test finished. Output file: tests\test_output_actual.txt
exit /b %TEST_STATUS%
