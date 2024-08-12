#include <iostream>
#include <curl/curl.h> //including cURL library header

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) { //creating a function to process the incoming data with cURL
    std::string data((char*)contents, size * nmemb); //converts contents to char* and stores it in string called data
    std::cout << "Received data chunk: " << data << std::endl; // Outputs the data to console + message Received data chunk
    ((std::string*)userp)->append(data); //appoints data chunk to string pointed to by userp 
    return size * nmemb;
}

int main() {
    CURL* curl; //defining objects to be used later
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        // Replace 'your_api_key' with your actual API key
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=IBM&apikey=JZAF2F9L3E1K03XR"); //using Alpha Vantage free API, daily time series for IBM Stock 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); //Using callback function used above when data is received
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); //Storing received data into readBuffer
        
        // Disable SSL certificate verification (for testing only) //Needed because I had some problems with SSL certificates
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl); //performing cURl request and storing it in res

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the entire response
            std::cout << "Response data: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
    }
    return 0;
}

