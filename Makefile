ALL:
	gcc -O3 -o network_monitor monitor_main.c -lrt -lm

clean:
	rm network_monitor
