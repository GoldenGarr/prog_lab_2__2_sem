#include <iostream>
#include <curl/curl.h>
#include <stdio.h>

using namespace std;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main() {
    CURL *curl = curl_easy_init();
    FILE * file = fopen("file.js", "wb");

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode response = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(file);
}
