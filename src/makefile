minurl:./*.cpp 
		g++ $^ -o minurl -std=c++17 -lhiredis -lpthread  -I ../_include

.PHONY:clean
clean:
	-rm -rf $(objs)
	-rm -rf minurl
