# BigInt
A class to perform precise computations with large integers.

## Background
This was created as part of the ECE 218 Data Structures course I took in Spring 2018. The objective was to practice inheritance by implementing a Big Int class which was based off of a vector of ints. Looking back, there is potential to improve the design, primarily by changing the implementation to be based of vectors of characters (i.e. strings) to conserve space. I may plan to use this as an opportunity to practice operator overloading as well.

## Key Notes
A BigInt is ...
* an inheritee of `vector<int>`
* stored in reverse

## Multiplication Algorithm
Most of the operations are fairly standard, but multiplication is worth a closer look.

![Basic-Example](https://user-images.githubusercontent.com/48261863/58354115-7820d300-7e3e-11e9-9df9-1259a8472283.gif)

Here is an example of how multiplication is conducted as learned in grade school. As one can see, the multiplier is stepped through digit-by-digit from its least significant digit, each in turn multiplying the multiplicand. If the multiplication between two digits gives a result greater than 9, a carry is generated to be added to the next multiplication's result. But this carry is just an intermediary simplification step.

![Clean-Example](https://user-images.githubusercontent.com/48261863/58355112-5543ee00-7e41-11e9-9261-f9039e4004e6.gif)

Above is the same example but with the carry during multiplication kept together with the least significant digit of the result. Now the result is tallied from these intermediary columns which are not necessarily singular digits. This effectively "unmasks" the algorithm slightly, so that it can be seen where each of the elements in the columns directly originates from.

![Basic-Algorithm](https://user-images.githubusercontent.com/48261863/58301291-e2406600-7db3-11e9-9574-38139ff76371.gif)

Here the algorithm is unveiled in full. Each of the rows in the addition correlates to a particular digit from the multiplier's "pass" through the multiplicand. The shift for each subsequent round of multiplication is necessary as the problem is essentially reformulated as the multiplicand shifted the appropriate number of powers of 10 being multiplied by the multiplier's digit for that power (e.g. the problem is taken as (473 * 5) + (473 * 20), the latter of which is reformulated as 4730 * 2). This idea persists, so if there had been a multiplier with digits in higher powers of 10, the shift would be larger for those digits.

![Algorithm-Passes](https://user-images.githubusercontent.com/48261863/58357671-b4a5fc00-7e49-11e9-9e4f-06fac35b748c.png)

So a problem arises - it is not ideal to hold entire intermediary multiplication results in reserve waiting to tally them. It is better to be able to tally column-wise from the get-go. With the scheme excluding the carry in the multiplication stage it is possible to reliably determine the maximum number of passes (i.e. the number of columns to tally). Since the shift only applies for digits in the multiplier that are ***not*** in the ones place, the maximum shift will always be 1 less than the number of digits in the multiplier. However, there is always an entry for each digit of the multiplicand so the total number of columns becomes the sum of the number of their digits _minus_ 1.

![Algorithm-Index](https://user-images.githubusercontent.com/48261863/58358022-2894d400-7e4b-11e9-8328-afd02d6cf3d6.png)

The last issue is the indexing. As was deduced from the full algorithm example, the indexing into the multiplicand begins with the current pass number as counted from its least significant digit (0 indexed of course). From there it countinues through the number _in reverse_. This is reflected with the above `i` index into the object `A`. For the multiplier, the indexing begins with its least significant digit and continues up through its entirety always (reflected by the above `j` index into `B`). The sum is derived from all of the relevant multiplications for that pass (an out-of-bounds index is handled by the digit function). 

At this point, the insightful reader might note that the inner for loop's performance is largely inefficient, and in fact this is true. There are two "wasteful zones" where computations are needlessly performed (i.e. the areas where the shift from multipliers induces large amounts of zeros to appear - the upper left and lower right regions in the addition area). The stop condition being `i >= 0` effectively removes the entirety of lower right region by forfeiting computations that request indexes less than 0 in `A`. However, the upper left region is left intact and in fact a new region is introduced (indexes where `j >= B->size()`). To rectify this issue, better control needs to be established over the starting indexes for `i` and `j` as well as the stopping condition. The below may serve as a potential solution.

```C++
...
int sum = 0;
int istart, jstart, iend;
if (h >= A->size()) {           //Upper left region elimination (indexes > A's MSD)
  istart = A->size() - 1;
  jstart = h - A->size() + 1;   //  Need to start with the proper B index
} else {                        //General case, indexing within A and from the end of B
  istart = h;
  jstart = 0;
}
                                 //Lower right region elimination
if (istart - (B->size() - jstart - 1) < 0) {
  iend = 0;
} else {                        //General case, indexing within A and need to stop after proper number of indexes into B
  iend = istart - (B->size() - jstart - 1);
}
for (i = istart, j = jstart; i >= iend; i--, j++) {
...
```
