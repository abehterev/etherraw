etherraw
========

Ethernet_RAW

Usage: send_pkt -i <iface> -n <mac_num>

	-i <iface>	interface for packet sending
	-n <mac_num>	number of MAC's

This samll application generate raw packets with different MAC addressess.

example:

```
$ sudo ./send_pkt -i eth0 -n 10

i: 1	mac_end[00:01] full_mac: 00:01:02:03:00:01
i: 2	mac_end[00:02] full_mac: 00:01:02:03:00:02
i: 3	mac_end[00:03] full_mac: 00:01:02:03:00:03
i: 4	mac_end[00:04] full_mac: 00:01:02:03:00:04
i: 5	mac_end[00:05] full_mac: 00:01:02:03:00:05
i: 6	mac_end[00:06] full_mac: 00:01:02:03:00:06
i: 7	mac_end[00:07] full_mac: 00:01:02:03:00:07
i: 8	mac_end[00:08] full_mac: 00:01:02:03:00:08
i: 9	mac_end[00:09] full_mac: 00:01:02:03:00:09
i: 10	mac_end[00:0A] full_mac: 00:01:02:03:00:0A

```

build
=====

You can compile this application in two ways:

    Production mode
    Debug mode

For first case just do `make`.

For second case you can do `make debug`.

For clan "shit" files do `make clean`.

For init state do make `distclean`.

author
======

(C) 2014 by Andrey I. Bekhterev (info@behterev.su)

Released under the GPL (see license file)
