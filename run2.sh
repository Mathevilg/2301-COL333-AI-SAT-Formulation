#!/bin/bash

sat_output="${1}.satoutput"
graph_file="${1}.graph"
mapping_file="${1}.mapping"

./genMapping "$graph_file" "$sat_output" > "$mapping_file"