.PHONY: all build clean run

all: build run image

build:
	cmake -S . -B build
	cmake --build build

run: build
	@mkdir -p images
	./build/raytracing

image:
	ffmpeg -y -i ./images/out.ppm ./images/out.png

clean:
	rm -rf build
