#include <string>

class CJDsBase64
{
public:
	CJDsBase64() {}
	virtual ~CJDsBase64() {}

	static inline bool is_base64(unsigned char c) 
	{
	  return (isalnum(c) || (c == '+') || (c == '/'));
	}

	static std::string base64_encode(unsigned char const* , unsigned int len);
	static std::string base64_decode(std::string const& s);
};
