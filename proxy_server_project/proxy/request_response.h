#ifndef REQUEST_RESPONSE_H
#define REQUEST_RESPONSE_H
#include "server_client_signal.h"
#include <errno.h>

int response_writer_send(struct client *c);
int request_validator_validate(struct client *c);
int request_reader_handle(struct client *c);
int request_parser_parse(struct client *c);
int request_filter_check(struct client *c);

#endif