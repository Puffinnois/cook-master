#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

int main() {
    FILE *fp;
    char buffer[1024];
    fp = fopen("recipe.json", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    struct json_object *parsed_json = json_tokener_parse(buffer);
    struct json_object *title_obj;
    struct json_object *ingredients_arr;

    if (!json_object_object_get_ex(parsed_json, "title", &title_obj)) {
        fprintf(stderr, "Failed to get 'title' field from JSON object\n");
        return 1;
    }
    if (!json_object_object_get_ex(parsed_json, "extendedIngredients", &ingredients_arr)) {
        fprintf(stderr, "Failed to get 'extendedIngredients' field from JSON object\n");
        return 1;
    }
    if (json_object_get_type(title_obj) != json_type_string) {
        fprintf(stderr, "'title' field is not a string\n");
        return 1;
    }
    if (json_object_get_type(ingredients_arr) != json_type_array) {
        fprintf(stderr, "'extendedIngredients' field is not an array\n");
        return 1;
    }

    const char *title = json_object_get_string(title_obj);
    printf("Title: %s\n", title);

    printf("Ingredients:\n");
    int ingredients_len = json_object_array_length(ingredients_arr);
    for (int i = 0; i < ingredients_len; i++) {
        struct json_object *ingredient_obj = json_object_array_get_idx(ingredients_arr, i);
        const char *ingredient = json_object_get_string(ingredient_obj);
        printf("- %s\n", ingredient);
    }

    return 0;
}
