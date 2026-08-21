# Three Origin Servers (C) — for student proxy debugging

Students compile these C programs on Linux or macOS and use them as the
**origin** side while they implement the multiplexed HTTP proxy.

```bash
cd simulators
make
make selftest
```

| Binary | Communication | Process model |
|--------|---------------|---------------|
| `origin_tcp` | TCP on **127.0.0.1** (loopback) | Independent listener; proxy `connect()`s up to 10 channels |
| `origin_pipes` | 20 pipes = 10 send/recv pairs | Proxy parent creates pipes, forks origin child |
| `origin_fifo` | 20 FIFO files = 10 pairs | **No hierarchy** — independent processes |
| `test_client` | TCP to student proxy | Separate tester |

---

## Common HTTP simulation

All three origins parse the same simplified HTTP-like requests and generate
the same canned responses (`/index`, `/data`, `/user`, POST, OPTIONS, …).
See `common/protocol.c`.

**Framing (important):**
- Use only `\n` as the newline (no `\r\n`)
- Headers end / header–body division: `\n\n`
- Body length still comes from `Content-Length`

Example request:

```text
GET /index HTTP/1.1
Content-Length: 0
Connection: keep-alive

```

Example response:

```text
HTTP/1.1 200 OK
Content-Length: 14
Connection: keep-alive

Hello, client
```

---

## 1) TCP loopback — `origin_tcp`

Proxy talks to the origin over **IPv4 loopback**:

```text
Proxy  -- TCP connect -->  127.0.0.1:9000  <-- listen --  Origin
```

### Proxy responsibilities

- Start `./origin_tcp` (or run it in another terminal)
- Create up to **10** TCP connections to `127.0.0.1:PORT`
- Each connection = one bidirectional channel (send request, read response)
- Watch those sockets in the same `select()` / `poll()` loop as clients

### Run

```bash
./origin_tcp           # 127.0.0.1:9000
./origin_tcp 9000
./origin_tcp --selftest
```

### Connect sketch

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */
addr.sin_port = htons(9000);
connect(fd, (struct sockaddr *)&addr, sizeof(addr));
```

If all 10 channels are busy, the origin accepts then replies `503` and closes.

---

## 2) Pipes — `origin_pipes`

### Proxy must create

- 10 pairs of pipes (20 pipe ends total):
  - `to_origin[i]` — proxy writes requests, origin reads
  - `from_origin[i]` — origin writes responses, proxy reads
- `fork()` child
- In child: close unused ends, `dup2` origin ends to fixed fds, `exec` origin
- In parent: close unused ends; `poll`/`select` on proxy ends

### Default fd map in the origin child

```text
pair i:  read_fd  = 100 + 2*i     # requests
         write_fd = 100 + 2*i + 1 # responses
```

### Run

```bash
./origin_pipes                 # expect fds 100..119
./origin_pipes --base-fd 100
./origin_pipes --pairs 3:4,5:6,7:8,...
./origin_pipes --selftest
```

### Child exec sketch

```c
for (int i = 0; i < 10; i++) {
    dup2(to_origin_read[i],  100 + 2 * i);
    dup2(from_origin_write[i], 100 + 2 * i + 1);
}
execl("./origin_pipes", "origin_pipes", "--base-fd", "100", (char *)NULL);
```

---

## 3) FIFOs — `origin_fifo`

Same pair idea as pipes, but with **named FIFO files**.
Proxy and origin are **separate processes** (no required parent/child).

### Proxy must create (mkfifo)

For `i = 0 .. 9`:

```text
/tmp/http_proxy_fifo_<i>_c2s   # proxy → origin (origin reads)
/tmp/http_proxy_fifo_<i>_s2c   # origin → proxy (origin writes)
```

```bash
./origin_fifo
./origin_fifo --prefix /tmp/http_proxy_fifo
./origin_fifo --selftest
```

---

## Test client (TCP → your proxy)

```bash
./test_client 127.0.0.1 8080 scenario all
./test_client 127.0.0.1 8080 get /index
./test_client 127.0.0.1 8080 forbidden
./test_client 127.0.0.1 8080 eleven
```

---

## Source tree

```text
simulators/
  Makefile
  README.md
  common/protocol.h
  common/protocol.c
  origin/origin_tcp.c
  origin/origin_pipes.c
  origin/origin_fifo.c
  client/test_client.c
  examples/proxy_attach_snippets.c
  samples/
```
