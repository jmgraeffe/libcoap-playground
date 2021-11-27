#include <stdio.h>
#include <string.h>
#include <coap3/coap.h>

#include "common.h"

coap_response_t handle_response(coap_session_t *session, const coap_pdu_t *sent, const coap_pdu_t *received, const coap_mid_t mid) {
	int ec;
	size_t len;
	uint8_t* data;

	coap_show_pdu(LOG_INFO, received);

	int* ack_received = (int*) coap_session_get_app_data(session);
	*ack_received = 1;

	return COAP_RESPONSE_OK;
}

int main(int argc, char const *argv[]) {
	int ec;
	int rc = 0;
	coap_context_t* context = NULL;
	int ack_received;
	coap_address_t address;
	coap_session_t* session = NULL;
	coap_pdu_t* pdu;

	coap_startup();
	coap_set_log_level(LOG_DEBUG);

	ec = resolve_address("127.0.0.1", "5683", &address);
	if (ec < 0) {
		printf("Could not resolve remote address!\n");

  		rc = 1;
  		goto cleanup;
	}

	context = coap_new_context(NULL);
	if (!context) {
		printf("Could not create CoAP context!\n");

  		rc = 1;
  		goto cleanup;
	}

	coap_register_response_handler(context, handle_response);

  	session = coap_new_client_session(context, NULL, &address, COAP_PROTO_UDP);
  	if (!session) {
  		printf("Could not create CoAP session!\n");

  		rc = 1;
  		goto cleanup;
  	}

  	coap_session_set_app_data(session, &ack_received);

  	for (int i = 0; i < 10; ++i) {
		pdu = coap_pdu_init(COAP_MESSAGE_CON, COAP_REQUEST_CODE_PUT, coap_new_message_id(session), coap_session_max_pdu_size(session));
	  	if (!pdu) {
	  		printf("Could not create CoAP PDU!\n");

	  		rc = 1;
	  		goto cleanup;
	  	}

	  	// dummy data
	  	uint8_t data[16];
	  	const size_t len = sizeof(data) / sizeof(data[0]);
		memset(data, i, len);
	  	
	  	//ec = coap_add_data(pdu, len, data);
	  	ec = coap_add_data(pdu, len, data);
	  	if (!ec) {
	  		printf("Could not add data to CoAP PDU!\n");

	  		rc = 1;
	  		goto cleanup;
	  	}

	  	ack_received = 0;
	  	coap_send(session, pdu);
	  	while (!ack_received) coap_io_process(context, COAP_IO_WAIT);
  	}

cleanup:
	if (session != NULL) coap_session_release(session);
	if (context != NULL) coap_free_context(context);
	coap_cleanup();

	return rc;
}