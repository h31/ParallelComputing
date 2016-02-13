#!/bin/bash

curl http://vojnaimir.ru/files/book1.txt http://vojnaimir.ru/files/book2.txt | iconv -f WINDOWS-1251 -t UTF-8 > book.txt
sed 's/–/-/g' -i book.txt