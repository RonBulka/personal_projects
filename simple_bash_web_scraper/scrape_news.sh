#!/bin/bash
# Scrape news from ynetnews.com and count the number of times each candidate's
# name appears in each article

site="https://www.ynetnews.com/category/3082"
names=(Netanyahu Gantz Bennett Peretz)
# Get all links to news articles
news_links=$(wget --no-check-certificate -O - $site 2> /dev/null | \
             grep -oP 'https://(www.)?ynetnews.com/article/[a-zA-Z0-9]*' | \
             sort | uniq)
result="$(echo "$news_links" | wc -l)\n"
# For each link, get the article's text and count the number of times each
# candidate's name appears in the article
for link in $news_links; do
    link_stat=$link
    name_repetition=(0 0 0 0)
    data=$(wget --no-check-certificate -O - $link 2> /dev/null)
    for (( i=0; i<4; i++ )); do
        name_repetition[$i]=$(echo "$data" | grep -oP "${names[$i]}" | wc -l)
    done
    sum=0
    # Format the results
    for (( i=0; i<4; i++ )); do
        sum=$(( sum+name_repetition[$i] ))
        link_stat="$link_stat, ${names[$i]}, ${name_repetition[$i]}"
    done
    # If no candidate's name appears in the article, add a '-' to link_stat
    if [[ sum -eq 0 ]]; then
            link_stat="$link, -"
    fi
    result="$result $link_stat\n"
done
result=$(echo -e "$result" | cut -c1-$(( ${#result} - 2 )))
echo -e -n "$result" > results.csv
