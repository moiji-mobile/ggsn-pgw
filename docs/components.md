== Main components

=== Load balancer

Simple front-end GTP-C UDP receiver that fills a round-robing ZeroMQ
queue. Will drop messages if queue size overflows.


=== Front-end GTP-C

Receive GTP-C from the load-balancer, alternatively with direct GTP-C
receiver. Most likely using in-memory ZeroMQ queue.


===  Worker code

Will parse GTP-C, will make checks, will delegate balance check and to
be used IP addresses. For messages that should have gone to a different
GSN the system will re-queue them.
After modifying a tunnel the reply will be sent back the message to the
load-balancer to the local front-end to send a response.

=== GTP-U manager

The worker will co-operate with the GTP-U manager to add the tunnel on
one or more systems (e.g. with keepalived). It is being used as request/reply
with the worker.


=== GTP-U restorer

Before starting the system and everybody connects. All active tunnels
will be restored. E.g. because of a reboot the node is not up-to-date
with the tunnels that should exist. The tool will check for all pending
updates since a specific version. This will be commands for add, update
or remove a tunnel.
