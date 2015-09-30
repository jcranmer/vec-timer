CXX := g++
CXXFLAGS := -O3 -std=c++11 -fno-exceptions -march=native
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

%.ll: %.cpp
	clang++ $(CXXFLAGS) -S -emit-llvm -o $@ $^

exe-%: timer.o %.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test-%: exe-%
	rm -rf $*.csv
	for i in `seq 1 20`; do ./$< $*.csv; done
