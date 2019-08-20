All experiments on a machine with 2 cores no hyper threading and 16GB of RAM running within a container.

# Notes on Key Value store implementations:
 - A completely in memory int based key value store: Takes approx 200us to set and update 1000 values in an int back hash map (just 10 elements), 200ms for 2million.
   Throughput of 10 million ops per second and a latency of 100ns per op.
   One cycle takes 0.3ns which means 300 cycles to actually write a value straight through to memory.
 - Writing to disk on every write - 20 times slower, takes ~4ms to write a thousand updates. (just 10 elements), 4s for 2 million.
   Gives a throughput of 500k ops per second but a latency of 2us per op..

# Reads and writes to c++ map (red black tree):
 - limits: you can make 1 million writes per second to a c++ map (rb tree) (that's 1/10th the speed of writing straight to memory)
         3million reads per second.
 - reads have a significantly smaller constant factor (~1.5) than writes - probably due to the number of cycles it takes to service a store in aggregate as opposed to a read.
   I don't think writes go through the cache on ever write, otherwise surely the factor would be much larger
 - Also can be seen in the notebook where 10pct of reads vs. 10pct of writes show a 2.5ms delay for input size > 500k
 - Using a thread pool is significantly faster - shaves of 10ms of thread init time, thread creation and deletion is expensive, (using pthreads), I implemented a very simple thread pool with a barrier implemented as an atomic counter and a state var that gates the thread task

# Data Serialization Tests
 - Serializing integers
   - Simply write out 4 bytes:
     - The values of the integers don't matter, serializing 1024, 1million and 5billion numbers takes 40us, 34ms and 18ms
     - The varint approach outlined below is 3 times faster when values can fit in a single byte but twice as slow when they require 5 bytes.
   - Varints:
     - I implemented these protobuf style with a leading bit flagging the end of the varint and the rest of the bits concatenated represeting the contents - 4 byte integers can take upto 5 bytes to represent in a varint.
     - Optimized Implementation (Serializing numbers from 1 - size): 
       - 1024 ints 10us, 1 million ints, 10ms, 5 billion ints, 6seconds, 50% reduction in the time by inlining the computation of the individual bytes into one expression. The larger the number - the greater the effect of the optimization.
       - Serializing numbers in the range (0 - 127) that can fit in a byte: 1024, 1million, 5billion times takes 10us, 11ms, 5s
       - Serializing the value (1 << 29) 1024, 1 million and 5 billion times takes 101us, 89ms and 46s respectively.
     - Initial implementation of serializing varints
       - Serializing numbers from 1 - size - 1024 ints take 0.012ms, and 1million ints taking 16ms, 5 billion ints/20GB takes 10 seconds. Network bandwidth is 128MBps, which would take about 3 minutes to stream so this wouldn't necessarily be a bottleneck
       - Serializing numbers 0 - 127 (those that can fit in 1 byte of a varint) 1024, 9us and 1million 9ms and 5 billion 5s (twice the speed)
     - Optimization Approaches:
       - Interestingly collapsing the calculation of size and updating the values had no real effect. Neither does making the if conditions seperate.
       - I reduced the runtime by merging the calculation of the value into a single statement.
     
     
   
   
 - Testing serializations of data strucutres:
  - Binary Trees
   - The tree is serialized as a DFS traversal and the values are dictionary encoded. For a traditional binary tree, you encode the structure. The value at a node is the offset into the data array if the value is not 0 which otherwise denotes a null pointer.
     
# Network experiments
 - The limit is 150MBps (established by reading into and discarding a buffer immediately).


# Useful data analysis note
 - pivot is useful when you have a table of observations and you want to convert a columns of observations

