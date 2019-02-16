## Dynamic Quad Tree

### Input 

The input contains multiple cases. The first line contains a single integer T indicating the number of test cases. Each test case begins with one integer k, indicating the size (n×n) of the image, where 𝑛 = 2 ^k^ and 0 ≤ k ≤ 10. The following n lines give the information of the pixels, where the i-th line contains a binary string of n characters, containing ‘0’(white pixel) and ‘1’(black pixel), indicating the pixel information in the i-th row of the image. Then there will be an integer m (1 <= m <= 1000) indicating the number of operations. In each of the following m lines there will be two integers **r c** (1 <= r, c <= n) corresponding to the row and column of the pixel that will be flipped. 

### Output 
For each operation, print a single line containing one integer indicating the number of nodes in corresponding quad tree. 

### Sample

| Input                                          | Output     |
| ---------------------------------------------- | ---------- |
| 2 2
0011
0001
1111
0111
3
31
33
23
2
0000
0000
0000
0000
1
11 | 13 17 13 9 |

