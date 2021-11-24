# side-channel-probing

probe1.c - a simple probe program that periodically measures the memory access time of the data pointed to by numbers[1], numbers[5] and numbers[6] as well as the code of the placenum function in the shared library. This program makes 500 measurements, stores them in a .csv file, and then you can use plotter.py to create a graphical plot of the measurement results.

probe2.c - a simple probe program that detects if some process uses the placenum function of the shared library.
The program can be tested by starting probe and then concurrently
- run “test 1” or “test 0”
- run “web 8888 .” and point your browser to “http://localhost:8888/1357.png” (or some other number instead of 1357).

autoprobe.c - continuously monitors the shared library’s memory. It detects if a client accesses the web server to generate a png file with digits, and print which digits are shown in the image.

Example output: “Access detected! digits: [1], [5], [7], [9]”