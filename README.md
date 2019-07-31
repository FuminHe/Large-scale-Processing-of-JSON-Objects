# Large-scale-Processing-of-JSON-Objects
Assignment : https://sites.google.com/view/jiazou-web/assignment

#Part 1:
1. Import jsoncpp for this project.
2. Put the data "small.txt", "medium.txt", "large.txt" in the same folder with the program. And change the file name in the main() function in order to run different data.
3. Run main1.cpp and main2.cpp separeately. (mian1.cpp is for Q1-Q4 in the assignment, and main2.cpp is for Q5 mentioned in the interview. I devided the cost_components into 5 blocks:[0-12],[13-24], [25-36],[37-48],[49-60])
4. small.txt: Q1: average = 49.2446,   Q2: max cost_components: 52.5,  Q3: "small-Q3List.txt" in src,  Q4: "small-Q4List.txt". 
5. medium.txt: Q1: average = 49.9419,   Q2: max cost_components: 52.5,  Q3: "medium-Q3List.txt" in src,  Q4: "medium-Q4List.txt". 


#Part 2:
1. I read the file in a binary form and add '[' and ']' to the original file in order to use jsoncpp to parse the data.
2. Use multi-threading to handle different queries.
3. Make sure the file is cached in OS buffer cache before I run my program.
4. Use unordered_map to store output informaiton.

#Part 3:
1. list the configuration of your computer: 
<br/>Number of CPU cores: 4
<br/>Size of physical memory: 8G
<br/>Type of Disk: SSD
<br/>Operating System and version: Win 10
2.  list the performance bottleneck(s) of your program when it runs on your computer:

