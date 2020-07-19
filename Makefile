compile:
	gcc generator.c utils.c -o generate_raw_image
	gcc core.c utils.c -pthread -o gopro_find_white
clean:
	rm -f generate_dummy
	rm -f overexpose
	rm -f input.txt
	rm -f output.txt

install:
	TODO

generate:
	./generate_raw_image
run:
	./gopro_find_white
