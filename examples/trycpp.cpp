#include <bits/stdc++.h>
#include <pillowtalk.h>

#include "pillowtalk.h"
//#define NULL ((void *)0)
using namespace std;

typedef struct {
    char *event_number;
    pt_node_t *timestamp;
    pt_node_t *event;
    pt_node_t *eventVideoFilename;
    pt_node_t *eventBinFilename;
    pt_node_t *detectedEventValue;
    pt_node_t *detectedViolationValue;
    pt_node_t *longitude;
    pt_node_t *latitude;
    pt_node_t *bearing;
    pt_node_t *speed;
    pt_node_t *altitude;
    pt_node_t *accuracy;
    pt_node_t *azimuth;
    pt_node_t *pitch;
    pt_node_t *roll;
    pt_node_t *bestActivity;
    pt_node_t *filteredActivity;
    pt_node_t *eventCounter;
    pt_node_t *mediaLinkStatus;
    pt_node_t *localFileName;

} event_description;

char *concat(const char *s1, const char *s2);

void create_new_document(char *doc_name);

pt_response_t *get_document(char *doc_name);

event_description *create_new_event(char *event_number);

void push_event_to_doc(char *doc_name, event_description *event);

char *configure_host;

int main() {
    pt_init();


    printf("Enter username:");
    char user[100], password[100];
    scanf("%s", user);

    printf("Enter password:");
    scanf("%s", password);

    configure_host = concat("http://",
                            concat(concat(concat(user, ":"), password), "@localhost:5984/events_db/"));
    printf("%s", configure_host);

    create_new_document("doc1");


    event_description *sample = create_new_event("5");

    sample->timestamp = pt_string_new("2018-05-13-06-39-50-993");
    sample->event = pt_string_new("WIFI_CONNECTION_PROGRESS");
    sample->eventVideoFilename = pt_string_new("null");
    sample->eventBinFilename = pt_string_new("null");
    sample->detectedEventValue = pt_string_new("-10000");
    sample->detectedViolationValue = pt_string_new("0");
    sample->longitude = pt_string_new("-74.755814");
    sample->latitude = pt_string_new("40.251656");
    sample->bearing = pt_string_new("0.000000");
    sample->speed = pt_string_new("0.000000");
    sample->altitude = pt_string_new("-1.000000");
    sample->accuracy = pt_string_new("-1.000000");
    sample->azimuth = pt_string_new("0.000000");
    sample->pitch = pt_string_new("0.000000");
    sample->roll = pt_string_new("0.000000");
    sample->bestActivity = pt_string_new("-1000.000000");
    sample->filteredActivity = pt_string_new("-2.000000");
    sample->eventCounter = pt_string_new("1");
    sample->mediaLinkStatus = pt_string_new("0");
    sample->localFileName =
            pt_string_new("/storage/emulated/0/Android/data/lm.zonarcoach/files/lightmetrics/null");


    push_event_to_doc("doc1", sample);

    pt_cleanup();
    return 0;
}

void push_event_to_doc(char *doc_name, event_description *event) {
    pt_response_t *response = get_document(doc_name);
    printf("Document name:%s  ..  GET  ..  Response code: %ld\n ", doc_name, response->response_code);

    pt_node_t *doc_root = response->root;

    pt_node_t *event_details = pt_map_new();


    pt_map_set(doc_root, event->event_number, event_details);

    pt_map_set(event_details, "timestamp", event->timestamp);
    pt_map_set(event_details, "event", event->event);
    pt_map_set(event_details, "eventVideoFilename", event->eventVideoFilename);
    pt_map_set(event_details, "eventBinFilename", event->eventBinFilename);
    pt_map_set(event_details, "detectedEventValue", event->detectedEventValue);
    pt_map_set(event_details, "detectedViolationValue", event->detectedViolationValue);
    pt_map_set(event_details, "longitude", event->longitude);
    pt_map_set(event_details, "latitude", event->latitude);
    pt_map_set(event_details, "bearing", event->bearing);
    pt_map_set(event_details, "speed", event->speed);
    pt_map_set(event_details, "altitude", event->altitude);
    pt_map_set(event_details, "accuracy", event->accuracy);
    pt_map_set(event_details, "azimuth", event->azimuth);
    pt_map_set(event_details, "pitch", event->pitch);
    pt_map_set(event_details, "roll", event->roll);
    pt_map_set(event_details, "bestActivity", event->bestActivity);
    pt_map_set(event_details, "filteredActivity", event->filteredActivity);
    pt_map_set(event_details, "eventCounter", event->eventCounter);
    pt_map_set(event_details, "mediaLinkStatus", event->mediaLinkStatus);
    pt_map_set(event_details, "localFileName", event->localFileName);


    pt_response_t *put_response = pt_put(concat(configure_host, doc_name), doc_root);
    printf("Document name:%s  ..  PUT  ..  Response code: %ld  (pushing event)event_no:%s\n", doc_name,
           put_response->response_code, event->event_number);
    pt_free_response(put_response);
    pt_free_response(response);


}

event_description *create_new_event(char *event_number) {
    event_description *new_event = (event_description *) calloc(1, sizeof(event_description));
    new_event->event_number = event_number;
    return new_event;
}

void create_new_document(char *doc_name) {
    pt_response_t *response = NULL;
    pt_node_t *dummy_root = pt_map_new();
    response = pt_put(concat(configure_host, doc_name), dummy_root);
    printf("Document name:%s  ..  PUT  ..  Response code: %ld\n ", doc_name, response->response_code);
    pt_free_response(response);
    pt_free_node(dummy_root);
}

pt_response_t *get_document(char *doc_name) {
    pt_response_t *response = NULL;
    response = pt_get(concat(configure_host, doc_name));
    printf("Document name:%s  ..  GET  ..  Response code: %ld\n ", doc_name, response->response_code);
    return response;
}

char *concat(const char *s1, const char *s2) {
    char *result = (char *) malloc(strlen(s1) + strlen(s2) + 1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}