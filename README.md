NIC_Speed_Show
==============

NIC Network traffic and speed monitor, it can show NIC now send and receive speed.

网卡流量与速率实时监控，可以显示指定网卡的流量和发送与接收速度（KB/s）.

Modified on http://blog.csdn.net/dlutbrucezhang/article/details/9132303

本程序修改于http://blog.csdn.net/dlutbrucezhang/article/details/9132303

Linux / Gcc / C
--------------
Only been tested on Centos 6.5/Centos 6.2

Dependent on /proc/net/dev , and it need has 16 numbers each line.

(Ex.

[root@colud net_flow]# cat /proc/net/dev

Inter-|   Receive                                                |  Transmit
 face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
 
 lo:     106       2    0    0    0     0          0         0      106       2    0    0    0     0       0          0
 
 seth0:3112359898 5614601    0    0    0     0          0         0 2423923875 2853648    0    0    0     0       0          0

gre0:       0       0    0    0    0     0          0         0        0       0    0    0    0     0       0          0

)

Usage
=============
1.make

2.Run ./net_flow NIC_name (default is eth0, you can get on command ifconfig)

Attention:
If seth0: (if has blank after ':' , please use net_flow_s(seth0: 12312), else please use net_flow(seth0:1231).


Demo
==============
[root@colud ~]# ./net_flow_s seth0
Iput: 3431.88MB  59.17KB/s Smooth_Speed:8.08KB/s | Output: 2812.71MB  61.82KB/s Smooth_Speed:8.26KB/s
