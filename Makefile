.PHONY: make_build
make_build:
	mkdir build && cd build && cmake .. && make

prerequisites: make_build

target: prerequisites

.PHONY: clean
clean:
	rm -rf build/
