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
 - Varints:
   - These are implemented protobuf style with a leading bit indicating the end of the varint and the rest of the bits concatenated represeting the contents.
   - Initial implementation of serializing varints - 1024 ints take 0.012ms, and 1million ints taking 16ms, 5 billion ints/20GB takes 10 seconds.
   
 - Testing serializations of data strucutres:
  - Binary Trees
   - The tree is serialized as a DFS traversal and the values are dictionary encoded. For a traditional binary tree, you encode the structure. The value at a node is the offset into the data array if the value is not 0 which otherwise denotes a null pointer.
     
# Network experiments
 - The limit is 150MBps (established by reading into and discarding a buffer immediately).


# Useful data analysis note
 - pivot is useful when you have a table of observations and you want to convert a columns of observations

