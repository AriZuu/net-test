net-test
========

Picoos-net test programs for different boards.
Ethernet address that is compiled into test is
00-bd-3b-33-04-d5. When test is compiled for IPv6,
system can be accessed with link-local address
fe80::2bd:3bff:fe33:4d5%ifname or if your network
has IPv6 autoconfiguration correctly setup a
address is assigned with your network prefix and
host id of board, resulting in something like
xxxx:yyyy::2bd:3bff:fe33:4d5, where xxxx:yyyy... is your
network prefix.

It is possible to access test system with telnet (a simple
command shell should respond) or by ping.

If compiled with IPv4 test system is (currently) hard-coded to
IP address 192.168.60.213/24.

