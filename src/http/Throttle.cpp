
class Clock {
    int ticks;
public:
    Clock(): ticks(0) {}
    Clock(int t): ticks(t) {}
    Clock(const Clock& c) { ticks = c.ticks; }
    Clock(Clock&& c) { ticks = c.ticks; }
    Clock operator-(Clock c) {
        return Clock(ticks - c.ticks)
    }
};

template <class T>
class CircularBuffer<T> {
    const T** arr;
    const int capacity;
    int size;
    int start;
public:
    CircularBuffer(int c): capacity(c), size(0), start(0) {
        arr = new char[sizeof(T) * capacity];
    }

    int size() {
        return size;
    }

    const T*& operator[](int ix) {
        return (const T**)arr[(start + ix)%capacity];
    }

    const T* operator[](int ix) {
        return (const T**)arr[(start + ix)%capacity];
    }

    const T* dequeue() {
        const T* result = arr[start++%capacity];
        start = start%capacity;
        return result;
    }
};

class HitCounter {
    int totalCount;
    CircularBuffer<int> buffer;
    Clock lastUpdated;
public:

    HitCounter(): totalCount(0), buffer(), lastUpdated() {}

    void slide(const Clock& c) {
        Clock newTime(c - buffer.size() + 1);
        int newWindowStart = min((newTime - lastUpdated).ticks, buffer.size());
        for(int i = 0; i < newWindowStart; i++) {
            totalCount -= buffer.dequeue();
        }
        buffer[-1] , &(*buffer.get(-1, 0) + 1));
        lastUpdated = newTime;
    }

    int count() {
        return totalCount;
    }

    const int& operator[](int index) {
        return buffer[index];
    }
};



class Throttler {

    HitCounter hc;

public:

    Throttler() {
        hc = HitCounter(WINDOW, Clock());
    }

    bool throttleRequest(int id, const Clock& c) {
        hits.slide(c);
        if(hits.count() > THRESHOLD) {
            return false;
        } else {
            hits[-1] += 1;
            return true;
        }
    }
};


void testThrottleBase() {
    // clock starts at time 61
    Clock c(61);
    // 5 requests every 60 ticks
    Throttler t(5, 60);
    for(int j = 0; j < 5; j++) {
        c += 10;
        for (int i = 0; i < 10; i++) {
            assert t.throttleRequest(i, c);
        }
    }
    c += 1;
    for (int i = 0; i < 10; i++) {
        assert !t.throttleRequest(i, c);
    }

}

int main() {
    testThrottleBase();
}