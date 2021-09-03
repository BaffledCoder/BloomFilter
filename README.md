A Bloom filter is a space-efficient probabilistic data structure that is used to test whether an element is a member of a set.
For example, checking availability of username is set membership problem, where the set is the list of all registered username.
The price we pay for efficiency is that it is probabilistic in nature that means, there might be some False Positive results.
False positive means, it might tell that given username is already taken but actually it’s not.  
