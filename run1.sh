#!/bin/bash

input_file="${1}.graphs"
output_file="${1}.satinput"

./part1 "$input_file" > "$output_file"