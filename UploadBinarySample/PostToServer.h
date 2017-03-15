#pragma once
#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <algorithm>

class PostToServer
{
private:
	std::string mFileName;
public:

	void pp(const char *pszName, const char *pszData)
	{
		

	}
	bool Push(const char *pszName, const char *pszData)
	{
		char params[1024];
		memset(params, 0, sizeof(params));
		sprintf(params, "name=%s&data=%s", pszName, pszData);


		//73,61,77,79,65,72
		char *ip = "103.10.4.203";
		ip = "192.168.126.128";

		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 0;

		SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (fd < 0)     throw;
		SOCKADDR_IN service;
		service.sin_family = AF_INET;
		service.sin_port = htons(80);
		LPHOSTENT host = gethostbyname(ip);
		if (!host)          throw;
		service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
		if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)     throw;
		int len = strlen(params);
		std::string strParam = this->UrlEncode(params);

		char header[1024];
		sprintf(header,
			"POST /vivobaby/php/binary.php HTTP/1.1\r\n"
			"Host: %s\r\n"
			"User-Agent: Mozilla Firefox/4.0\r\n"
			"Content-Length: %d\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Accept-Charset: utf-8\r\n\r\n"
			"%s\r\n",
			ip, len, strParam.c_str());
		send(fd, header, strlen(header), 0);
		fprintf(stderr, "Done\n");
		closesocket(fd);
		WSACleanup();
	}

	std::string UrlEncode(std::string str) {
		std::string new_str = "";
		char c;
		int ic;
		const char* chars = str.c_str();
		char bufHex[10];
		int len = strlen(chars);

		for (int i = 0; i<len; i++) {
			c = chars[i];
			ic = c;
			// uncomment this if you want to encode spaces with +
			/*if (c==' ') new_str += '+';
			else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
			else {
				sprintf(bufHex, "%X", c);
				if (ic < 16)
					new_str += "%0";
				else
					new_str += "%";
				new_str += bufHex;
			}
		}
		
		new_str = ReplaceAll(new_str, "x", "");
		return new_str;
	}


	std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
	}
};
