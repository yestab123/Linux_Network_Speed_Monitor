ALL:
	gcc -g -o net_flow net_flow.c -lrt -lm
	gcc -g -o net_flow_s net_flow_s.c -lrt -lm

clean:
	rm net_flow net_flow_s
