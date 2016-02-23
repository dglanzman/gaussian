gaussian: gaussian.c
	cc -lm -o gaussian -O3 gaussian.c

debug: gaussian.c
	cc -lm -o gaussian -O1 -g gaussian.c

clean:
	rm -f gaussian
