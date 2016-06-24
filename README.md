Linux_Network_Speed_Monitor
==============

NIC Network traffic and speed monitor, it can show NIC instantaneous speed for send and recv on KB/s.
网卡流量与速率实时监控，可以显示指定网卡的流量和发送与接收速度（KB/s）.

Data reading algorithms base on http://blog.csdn.net/dlutbrucezhang/article/details/9132303
本程序数据读取算法基于http://blog.csdn.net/dlutbrucezhang/article/details/9132303

Linux / Gcc / C
--------------
Only been tested on Centos 6.5/Centos 6.2/Ubuntu (You can use in Linux System)

Usage
=============
1.make

2.Run ./net_flow NIC_name (default is eth0, you can get on command ifconfig)


Demo
==============
[root@colud ~]# ./net_flow seth0

Input: 3431.88MB  59.17KB/s Smooth_Speed:8.08KB/s | Output: 2812.71MB  61.82KB/s Smooth_Speed:8.26KB/s
