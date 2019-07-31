# Large-scale-Processing-of-JSON-Objects
Assignment : https://sites.google.com/view/jiazou-web/assignment

#Part 1:
1. Import jsoncpp for this project.
2. Put the data "small.txt", "medium.txt", "large.txt" in the same folder with the program. 
<br/>And change the file name in the main() function in order to run different data.
3. Run main1.cpp and main2.cpp separeately. (mian1.cpp is for Q1-Q4 in the assignment, and main2.cpp is for Q5 mentioned in the interview)

#Part 2:
1. I read the in a binary form and add '[' and ']' to the original file in order to use jsoncpp to parse the data.
2. Use multi-threading to handle different queries.
3. Make sure the file is cached in OS buffer cache before I run my program.
4. Use unorder_map to store output informaiton.

#Part 3:
1. list the configuration of your computer: 
Number of CPU cores: 4
Size of physical memory: 8G
Type of Disk: SSD
Operating System and version: Win 10
2.  list the performance bottleneck(s) of your program when it runs on your computer:
My program is CPU-intensive and the top three functions that spent the most CPU cycles are XXX, YYY, ZZZ.
My program is Disk I/O-intensive and the top three functions that spent the longest time are XXX, YYY,  ZZZ.

