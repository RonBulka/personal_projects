#!/bin/bash

# Check if firewall.exe is present in dir
if [[ ! -f "firewall.exe" ]]; then
  echo "Error: firewall.exe file not found."
  exit 1
fi

rules_file=$1

# Check if rules_file is empty or not provided
if [[ -z $rules_file ]]; then
  echo "Error: Argument file not provided."
  exit 1
fi

# Check if rules_file exists
if [[ ! -f $rules_file ]]; then
  echo "Error: Argument file '$rules_file' not found."
  exit 1
fi


# check that an stdin was provided.
if [ -t 0 ]; then
  echo "Error: No input received from stdin."
  exit 1
fi

# Read stdin and save it to a variable
stdin_data=$(cat)
result=()

# format a packet to match the format
sort_format(){
  local line="$1"
  local arr=($(echo "$line" | sed 's/ //g' | \
               tr ',' '\n' | sort | tr '\n' ' '))
  echo "${arr[2]},${arr[0]},${arr[3]},${arr[1]}"
}

# Read and process each line of the file
while read -r line; do
    # Remove everything that comes after # and remove all spaces
    line=$(echo "$line" | sed 's/ *#.*//; s/ //g')

    # Skip empty lines
    if [ -z "$line" ]; then
        continue
    fi
    row=($(echo "$line" | tr ',' ' ')) # Split the line into an array
    output=$(echo "$stdin_data" | ./firewall.exe "${row[0]}" |
                                  ./firewall.exe "${row[1]}" |
                                  ./firewall.exe "${row[2]}" |
                                  ./firewall.exe "${row[3]}")
    if [ -z "$output" ]; then
        continue
    fi

    IFS=$'\n' read -r -d '' -a lines_array <<< "$output"
    for line in "${lines_array[@]}"; do
      result+=("$(sort_format "$line")")
    done

done < "$rules_file"

# Print the sorted result array
printf "%s\n" "${result[@]}" | sort -u
