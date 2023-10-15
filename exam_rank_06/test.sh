#!/bin/bash

for ((i=1; i<=3000; i++))
do
  echo "This is message number $i" | nc localhost 6667
done