# Applications
A collection of code that stands to benefit from using the BigInt class for calculations.

## PascalPattern.cpp
This was an interesting pattern I found while looking through the Merge Sort algorithm.
I was trying to see how the number of allowable orderings for elements in a given merge step
related to the total number of orderings possible (`n!` for a set of n elements being merged).
Here I discovered that the set of allowables followed the order of Pascal's Triangle as there
are two clear cut "piles" to choose from. However, typically the two arrays being merged are
close in size (e.g. a merger between a sorted array of size `10` with say another of size `2`,
corresponding to `12C10` or `12C2`, would be rather uncommon), so the pattern is somewhat hidden.
<br>
Beyond this I sought to determine the pattern behind the change in the sizes of the sets of
allowables from a given `n` to `n + 1`. Here is where I found the ratio to be the previous term 
being multiplied by a constant pattern that seems to follow the below.

 `1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, ...`
 
This corresponds to the following `n`'s.

 `1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, ...`

The formula that creates this is rather easily found to be
```
 (factorial(floor(n / 2)) * factorial(n - floor(n / 2))) / 
            (factorial(floor((n - 1) / 2) * factorial(n - floor((n - 1) / 2)))
```
which is just the denominator of the n choose k equation for `n` divided by the one for `n - 1`.
Simple, but interesting nonetheless.
