compile:
	gcc generator.c utils.c -o generate_dummy
	gcc main.c utils.c -o overexpose
clean:
	rm generate_dummy
	rm overexpose

install:
	TODO
