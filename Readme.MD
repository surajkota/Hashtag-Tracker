Implemented a system to find the n most popular hashtags that appeared on social media such as Facebook or Twitter. Hashtags are given from an input file. 
Basic idea for the implementation is to use a max fibonacci heap to find out the most popular hashtags because it has better theoretical bounds(O(log n)) for increase key operation. 

Read the 'Hashtag Tracker Documentation.pdf' for more details.

Running the program
====================

Program takes the input file name as an argument. Following is an example of how to run the program that read from a file named file_name. 

$ make

$ ./hashtagcounter file_name

I have also uploaded some sample input files where the number represents number of hashtags in that file.

Input Format (Streaming input)
==============================

Hashtags appear one per each line in the input file and starts with # sign. After the hashtag an integer will appear and that is the count of the hashtag (There is a space between hashtag and the integer). You need to increment the hashtag by that count. Queries will also appear in the input file and once a query appears you should append the output to the output file. Query will be an integer number (n) without # sign in the beginning. You should write the top most n hashtags to the output file. Once it reads the stop (without hashtag) program should end. Following is an example of an input file. 

\#saturday 5 

\#sunday 3 

\#saturday 10 

\#monday 2 

\#reading 4 

\#playing_games 2 

\#saturday 6 

\#sunday 8 

\#friday 2 

\#tuesday 2 

\#saturday 12 

\#sunday 11 

\#monday 6 

3 

\#saturday 12 

\#monday 2 

\#stop 3 

\#playing 4 

\#reading 15 

\#drawing 3 

\#friday 12 

\#school 6 

\#class 5

5 

stop 

Output format
============= 

Once a query is appeared you need to write down the most popular hashtags of appeared number in to the output file in descending order. Output for a query should be comma separated list without any new lines. Once the output for a query is finished you should put a new line to write the output for another query. You should produce all the outputs in the output file named “output_file.txt”. 
Following is the output file for the above input file. 

saturday,sunday,monday 
saturday,sunday,reading,friday,monday
