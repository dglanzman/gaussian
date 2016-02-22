gaussian: gaussian.c
	cc -lm -o gaussian -O3 gaussian.c

clean:
	rm -f gaussian
