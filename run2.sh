#!/bin/bash

sat_output="${1}.satoutput"
graph_file="${1}.graphs"
mapping_file="${1}.mapping"

./genMapping "$sat_output" "$graph_file" > "$mapping_file"