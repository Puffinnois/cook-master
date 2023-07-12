// how to compile : gcc -o main main.c -ljson-c -lcurl `pkg-config --cflags --libs gtk+-3.0`
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h> // sudo apt install libjson-c-dev
#include <curl/curl.h> 
#include <gtk/gtk.h> // sudo apt install libgtk-3-dev

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

int parse_json(json_object * jobj, const char* key, GtkTextBuffer *buffer) {
    enum json_type type;
    int key_found = 0; // 0 for false, 1 for true
    GtkTextIter end;
    
    json_object_object_foreach(jobj, key_iter, val_iter) {
        type = json_object_get_type(val_iter);
        switch (type) {
            case json_type_boolean: 
            case json_type_double: 
            case json_type_int: 
            case json_type_string:
                if(strcmp(key_iter, key) == 0) {
                    char text[512];
                    snprintf(text, sizeof(text), "Key: %s, Value: %s\n", key_iter, json_object_to_json_string(val_iter));
                    gtk_text_buffer_get_end_iter(buffer, &end);
                    gtk_text_buffer_insert(buffer, &end, text, -1);
                    key_found = 1;
                }
                break; 
            case json_type_object:
                key_found |= parse_json(val_iter, key, buffer);
                break;
            case json_type_array:
                if(strcmp(key_iter, key) == 0) {
                    for(int n = 0; n < json_object_array_length(val_iter); n++) {
                        json_object * array_item = json_object_array_get_idx(val_iter, n);
                        json_object *aisle, *amount;
                        if(json_object_object_get_ex(array_item, "aisle", &aisle) &&
                            json_object_object_get_ex(array_item, "amount", &amount)) {
                            char text[512];
                            snprintf(text, sizeof(text), "Aisle: %s, Amount: %f\n", json_object_get_string(aisle), json_object_get_double(amount));
                            gtk_text_buffer_get_end_iter(buffer, &end);
                            gtk_text_buffer_insert(buffer, &end, text, -1);
                            key_found = 1;
                        }
                    }
                }
                else {
                    for(int n = 0; n < json_object_array_length(val_iter); n++) {
                        json_object * array_item = json_object_array_get_idx(val_iter, n);
                        if(json_object_get_type(array_item) == json_type_object) {
                            key_found |= parse_json(array_item, key, buffer);
                        }
                    }
                }
                break;
        }
    }
    return key_found;
}  


int main(int argc, char **argv) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "JSON Parser");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create a new text view widget with a scrolled window
    GtkWidget *textView = gtk_text_view_new();
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), textView);
    gtk_container_add(GTK_CONTAINER(window), scrolledWindow);

    // Get the text buffer for the text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    // Show the window and all its children widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
