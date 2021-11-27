#include <stdio.h>
#include <coap3/coap.h>

#include "common.h"

void handle_request(coap_resource_t* resource, coap_session_t* session, const coap_pdu_t* request, const coap_string_t* token, coap_pdu_t* response) {
	int ec;
	size_t len;
	uint8_t* data;

	ec = coap_get_data(request, &len, (const uint8_t**) &data);
	if (ec != 1) {
		coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_ACCEPTABLE);
		return;
	}
	if (len <= 0) {
		coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_ACCEPTABLE);
		return;
	}

	coap_show_pdu(LOG_INFO, request);

	coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
}

int main(int argc, char const *argv[]) {
	int ec;
	coap_context_t* context;
	coap_address_t address;
	coap_endpoint_t* endpoint;
	coap_resource_t* resource;

	coap_startup();
	coap_set_log_level(LOG_DEBUG);

	ec = resolve_address("127.0.0.1", "5683", &address);
	if (ec < 0) {
		printf("Could not resolve bind address!\n");
		return 1;
	}

	context = coap_new_context(NULL);
	if (!context) {
		printf("Could not create CoAP context!\n");
		return 1;
	}

	coap_context_set_block_mode(context, COAP_BLOCK_USE_LIBCOAP | COAP_BLOCK_SINGLE_BODY);

	endpoint = coap_new_endpoint(context, &address, COAP_PROTO_UDP);
	if (!endpoint) {
		printf("Could not create CoAP endpoint!\n");
		return 1;
	}

	resource = coap_resource_init(coap_make_str_const(""), 0);
	coap_register_handler(resource, COAP_REQUEST_PUT, handle_request);
	coap_add_resource(context, resource);

	while (1) {
		coap_io_process(context, COAP_IO_WAIT);
	}

	return 0;
}