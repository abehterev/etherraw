etherraw
========

Ethernet_RAW

Usage: send_pkt -i `<iface>` -n `<mac_num>`

	-i <iface>	interface for packet sending
	-n <mac_num>	number of MAC's
	-s		work in slow mode
	-r 		generate random Src MAC for each new packet

This is small application that generate raw-packets with different MAC addresses.

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

how to build
=========

You can compile this application in two ways:

 * Production
 * Debug

First of all you can just do `make`.

If you need debug you may build as `make debug`.

For cleaning just do `make clean`.

And for recover in virgin state do `make distclean`.

author
======

(C) 2014 by Andrey I. Bekhterev (info@behterev.su)

Released under the GPL (see license file)
