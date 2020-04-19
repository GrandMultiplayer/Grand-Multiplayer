#include "Master.h"

#include <sstream>
#include <vector>

#include "Config.h"
#include "CPlayer.h"

#include "curl/curl.h"

template<typename Out>
void split(const std::string &s, char delim, Out result)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

bool Master::IsLocalIPAddress(std::string ipaddr)
{
	if (ipaddr.compare("localhost") == 0)
	{
		return true;
	}

	std::vector<std::string> x = split(ipaddr, '.');

	if ((std::stoi(x[0]) == 192 && std::stoi(x[1]) == 168) ||
		(std::stoi(x[0]) == 172 && std::stoi(x[1]) >= 16 && std::stoi(x[2]) <= 31) ||
		(std::stoi(x[0]) == 127 && std::stoi(x[3]) == 1) ||
		std::stoi(x[0]) == 10)
	{
		return true;
	}

	return false;
}

void Master::UpdateMaster()
{
	if (Master::IsLocalIPAddress(g_Config->GetIp()))
		return;

	bool locked = false;
	if (!g_Config->GetPassword().empty())
		locked = true;

	std::stringstream oss;
	oss << "http://api.grand-multiplayer.com/gmmasterlist/postServer.php?ipAddress=" << g_Config->GetIp() << "&port=" << g_Config->GetPort() << "&name=" 
		<< g_Config->GetServerName() << "&version=" << VERSION << "&curPlayers=" << g_Players.size() << "&maxPlayers=" << g_Config->GetMaxPlayers()
		<< "&gamemode=" << g_Config->GetGamemode() << "&locked=" << locked;

	/*else
	{
		oss << "http://api.grand-multiplayer.com/gmmasterlist/postServer.php?port=" << port << "&name=" << name << "&version=" << version << "&curPlayers=" << players << "&maxPlayers=" << maxPlayers << "&gamemode=" << gamemode;
	}*/

	std::string url = oss.str();

	// set our locator equal to the first appearance of any character in replace
	size_t found = url.find_first_of(' ');

	while (found != std::string::npos)
	{ // While our position in the sting is in range.
		url.replace(found, 1, "%20"); // Replace the character at position.
		found = url.find_first_of(' ', found + 1); // Relocate again.
	}

	CURL *curl = curl_easy_init();
	CURLcode res;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

	/*if (!responce)
	{
		curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, &Master::my_dummy_write);
	}*/

	CURLcode ret = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
}