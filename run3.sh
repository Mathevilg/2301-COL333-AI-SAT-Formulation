#!/bin/bash

input_file="${1}.graphs"
output_file="${1}.mapping"

./part2 "$input_file" > "$output_file"