compile:
	gcc generator.c utils.c -o generate_raw_image
	gcc core.c utils.c -o gopro_find_white
clean:
	rm -f generate_dummy
	rm -f overexpose
	rm -f input.txt
	rm -f output.txt

install:
	TODO

run: clean
	./generate_raw_image
	./gopro_find_white
