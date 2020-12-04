#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>

using namespace std;

string getHttps(char *url);

int main(){
    printf("Download Google Started! \n");
    string mystr = getHttps("https://www.google.com");
    ofstream myfile;
    myfile.open ("../../google.html");
    myfile << mystr;
    myfile.close();
    printf("Finised downloading google, you can open google.html now! \n");
    return 0;
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string getHttps(char *url) {
    string readBuffer;
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        //301 moved
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }else{
        printf("CURL ERROR");
    }
    curl_global_cleanup();
    return readBuffer;
}
