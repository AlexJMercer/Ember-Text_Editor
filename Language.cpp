
#include "Language.hpp"


QString Language::toString(Lang language)
{
	switch (language)
	{
		case CPP:
			return "C++";
		case C:
			return "C";
		case Python:
			return "Python";
		case Java:
			return "Java";
		default:
			return "Not Selected";
	}
}