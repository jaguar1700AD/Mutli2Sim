#!/bin/bash
input="src/lut_config"
while IFS= read -r line
do
  echo "$line" > "src/lut_config_private"
  $1 --si-sim detailed ../benchmarks/$2/$2"_static" --load ../benchmarks/$2/$2"_Kernels".bin -i $3 
done < "$input"
