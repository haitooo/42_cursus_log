１．文字列コンストラクタで、先頭の余分なゼロを除去してください。

bigint::bigint(std::string str)
{
    std::string::size_type pos = str.find_first_not_of('0');

    if (pos == std::string::npos)
        _value = "0";
    else
        _value = str.substr(pos);
}

これで "00012" は "12"、"000" や空文字列は "0" になります。数字だけの文字列を受け取る前提です。

２．左シフトで、巨大なシフト量を無視するのは誤りです。

if (!(iss >> shift))
    return bigint(tmp);  // 元の値を返すのは、計算結果として不正

右シフトと違い、左シフトでは巨大な結果を格納できません。その場合は、例外で失敗を知らせる方法があります。

#include <stdexcept>

bigint bigint::operator<<(const bigint& other) const
{
    std::string tmp = getValue();

    if (tmp == "0")
        return bigint(0);

    std::istringstream iss(other.getValue());
    std::string::size_type shift = 0;

    if (!(iss >> shift) ||
        shift > tmp.max_size() - tmp.size())
    {
        throw std::length_error("bigint: shift too large");
    }

    tmp.append(shift, '0');
    return bigint(tmp);
}

max_size() 以内でも実際のメモリが足りなければ、文字列の確保時に例外が発生します。
