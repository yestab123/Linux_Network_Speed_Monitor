ALL:
	gcc -O3 -o net_flow net_flow.c -lrt -lm

clean:
	rm net_flow
