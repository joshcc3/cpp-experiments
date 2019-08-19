# jjvm\n\nAn implementation of the jvm in C++ as a learning exercise


# Notes on Key Value store
 - In memory, Takes approx 200us to set and update 1000 values in an int back hash map (just 10 elements), 200ms for 2million.
   Throughput of 10 million ops per second and a latency of 100ns per op.
   One cycle takes 0.3ns which means 300 cycles to actually write a value straight through to memory.
 - Writing to disk on every write - 20 times slower, takes ~4ms to write a thousand updates. (just 10 elements), 4s for 2 million.
   Gives a throughput of 500k ops per second but a latency of 2us per op..

# Reads and writes to map:
 - limits: you can make 1million writes per second to a c++ map (rb tree) (that's 1/10th the speed of writing straight to memory)
         3million reads per second.
 - reads have a significantly smaller constant factor (~1.5) than writes - probably due to the number of cycles it takes to service a store in aggregate as opposed to a read.
   I don't think writes go through the cache on ever write, otherwise surely the factor would be much larger
 - Also can be seen in the notebook where 10pct of reads vs. 10pct of writes show a 2.5ms delay for input size > 500k


# Useful data analysis note
 - pivot is useful when you have a table of observations and you want to convert a column of observations
   into a columns of your table.

