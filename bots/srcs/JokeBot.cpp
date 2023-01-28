/* Local includes */
#include "JokeBot.hpp"

JokeBot::JokeBot(string name, string servName, string channel, int port, string pass)
  : Bot(name, servName, channel, port, pass) {}

JokeBot::~JokeBot( ) {}

int JokeBot::reply(std::string sentence) {
	(void)sentence;
	// Initialize cURL
	CURL *curl = curl_easy_init( );

	if (!curl) {
		std::cerr << "Error initializing cURL" << std::endl;
		return 1;
	}

	// Set the API endpoint
	const std::string endpoint = "https://jokeapi.dev/joke/Any";

	// Perform the request
	curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str( ));
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "Error making request: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(curl);
		return 1;
	}

	// Get the response
	std::string response;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "Error getting response: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(curl);
		return 1;
	}

	// Clean up
	curl_easy_cleanup(curl);

	// Message the channel
	_parse_joke(response);
	if (sendChannelMessage( ))
		return 1;
	return 0;
}

void JokeBot::_parse_joke(std::string html) {
	size_t      pos  = html.find("type") + 8;
	std::string type = html.substr(pos, html.find(",", pos) - pos - 1);

	_reply.clear( );

	if (!strcmp(type.c_str( ), "twopart")) {
		_parse_joke_html_field(html, "setup", "delivery");
		_reply.append(" ");
		_parse_joke_html_field(html, "delivery", "flags");
	}
	else if (!strcmp(type.c_str( ), "single")) {
		_parse_joke_html_field(html, "joke", "flags");
	}
}

void JokeBot::_parse_joke_html_field(std::string raw,
                                     std::string start,
                                     std::string end) {
	size_t i = raw.find(start) + start.size( ) + 4;
	size_t j = raw.find(end) - 3;

	if (i == std::string::npos || j == std::string::npos || j < i) {
		std::cerr << "Error parsing html fields" << i << "\n";
		return;
	}
	while (i + 5 != j) {
		std::cout << raw[i];
		if (raw[i] == '\\') {
			++i;
			switch (raw[i++]) {
			case 'n':
				_reply.append(" ");
				continue;
			case '"':
				_reply.append("\"");
				continue;
			}
		}
		_reply += raw[i];
		i++;
	}
	// std::cerr << "Parsed joke: '" << _reply << "'" << std::endl;
}
