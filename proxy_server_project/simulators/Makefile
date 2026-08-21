CC      = cc
CFLAGS  = -Wall -Wextra -O2 -std=gnu11
LDFLAGS =

COMMON   = common/protocol.c
INCLUDES = -Icommon

.PHONY: all clean selftest

all: origin_tcp origin_pipes origin_fifo test_client

origin_tcp: origin/origin_tcp.c $(COMMON)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ origin/origin_tcp.c $(COMMON) $(LDFLAGS)

origin_pipes: origin/origin_pipes.c $(COMMON)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ origin/origin_pipes.c $(COMMON) $(LDFLAGS)

origin_fifo: origin/origin_fifo.c $(COMMON)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ origin/origin_fifo.c $(COMMON) $(LDFLAGS)

test_client: client/test_client.c
	$(CC) $(CFLAGS) -o $@ client/test_client.c $(LDFLAGS) -pthread

selftest: origin_tcp origin_pipes origin_fifo
	./origin_tcp --selftest
	./origin_pipes --selftest
	./origin_fifo --selftest

clean:
	rm -f origin_tcp origin_pipes origin_fifo test_client origin_shm
	rm -f /tmp/http_proxy_fifo_*
