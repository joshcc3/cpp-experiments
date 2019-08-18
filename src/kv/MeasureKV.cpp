
template<class A>
pair<int, A> time_it(const string& e, function<A()> f) {
    auto start = chrono::high_resolution_clock::now();
    A result = f();

    auto end = chrono::high_resolution_clock::now();
    int count = (chrono::duration_cast<chrono::microseconds>(end - start)).count();
    cout << "Experiment " << e << " took: " << count / 1000.0 << "ms" << endl;

    return pair<int, A>(count, result);
}

class X {
public:
    int z;
};
template<class A, class B>
function<list<B>(list<A>&)> cmap(function<B(A)> f) {
    return [&f](list<A> l) {
        list<B> result = l;
        for(auto it = result.begin(); it != result.end(); it++) {
            *it = *it * *it;
        }
        return result;
    };
}

template<class A, class B>
function<void(list<A>&)> cmapWithRef(function<B(A)> f, list<B>& result) {
    return [&f, &result](list<A> l) {
        for(auto it = l.begin(); it != l.end(); it++) {
            result.push_back(*it * *it);
        }
    };
}

template<class A, class B>
function<void(list<A>&)> cmapInPlace(function<B(A)> f) {
    return [&f](list<A> l) {
        for(auto it = l.begin(); it != l.end(); it++) {
            *it = *it * *it;
        }
    };
}

template<class A, class B, class C>
function<function<C(A)>(function<C(B)>)> compose(function<B(A)> f) {
    function<function<C(A)>(function<C(B)>)> res = [&f](function<C(B)> g) {
        function<C(A)> innerRes = [&](A a) {
            return g(f(a));
        };
        return innerRes;
    };
    return res;
}


int measureMap() {

//    cout <<  (*xs.begin())->z << endl;
//    (*xs.begin())->z = 321;
//    cout <<  (*xs.begin())->z << " " << x->z << endl;

    function<int(int)> f1 = [](int x) -> int {
        return x * x;
    };
    list<int> l;
    list<int> listResult;
    function<int()> interesting = [&]() {
        cmap(f1)(l);
        return 1;
    };

    function<int()> interesting2 = [&]() {
        cmapWithRef(f1, listResult)(l);
        return 1;
    };

    function<int()> mapInPlace = [&]() {
        cmapInPlace(f1)(l);
        return 1;
    };

//    for(int size = 1000; size < 100000000; size *= 10) {
//        stringstream experimentName;
//        experimentName << "Map experiment With Ref " << size;
//        l.clear();
//        function<int()> initFun = [&l, size]() -> int {
//            for (int i = 0; i < size; i++) {
//                l.push_back(i);
//            }
//            return 1;
//        };
//        stringstream initName;
//        initFun();
//        time_it(experimentName.str(), interesting);
//    }
//    cout << "############" << endl;
//    this_thread::sleep_for(5s);
    for(int size = 1000; size < 100000000; size *= 10) {
        stringstream experimentName;
        experimentName << "Map experiment inplace " << size;
        l.clear();
        listResult.clear();
        function<int()> initFun = [&l, size]() -> int {
            for (int i = 0; i < size; i++) {
                l.push_back(i);
            }
            return 1;
        };
        stringstream initName;
        initName << "Init function " << size;
        initFun();
        time_it(experimentName.str(), mapInPlace);
    }
}
