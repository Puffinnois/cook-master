#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define API_KEY "13ffcd238a97446caf2ee7851baba246"

int main(void) {
    CURL *curl;
    CURLcode res;
    char *url = "https://api.spoonacular.com/recipes/716429/information?includeNutrition=false&apiKey=" API_KEY;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        FILE *recipe = fopen("recipe.json", "w");
        if (!recipe) {
            printf("Failed to open recipe.json for writing\n");
            return 1;
        } else {
            printf("recipe.json opened for writing\n");
        }
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, recipe);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        fclose(recipe);
    }
    return 0;
}