#!/bin/bash

exec 0< numbers.txt
count=0
sum=0
while read line
do
sum=$[$sum + $line]
count=$[ $count + 1 ]
shift
done

echo $count
echo $[$sum/$count]