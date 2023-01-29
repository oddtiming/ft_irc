/* Local includes */
#include "DictBot.hpp"

DictBot::DictBot(string name, string servName, string channel, int port, string pass)
  : Bot(name, servName, channel, port, pass) {}

DictBot::~DictBot( ) {}

int DictBot::reply(std::string sentence) {
	// Initialize cURL
	CURL    *curl = curl_easy_init( );
	CURLcode res;
	if (!curl) {
		std::cerr << "Error initializing cURL" << std::endl;
		return 1;
	}

	// Set the API endpoint and API key
	std::string endpoint = "https://od-api.oxforddictionaries.com/api/v2/entries/en-gb/";
	const char *app_id   = getenv("OXFORD_APP_ID");
	const char *app_key  = getenv("OXFORD_APP_KEY");
	if (!app_id || !app_key) {
		std::cerr << "Error initializing cURL" << std::endl;
		curl_easy_cleanup(curl);
		return 1;
	}
	std::string app_id_header = "app_id: ";
	app_id_header.append(app_id);
	std::string app_key_header = "app_key: ";
	app_key_header.append(app_key);

	// Build the HTML GET header
	curl_slist *headers = NULL;
	headers             = curl_slist_append(headers, app_id_header.c_str( ));
	headers             = curl_slist_append(headers, app_key_header.c_str( ));
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Build the request URL
	size_t      pos  = sentence.find_first_not_of(" \t\n\r", 4);
	std::string word = sentence.substr(pos, sentence.find_first_of(" \t\n\r", pos));
	std::string url  = endpoint + word + "?fields=definitions&strictMatch=false";

	std::cerr << "Word to define : '" << word << "'" << std::endl;
	std::cerr << "Querying url '" << url << "'" << std::endl;

	// Set the URL and perform the request
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str( ));
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // follow HTTP 3xx redirects

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "Error making request: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(curl);
		return 1;
	}

	// Get the response
	_reply.clear( );
	std::string html;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "Error getting response: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(curl);
		return 1;
	}

	// Clean up
	curl_easy_cleanup(curl);

	// Message the channel
	if (_parse_definitions(html))
		return 1;
	return 0;
}

int DictBot::_parse_definitions(const std::string &html) {
	size_t it = html.find("definitions");
	size_t ite;
	size_t count = 0;

	while (it != std::string::npos && count < 5) {
		it  = html.find_first_not_of(" \t\r\n", it + 16);
		ite = html.find("\n", it);
		_reply.clear( );
		_reply = std::to_string(count);
		_reply.append(") ");
		_reply.append(html.substr(it, ite - 5));
		std::cout << "_reply : " << _reply << std::endl;
		if (sendChannelMessage( ))
			return 1;
		it = html.find("definitions", ite);
		++count;
	}
	return 0;
}
