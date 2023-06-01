#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

void parse_ingredient(json_object * jobj) {
    enum json_type type;
    json_object_object_foreach(jobj, key, val) {
        type = json_object_get_type(val);
        if (type == json_type_string && strcmp(key, "name") == 0) {
            printf("- %s\n", json_object_get_string(val));
        }
    }
}

void parse_json(json_object * jobj) {
    enum json_type type;
    json_object_object_foreach(jobj, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_string:
                if(strcmp(key, "title") == 0) {
                    printf("Title: %s\n", json_object_get_string(val));
                }
                break;
            case json_type_array:
                if(strcmp(key, "extendedIngredients") == 0) {
                    printf("Ingredients:\n");
                    for(int i=0; i < json_object_array_length(val); i++){
                        json_object * array_item = json_object_array_get_idx(val, i);
                        parse_ingredient(array_item);
                    }
                }
                break;
            case json_type_object:
                parse_json(val);
                break;
        }
    }
}

void parse_instructions(json_object * jobj) {
    enum json_type type;
    json_object_object_foreach(jobj, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_array:
                if(strcmp(key, "steps") == 0) {
                    printf("Instructions:\n");
                    for(int i=0; i < json_object_array_length(val); i++){
                        json_object * array_item = json_object_array_get_idx(val, i);
                        json_object_object_foreach(array_item, step_key, step_val) {
                            if (json_object_get_type(step_val) == json_type_string && strcmp(step_key, "step") == 0) {
                                printf("- %s\n", json_object_get_string(step_val));
                            }
                        }
                    }
                }
                break;
            case json_type_object:
                parse_instructions(val);
                break;
        }
    }
}

int main() {
    FILE *fp;
    char buffer[2048];
    fp = fopen("recipe.json", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[read_size] = '\0';  // Ensure the string is null-terminated
    fclose(fp);

    json_object * jobj = json_tokener_parse(buffer);
    parse_json(jobj);
    parse_instructions(jobj);
    return 0;
}
   
 