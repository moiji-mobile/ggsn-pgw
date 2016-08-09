Scalable, flexible and redundant GGSN/PGW

The moiji-mobile.com GGSN/PGW is a scalable, flexible and redundant GGSN/PGW
implementing GTP-C version 1 and 2. It is using the in-kernel GTP-U support
to provide fast switching/routing of the userplane. It is meant to be either
deployed on raw hardware or cloud infrastructure and consists out of several
parts that are connected using ZeroMQ.


Scalable:

There can be one or more front-end GTP-C handlers to dispatch GTP-C requests
to serving gateway nodes. ZeroMQ will handle dispatching requests in a round
robin manner.


Flexible:

Worker processes that check context activation requests can delegate the
checking allowing to use scripts, REST calls, RADIUS/DIAMETER calls, etc.
The Free Software nature and the architecture of this GGSN/PGW makes the
adaption to different backends easy.


Redundant:

A serving GGSN/PGW can be organized to use VRRP. A working node is paired with
one or more other working nodes and will apply the same GTP-U setting on
each system. If a working node fails, the other system will be able to take
over. A restored system will be able to apply all pending GTP-U configuration
before becoming active again.
