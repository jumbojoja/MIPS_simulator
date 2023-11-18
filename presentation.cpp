#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <math.h>
using namespace std;

string To_anyscale_string(int x, int scale, int num_of_str, int character = 0) // 十进制转二进制(数，进制，位数，是否符号扩展(默认不扩展))
{
    int flag = 0, num = 0;
    string s = "";
    string tmp;
    if (x < 0)
    {
        x = -x;
        flag = 1;
    }
    while (x)
    {
        if (x % scale < 10)
            tmp = to_string(x % scale);
        else
        {
            tmp = 'a' + (x % scale - 10);
        }
        if (x % scale == 0)
            s = '0' + s;
        else
            s = tmp + s;
        x /= scale;
    }

    while (s.size() < num_of_str)
    {
        if (character == 0)
            s = '0' + s;
        else if (flag) // 负数符号扩展,因为下一步会取反，因此我们先补0
            s = '0' + s;
        else
            s = '0' + s;
    }
    if (flag)
    {
        for (auto &each : s)
        {
            each = each == '1' ? 'm' : each;
        }
        for (auto &each : s)
        {
            each = each == '0' ? '1' : each;
        }
        for (auto &each : s)
        {
            each = each == 'm' ? '0' : each;
        }
        num = 1 + stoi(s, nullptr, 2);
        s = To_anyscale_string(num, scale, num_of_str);
    }
    return s;
}

long int To_decimal_int(string source, int base, int symbol = 1) // 将任意进制的字符串转换为10进制的整数，base为该字符串的进制,symbol = 1为有符号数
{
    int flag = 0;
    // cout << source[0] << endl;
    if (source[0] == '1' && symbol) // symbol为0则是无符号数，不用取反加一
    {
        for (auto &each : source)
        {
            each = each == '1' ? 'm' : each;
        }
        for (auto &each : source)
        {
            each = each == '0' ? '1' : each;
        }
        for (auto &each : source)
        {
            each = each == 'm' ? '0' : each;
        }
        int num = 1 + stoi(source, nullptr, 2); // 取反加一
        source = To_anyscale_string(num, base, source.length());
        flag = 1;
    }
    // cout << source << endl;
    char str[source.length()];
    strcpy(str, source.c_str());
    // cout << str << endl;
    char *stop; // 存非法字符串
    if (flag)
    {
        int result = strtol(str, &stop, base);
        return -1 * result;
    }
    else
        return strtol(str, &stop, base);
}

// 将单精度浮点数，转换为二进制
string Float_To_Binary_ByMemory(float num)
{
    unsigned char *p = (unsigned char *)(&num); // 每次读一个字节（8位）
    string Result("");
    // cout << sizeof(num) << endl;
    for (int i = sizeof(num) - 1; i >= 0; --i) // 这里使用的是小端机器，对于大端法的机器，可以使用 i=0;i<sizeof(num)-1;++i
    {
        // 将unsigned char 转换为 二进制
        string temp(8, '0');         // 保存每个字节的二进制表示
        int Index = temp.size() - 1; // 7
        unsigned char ch = p[i];
        while (ch)
        {
            temp[Index] = ch % 2 + '0';
            ch /= 2;
            --Index;
        }
        Result += temp;
    }
    return Result;
}

// 将二进制的数据，转换为float浮点数
float Binary_To_float(string str)
{
    if (str.size() != 32)
    {
        return NAN;
    }
    //	(-1) s表示符号位，当s = 0，V为正数；当s = 1，V为负数。
    //	（2）M表示有效数字，大于等于1，小于2。
    //	（3）2^E表示指数位。
    // 对于单精度 符号位 1 指数为8  有效数字 23
    //  （1）E不全为0或不全为1。这时，浮点数就采用上面的规则表示，即指数E的计算值减去127（或1023），得到真实值，再将有效数字M前加上第一位的1。
    //	（2）E全为0。这时，浮点数的指数E等于1 - 127（或者1 - 1023），有效数字M不再加上第一位的1，而是还原为0.xxxxxx的小数。这样做是为了表示±0，以及接近于0的很小的数字。
    //	（3）E全为1。这时，如果有效数字M全为0，表示±无穷大（正负取决于符号位s）；如果有效数字M不全为0，表示这个数不是一个数（NaN）。
    int sign = (str[0] == '0') ? 1 : -1;
    // 分解有效数数字
    float frac = 0.0;
    for (int i = 9; i < str.size(); ++i)
    {
        frac += pow(2, -i + 8) * (str[i] - '0');
    }
    // 分解指数位

    // 假如全为0的情况
    bool judge = true;
    for (int i = 1; i < 9; ++i)
    {
        if (str[i] == '1')
        {
            judge = false;
            break;
        }
    } // 判断是否全为0
    if (judge)
    {
        return sign * frac * pow(2, 1 - 127);
    }

    // 假如全为1的情况
    judge = true;
    for (int i = 1; i < 9; ++i)
    {
        if (str[i] == '0')
        {
            judge = false;
            break;
        }
    }
    if (judge)
    {
        if (0.0 == frac) // 如果M全为0
        {
            if (1 == sign) // 那么如果符号为1 的话，返回负无穷
            {
                return -INFINITY;
            }
            else // 如果符号为0的话，返回正无穷
            {
                return INFINITY;
            }
        }
        else // 不然的话，返回NAN 不是一个数字
        {
            return NAN;
        }
    }

    // 最后一种情况，指数不全为0或者1
    unsigned char p = 0;
    for (int i = 8, Index = 0; i >= 1; --i, ++Index)
    {
        p += pow(2, Index) * (str[i] - '0'); // 倒着算，从右往左
    }

    float E = pow(2, p - 127);
    return sign * E * (1 + frac); // 注意frac应加1
}

int main()
{
    int num = 1.25;
    // char str[100];
    // itoa(num, str, 2);
    // cout << str << endl;
    // cout << To_string(num, 8, 8, 0);
    string buffer = "11110101";

    cout << Binary_To_float("00111111101000000000000000000000") << endl;
    cout << Float_To_Binary_ByMemory(num);

    // cout << To_decimal_int(buffer, 2, 1);
    // char buffer[20] = "10379cend$3";
    // char *stop;
    // printf("%d\n", strtol(buffer, &stop, 8));
    // printf("%s\n", stop);
}
