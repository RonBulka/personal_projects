#!/bin/bash
# Calculate statistics about exam results data
readonly num_of_bins=10
readonly bins=20
readonly passed_mark=55
readonly percentage=100

calculate_passed_precentage() {
    # Calculate the precentage of students who passed the course
    # $1 - file name
    # Return the precentage of students who passed the course
    total=0
    passed=0
    # capture output and error messages of command
    hist_output=$(./hist.exe $1 -n_bins $bins)
    # capture exit status of command
    hist_exit_status=$?
    if [[ $hist_exit_status -ne 0 ]]; then
        echo "Error: failed to execute hist.exe" >&2
        exit 1
    fi
    while read -r key value; do
        total=$(( total+value ))
        if [[ "$(echo $key | cut -d '-' -f 1)" -ge "$passed_mark" ]]; then
            passed=$(( passed+value ))
        fi
    done < <(echo "$hist_output" | awk -F '\t' '{print $1, $2}')
    if (( total > 0 )); then
        echo $(( $percentage * passed / total ))
    else
        echo 0
    fi
}

# Check arguments
if [[ $# -ne 1 ]]; then
    echo "Wrong number of arguments" >&2
    exit 1
fi

# Check format of argument
if [[ ! $1 =~ ^[0-9]{6}$ ]]; then
    echo "Wrong format of argument" >&2
    exit 1
fi

# Check if file exists
if [[ ! -e $1.txt ]]; then
    echo "Course not found" >&2
    exit 1
fi

# Make relevant directory (the file name + _stat)
if [[ ! -d $1_stat ]]; then
    mkdir $1_stat
# Directory already exists -> clean it
else
    rm -rf $1_stat
    mkdir $1_stat
fi

# Make histogram.txt file using hist.exe
if ./hist.exe $1.txt -n_bins $num_of_bins > $1_stat/histogram.txt; then
    :
else
    echo "Failed to create histogram" >&2
fi

# Make statistics.txt using median.exe, mean.exe, min.exe, max.exe, hist.exe
if mean=$(./mean.exe $1.txt); then
    :
else
    echo "Failed to calculate mean" >&2
    mean=''
fi

if median=$(./median.exe $1.txt); then
    :
else
    echo "Failed to calculate median" >&2
    median=''
fi

if min=$(./min.exe $1.txt); then
    :
else
    echo "Failed to calculate minimum" >&2
    min=''
fi

if max=$(./max.exe $1.txt); then
    :
else
    echo "Failed to calculate maximum" >&2
    max=''
fi

if passed=$(calculate_passed_precentage $1.txt); then
    passed="$passed%"
else
    echo "Failed to calculate passed percentage" >&2
    passed=''
fi
echo -e "$mean\t$median\t$min\t$max\t$passed" > $1_stat/statistics.txt
