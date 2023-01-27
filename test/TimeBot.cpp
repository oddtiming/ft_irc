#include <curl/curl.h>

#include <arpa/inet.h>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

const int PORT = 6667;
const string SERVER_PASSWORD = "pass";
const string NICKNAME = "MyBot";
const string USERNAME = "MyBot";
const string REALNAME = "MyBot";
const string CHANNEL = "#default";

// int run_translator() {

//     // Initialize cURL
//     CURL *curl = curl_easy_init();
//     if (!curl) {
//         std::cerr << "Error initializing cURL" << std::endl;
//         return 1;
//     }

//     // Set the API endpoint and API key
//     const std::string endpoint = "https://translation.googleapis.com/language/translate/v2";
//     const std::string api_key = "-----BEGIN PRIVATE
//     KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDaPH5q9GAG4ax7\nauJMeEP/ZNsGtbqin+9wADzvM+iW+t7E86hiV551qwbp3EyIWVjLM8V9ogMHyisQ\noy0iaFncYdHFGiqzpB2Zaww50uyhKrm639vR/TwLhE0A1bsLfI/h1gdGcmze5d5g\nF7AvYx1c14/ZlBPOzAOoHuMN0xmGSCL+gSde0AX6pUd1voc4DVsICI0Gfx2WGV+S\n1DJYQ1UBoSyjZTJ9ZXDGOCuF8VkIhUEqYC6gPA1gew0xRW+Ioi7PnRNVEITP8EbH\n8BmiEmDdOMGTPItR69z14xgNLxGyEY1DKdxgih3bz7hfyChgiGFYoOJc/TVTO1Ny\n0DwxB1cbAgMBAAECggEAZcRp6zQ0B2F7Tj4kcyZuBw2f+XwLIUTVofPqdl1NbAJm\nmY9KCTn0och0O9/ILJkFiBSHVrOTGDHVbTe6l13z8YoICRLPsN+qEd3SYLCUOf+z\nhopXdo/RTUmPowEH7S5T5ispT1eSn0qwLT0+oopkqeO3p+2eYwPN/LsShGLZdz0V\n2w3fNqFa48wdKnBny8iSa00iei0S62YR+Fr6LZq2YPQB0JWnh26AedF0rZXNcS9y\npEFESWqnla/NirRVFmAWcAkBTOpWY3eUr/Bx25oot/aTx5b9/v8IVGbsqRI3tnoL\noSsydD5E/E82Gd+sAzT6gCrLlsMDXta0zpX1Pq622QKBgQDx82cUaT8L/K1fULFK\njxA6Yb4gnTsckcS4d7wc825uN7fH7GChLfwU/3bhkKidBg/4USjQw0uGsEVTort9\n3T63PUBRlltdBnBf8IcLCT8g3bw8j6sr4ImcnaBPncodHspEHZmaMycBhBtaaNZX\njTxdFCMwGLwQzOxRhNw/ZLBBtQKBgQDm6JNOwZey2x8PTRZmKj8TVOo4FFRr5kZz\ngHpDJHl2M0/1pLge+d5ZHkicMzvtHSWurZMR8qCWXdr6bhSJwC+3q0vIZ7O2SwDk\noCo97umLygyM79e5Rk1fuF90sYM4333ZwHEKr7jtj/PPBAXC9oVf0tNCBc9AiZu/\n1cE55pn3jwKBgQDBxNN670cEzJn0yeUS9zxh5UGlUQa0PLG1on7PGM4KCyluPYqt\nyuwlzOpif73p8576mdT8TEvCVI1RC/ToOjYt/inyrV4CRFPfIfylBATAx5QnVFAb\n1bmf6kUnPhFN/Qi0QJhxfCHy/uABKhDvETeUu7nqnQSQ1+0y5BqX0oWuOQKBgEVU\nHS5vR7nGupugfJIuKwQF4dD2oaZH3G0/bPYN1H4dx+ssCrAaO50sT4a+h0aZEsBQ\nkzC3VWnwQhggxv/Y+Tq/LqC8KiQYfusl/6jau7ph7sS+Glohdoe/1sE1rv17dNDV\nS9t2iSKirwQ/4+72j3MG3LEdB24MIvxFFJ/q0d2XAoGAN00pcBHSqRLacUQwBp+i\nxaMPSGIovMsfjV2LtmGzE6egyx46rGLp8Jc0rNoR/Ks0g/FEqJaMe4L1yjILlFsV\nf2GEn92t859p1iw9+uVJzAnWmcWRHoKl30GL9s6in3caoNTbRBPa2pYB8q3yFpfn\n0OS4DN7j/njGabeAqYLD3jg=\n-----END
//     PRIVATE KEY-----\n";

//     // Set the text to be translated and the target language
//     const std::string text = "Hello, world!";
//     const std::string target_language = "fr";

//     // Build the request URL
//     std::string url = endpoint + "?key=" + api_key + "&q=" + text + "&target=" + target_language;

//     // Set the URL and perform the request
//     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//     CURLcode res = curl_easy_perform(curl);
//     if (res != CURLE_OK) {
//         std::cerr << "Error making request: " << curl_easy_strerror(res) << std::endl;
//         curl_easy_cleanup(curl);
//         return 1;
//     }

//     // Get the response
//     std::string response;
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &response);
//     // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//     res = curl_easy_perform(curl);
//     if (res != CURLE_OK) {
//         std::cerr << "Error getting response: " << curl_easy_strerror(res) << std::endl;
//         curl_easy_cleanup(curl);
//         return 1;
//     }
//     std::cout << "Response successfully received: '" << res << std::endl;

//     // Clean up
//     curl_easy_cleanup(curl);
//     return 0;
// }
// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// #include "google/cloud/translate/translation_client.h"
// #include "google/cloud/project.h"
// #include <iostream>

// auto constexpr kText = R"""(
// Four score and seven years ago our fathers brought forth on this
// continent, a new nation, conceived in Liberty, and dedicated to
// the proposition that all men are created equal.)""";

// int run_translator(int argc, char* argv[]) try {
//   if (argc < 2 || argc > 3) {
//     std::cerr << "Usage: " << argv[0] << " project-id "
//               << "[target-language (default: es-419)]\n";
//     return 1;
//   }

//   namespace translate = ::google::cloud::translate;
//   auto client = translate::TranslationServiceClient(
//       translate::MakeTranslationServiceConnection());

//   auto const project = google::cloud::Project(argv[1]);
//   auto const target = std::string{argc >= 3 ? argv[2] : "es-419"};
//   auto response =
//       client.TranslateText(project.FullName(), target, {std::string{kText}});
//   if (!response) throw std::move(response).status();

//   for (auto const& translation : response->translations()) {
//     std::cout << translation.translated_text() << "\n";
//   }

//   return 0;
// } catch (google::cloud::Status const& status) {
//   std::cerr << "google::cloud::Status thrown: " << status << "\n";
//   return 1;
// }

string run_dictionary(istringstream word);
string run_jokes();

int main() {
    // Create the socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // Connect to the server
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    if (connect(sock, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error connecting to server" << endl;
        return 1;
    }

    // Send the server password
    string message = "PASS " + SERVER_PASSWORD + "\r\n";
    if (send(sock, message.c_str(), message.size(), 0) == -1) {
        cerr << "Error sending server password" << endl;
        return 1;
    }

    // Send the nickname
    message = "NICK " + NICKNAME + "\r\n";
    if (send(sock, message.c_str(), message.size(), 0) == -1) {
        cerr << "Error sending nickname" << endl;
        return 1;
    }

    // Send the username
    message = "USER " + USERNAME + " 0 * :" + REALNAME + "\r\n";
    if (send(sock, message.c_str(), message.size(), 0) == -1) {
        cerr << "Error sending username" << endl;
        return 1;
    }

    // Join the channel
    message = "JOIN " + CHANNEL + "\r\n";
    if (send(sock, message.c_str(), message.size(), 0) == -1) {
        cerr << "Error joining channel" << endl;
        return 1;
    }

    // run_translator(2, argv);

    // Continuously read and process messages from the server
    while (true) {
        char buffer[4096];
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            cerr << "Error receiving message" << endl;
            return 1;
        }
        // Convert the buffer to a string
        string message(buffer, bytes_received);
        // Split the message into lines
        stringstream stream(message);
        string line;
        // Check if the message is a command
        while (getline(stream, line, '\r')) {
            cout << "Received: " << line << endl;
            size_t it = line.find("PRIVMSG " + CHANNEL);
            if (it == string::npos)
                it = line.find("NOTICE " + CHANNEL);
            if (it != string::npos)
                it = line.find(" :", it);
            if (it != string::npos && line.at(it + 2) == '!') {
                string command = line.substr(it + 3);
                std::cout << "command = " << command << std::endl;

                // Handle the "!time" command
                if (strncmp(command.c_str(), "time", sizeof("time")) == 0) {
                    time_t now = time(0);
                    string time_str = ctime(&now);
                    message = "PRIVMSG " + CHANNEL + " :The current time is " + time_str + "\r\n";
                    if (send(sock, message.c_str(), message.size(), 0) == -1) {
                        cerr << "Error sending message" << endl;
                        return 1;
                    }
                }

                else if (strncmp(command.c_str(), "define", sizeof("define")) == 0) {
                    message = "PRIVMSG " + CHANNEL + " :" + run_dictionary(istringstream(command));
                    std::cerr << "message : '" << message << "'\n" << std::endl;
                    if (send(sock, message.c_str(), message.size(), 0) == -1) {
                        cerr << "Error sending message" << endl;
                        return 1;
                    }
                }
                
                else if (strncmp(command.c_str(), "jokes", sizeof("jokes")) == 0) {
                    message = "PRIVMSG " + CHANNEL + " :" + run_jokes();
                    std::cerr << "message : '" << message << "'\n" << std::endl;
                    if (send(sock, message.c_str(), message.size(), 0) == -1) {
                        cerr << "Error sending message" << endl;
                        return 1;
                    }
                }
                
                else if (line.find("PING")) {
                    message = "PONG localhost\r\n";
                    if (send(sock, message.c_str(), message.size(), 0) == -1) {
                        cerr << "Error sending message" << endl;
                        return 1;
                    }
                }
            }
        }
    }
}

string run_dictionary(istringstream sentence) {
    // Initialize cURL
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing cURL" << std::endl;
        return "";
    }

    // Set the API endpoint and API key
    const std::string endpoint = "https://od-api.oxforddictionaries.com/api/v2/entries/en-gb";
    const std::string app_id = "";
    const std::string app_key = "";

    // Build the request URL
    string word;
    sentence >> word; // get rid of 'define'
    word.clear();
    sentence >> word;
    std::string url = endpoint + "/" + word;

    // Set the URL and perform the request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, url.c_str());
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error making request: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    // Get the response
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &response);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error getting response: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    // Clean up
    curl_easy_cleanup(curl);
    return response;
}

string run_jokes() {
    // Initialize cURL

    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing cURL" << std::endl;
        return "";
    }

    // Set the API endpoint
    const std::string endpoint = "https://jokeapi.dev/joke/Any";

    // Perform the request
    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error making request: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    // Get the response
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error getting response: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_cleanup(curl);
    return response;
}
