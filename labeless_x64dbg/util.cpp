/* Labeless
 * by Aliaksandr Trafimchuk
 *
 * Source code released under
 * Creative Commons BY-NC 4.0
 * http://creativecommons.org/licenses/by-nc/4.0
 */

#include "util.h"
#include <regex>
#include <WinSock2.h>

namespace util {

xstring getHostAppDir()
{
	TCHAR buff[MAX_PATH] = {};
	GetModuleFileName(NULL, buff, MAX_PATH);
	PathRemoveFileSpec(buff);
	return xstring(buff);
}

xstring getPluginDir(HINSTANCE hPlugin)
{
	TCHAR buff[MAX_PATH] = {};
	GetModuleFileName(hPlugin, buff, MAX_PATH);
	PathRemoveFileSpec(buff);
	return xstring(buff);
}

xstring inetAddrToString(sockaddr_in* sin)
{
	TCHAR buff[MAX_PATH] = {};
	DWORD buffLen = MAX_PATH;
	if (SOCKET_ERROR == WSAAddressToString(reinterpret_cast<sockaddr*>(sin), sizeof(sockaddr_in), nullptr, buff, &buffLen))
	{
		return xstring();
	}
	return xstring(buff);
}


std::wstring mb2w(const std::string& v)
{
	if (v.empty())
		return{};

	int len = static_cast<int>(v.length());

	int iLen = MultiByteToWideChar(CP_ACP, 0, v.c_str(), len, nullptr, 0);
	if (iLen <= 0)
		return{};
	std::wstring rv(iLen, L'\0');
	iLen = MultiByteToWideChar(CP_ACP, 0, v.c_str(), len, &rv[0], iLen);
	if (iLen > 0)
		return rv;
	return{};
}

std::string w2mb(const std::wstring& v)
{
	if (v.empty())
		return{};

	int wideLen = static_cast<int>(v.length());

	int iLen = WideCharToMultiByte(CP_ACP, 0, v.c_str(), wideLen, nullptr, 0, NULL, NULL);
	if (iLen <= 0)
		return{};

	std::string rv(iLen, '\0');
	iLen = WideCharToMultiByte(CP_ACP, 0, v.c_str(), wideLen, &rv[0], iLen, NULL, NULL);
	if (iLen)
		return rv;
	return{};
}

std::deque<std::string> split(const std::string& s, const std::string& delimitersSet)
{
	std::deque<std::string> rv;

	const std::string fixed = std::regex_replace(s, std::regex("\r"), "");

	std::regex rePattern(delimitersSet);
	std::sregex_token_iterator iter(fixed.begin(), fixed.end(), rePattern, -1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter)
	{
		const std::string& item = *iter;
		if (!item.empty())
			rv.push_back(item);
	}
	return rv;
}

std::string randStr(int len)
{
	auto randChar = []() {
		char c = '\0';
		do {
			c = rand() % 0xFF;
		} while (!::isalpha(c));
		return c;
	};

	std::string rv(len, '\0');
	for (int i = 0; i < len; ++i)
		rv[i] = randChar();

	return rv;
}

} // util
