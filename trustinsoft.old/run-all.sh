#!/bin/bash

set -euo pipefail

##############################################################################
# Working directory
##############################################################################
cd "$(dirname "$0")"

##############################################################################
# Cleaning old results
##############################################################################
res_dir="_results"
export res_dir
rm -rf tis_report.html "${res_dir}"
mkdir -p "${res_dir}"

##############################################################################
# Analysis of all test cases
##############################################################################

function run_analysis {
  analysis_name="$1"
  opt=(
    -tis-config-load config.json
    -tis-config-select "${analysis_name}"
    -tis-report
  )
  tis-analyzer "${opt[@]}"
}

export -f run_analysis
number_of_analyses=`jq ". | length" < config.json`
echo "Number of analyses: $number_of_analyses"
# parallel -j 3 run_analysis ::: $(seq $number_of_analyses)

##############################################################################
# Computing the results
##############################################################################
tis-report "${res_dir}"
