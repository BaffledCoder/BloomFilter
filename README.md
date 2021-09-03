A Bloom filter is a space-efficient probabilistic data structure that is used to test whether an element is a member of a set.
For example, checking availability of username is set membership problem, where the set is the list of all registered username.
The price we pay for efficiency is that it is probabilistic in nature that means, there might be some False Positive results.
False positive means, it might tell that given username is already taken but actually it’s not.  

#Bloom Filter Implementation
##This folder contains the implementation of Bloom Filter(`BloomFilte.cpp` and `BloomFilter.h`). 
- To run Bloom Filter, please run `g++ BloomFilter.cpp -o bf` with g++ installed\
  1. There are two main functions in this implementation, which are `build` and `query`
  - To run build function, please run  
  ```
  $bf build -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
  ``` 
  please provide a `<key file>`, which contains `<num. distinct keys>` distinct input keys, 
  and construct a bloom filter with a target false positive rate of `<fpr>`. The constructed Bloom filter should be written to the file `<output file>`.
For example, please run 'build -k list.txt -f 0.001 -n 10 -o out.txt'

  - To run query function, please run  
```
$bf query -i <input file> -q <queries> -o <output file>
```


  please provide a `<input file>`, which contains the <output file> from `build`, and a <queries>, which has a query in each line. 
  Please also provide a output file <output file> for writing down the result of queries.
  For example, please run 'query -i out.txt -q list.txt -o queryResults.txt'
