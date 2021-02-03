#!/bin/bash
make
rm ../Results/$2
tput reset
input="src/lut_config"
i=0
while IFS= read -r line
do
  echo "$line" > "src/lut_config_private"
  $1 --si-sim detailed ../benchmarks/$2/$2"_static" --load ../benchmarks/$2/$2"_Kernels".bin -i $3 >> ../Results/$2
  i=$((i+1))
  echo i: $i
done < "$input"
