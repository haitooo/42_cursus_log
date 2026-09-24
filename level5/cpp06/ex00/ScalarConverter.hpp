#ifndef SCALARCONVERTER_HPP
# define SCALARCONVERTER_HPP

#include <string>

class	ScalarConverter {
	private:
		enum	LiteralType {
			TYPE_CHAR,
			TYPE_INT,
			TYPE_FLOAT,
			TYPE_DOUBLE,
			TYPE_INVALID
		};

		ScalarConverter();
		ScalarConverter(const ScalarConverter& other);
		ScalarConverter&	operator=(const ScalarConverter& other);
		~ScalarConverter();

		static LiteralType	detectType(const std::string& literal);
		static LiteralType	checkRange(const std::string& literal, LiteralType type);

		static char			parseChar(const std::string& literal);
		static int			parseInt(const std::string& literal);
		static float		parseFloat(const std::string& literal);
		static double		parseDouble(const std::string& literal);

		static void			convertFromChar(char c);
		static void			convertFromInt(int n);
		static void			convertFromFloat(float f);
		static void			convertFromDouble(double d);

		static void			printChar(char c);
		static void			printInt(int n);
		static void			printFloat(float f);
		static void			printDouble(double d);
		static void			printImpossible(const char* typeName);

		static bool			fitsChar(double value);
		static bool			fitsInt(double value);
		static bool			fitsFloat(double value);
		static bool			isNan(double value);
		static bool			isInf(double value);
		static std::string	formatDecimal(double value, int precision);

	public:
		static void	convert(const std::string& literal);
};

#endif
