#include "Float_Calculate.h"

// ��ȡ�������ķ���λ��ָ�����ֺ�β������
void extractComponents(uint32_t bits, int &sign, int &exponent,
                       uint32_t &fraction) {
  sign = (bits >> 31) & 0x1;
  exponent = ((bits >> 23) & 0xFF);        // ����ָ����ƫ��
  fraction = (bits & 0x7FFFFF) | 0x800000; // ����ʡ�Ե� 1
}

// ������λ��ָ�����ֺ�β��������װ�� IEEE 754 ��ʽ
uint32_t combineComponents(int sign, int exponent, uint32_t fraction) {
  return (sign << 31) | (exponent << 23) | (fraction & 0x7FFFFF);
}

// �������ӷ�
uint32_t floatAddition(uint32_t aBits, uint32_t bBits) {
  int aSign, bSign;
  int aExponent, bExponent;
  uint32_t aFraction, bFraction;
  extractComponents(aBits, aSign, aExponent, aFraction);
  extractComponents(bBits, bSign, bExponent, bFraction);

  // ��ָ�����е�����ʹ�������
  int exponentDiff = aExponent - bExponent;
  if (exponentDiff > 0) {
    bFraction >>= exponentDiff;
    bExponent += exponentDiff;
  } else {
    aFraction >>= -exponentDiff;
    aExponent += -exponentDiff;
  }

  // ���ݷ���λִ�мӷ������
  uint32_t resultFraction;
  if (aSign == bSign) {
    resultFraction = aFraction + bFraction;
  } else {
    resultFraction = (aFraction > bFraction) ? (aFraction - bFraction)
                                             : (bFraction - aFraction);
  }

  // ����β�����ڵ��� 0x8fffff �����
  while (resultFraction >= 0xffffff) {
    resultFraction >>= 1;
    ++aExponent;
  }

  // ����β��С�ڵ��� 0x800000 �����
  while (resultFraction <= 0x800000) {
    resultFraction <<= 1;
    --aExponent;
  }

  // ȥ�������1
  resultFraction &= 0x7FFFFF;

  // ��װ���
  int resultSign = aSign;
  int resultExponent = aExponent;
  return combineComponents(resultSign, resultExponent, resultFraction);
}

// ����������
uint32_t floatSubtraction(uint32_t aBits, uint32_t bBits) {
  int bSign, bExponent;
  uint32_t bFraction;

  extractComponents(bBits, bSign, bExponent, bFraction);
  bSign ^= 0x1;

  return floatAddition(aBits, combineComponents(bSign, bExponent, bFraction));
}
// ģ�������ӷ���ʹ���޷�������
unsigned int simulateIntegerAddition(unsigned int a, unsigned int b) {
  // �޷��������ӷ������ʱ�������ģ���Ĳ���
  unsigned int sum = a + b;

  return sum;
}

// ģ������������ʹ���޷�������
unsigned int simulateIntegerSubtraction(unsigned int a, unsigned int b) {
  // ע�⣺������� a >= b��������Ҫ���н���

  // �޷����������������ʱ�������ģ���Ĳ���
  unsigned int difference = a - b;

  return difference;
}

string To_anyscale_string(
    int x, int scale, int num_of_str,
    int character) // ʮ����ת������(�������ƣ�λ�����Ƿ������չ(Ĭ�ϲ���չ))
{
  int flag = 0, num = 0;
  string s = "";
  string tmp;
  if (x < 0) {
    x = -x;
    flag = 1;
  }
  while (x) {
    if (x % scale < 10)
      tmp = to_string(x % scale);
    else {
      tmp = 'a' + (x % scale - 10);
    }
    if (x % scale == 0)
      s = '0' + s;
    else
      s = tmp + s;
    x /= scale;
  }

  while (s.size() < num_of_str) {
    if (character == 0)
      s = '0' + s;
    else if (flag) // ����������չ,��Ϊ��һ����ȡ������������Ȳ�0
      s = '0' + s;
    else
      s = '0' + s;
  }
  if (flag) {
    for (auto &each : s) {
      each = each == '1' ? 'm' : each;
    }
    for (auto &each : s) {
      each = each == '0' ? '1' : each;
    }
    for (auto &each : s) {
      each = each == 'm' ? '0' : each;
    }
    num = 1 + stoi(s, nullptr, 2);
    s = To_anyscale_string(num, scale, num_of_str);
  }
  return s;
}

long int To_decimal_int(
    string source, int base,
    int symbol) // ��������Ƶ��ַ���ת��Ϊ10���Ƶ�������baseΪ���ַ����Ľ���,symbol
                // = 1Ϊ�з�����
{
  int flag = 0;
  // cout << source[0] << endl;
  if (source[0] == '1' && symbol) // symbolΪ0�����޷�����������ȡ����һ
  {
    for (auto &each : source) {
      each = each == '1' ? 'm' : each;
    }
    for (auto &each : source) {
      each = each == '0' ? '1' : each;
    }
    for (auto &each : source) {
      each = each == 'm' ? '0' : each;
    }
    int num = 1 + stoi(source, nullptr, 2); // ȡ����һ
    source = To_anyscale_string(num, base, source.length());
    flag = 1;
  }
  // cout << source << endl;
  char str[source.length()];
  strcpy(str, source.c_str());
  // cout << str << endl;
  char *stop; // ��Ƿ��ַ���
  if (flag) {
    int result = strtol(str, &stop, base);
    return -1 * result;
  } else
    return strtol(str, &stop, base);
}
// �������ȸ�������ת��Ϊ������
string Float_To_Binary_ByMemory(float num) {
  unsigned char *p = (unsigned char *)(&num); // ÿ�ζ�һ���ֽڣ�8λ��
  string Result("");
  // cout << sizeof(num) << endl;
  for (int i = sizeof(num) - 1; i >= 0;
       --i) // ����ʹ�õ���С�˻��������ڴ�˷��Ļ���������ʹ��
            // i=0;i<sizeof(num)-1;++i
  {
    // ��unsigned char ת��Ϊ ������
    string temp(8, '0');         // ����ÿ���ֽڵĶ����Ʊ�ʾ
    int Index = temp.size() - 1; // 7
    unsigned char ch = p[i];
    while (ch) {
      temp[Index] = ch % 2 + '0';
      ch /= 2;
      --Index;
    }
    Result += temp;
  }
  return Result;
}

// �������Ƶ����ݣ�ת��Ϊfloat������
float Binary_To_float(string str) {
  if (str.size() != 32) {
    return NAN;
  }
  //	(-1) s��ʾ����λ����s = 0��VΪ��������s = 1��VΪ������
  //	��2��M��ʾ��Ч���֣����ڵ���1��С��2��
  //	��3��2^E��ʾָ��λ��
  // ���ڵ����� ����λ 1 ָ��Ϊ8  ��Ч���� 23
  //  ��1��E��ȫΪ0��ȫΪ1����ʱ���������Ͳ�������Ĺ����ʾ����ָ��E�ļ���ֵ��ȥ127����1023�����õ���ʵֵ���ٽ���Ч����Mǰ���ϵ�һλ��1��
  //	��2��EȫΪ0����ʱ����������ָ��E����1 - 127������1 -
  // 1023������Ч����M���ټ��ϵ�һλ��1�����ǻ�ԭΪ0.xxxxxx��С������������Ϊ�˱�ʾ��0���Լ��ӽ���0�ĺ�С�����֡�
  //	��3��EȫΪ1����ʱ�������Ч����MȫΪ0����ʾ�����������ȡ���ڷ���λs���������Ч����M��ȫΪ0����ʾ���������һ������NaN����
  int sign = (str[0] == '0') ? 1 : -1;
  // �ֽ���Ч������
  float frac = 0.0;
  for (int i = 9; i < str.size(); ++i) {
    frac += pow(2, -i + 8) * (str[i] - '0');
  }
  // �ֽ�ָ��λ

  // ����ȫΪ0�����
  bool judge = true;
  for (int i = 1; i < 9; ++i) {
    if (str[i] == '1') {
      judge = false;
      break;
    }
  } // �ж��Ƿ�ȫΪ0
  if (judge) {
    return sign * frac * pow(2, 1 - 127);
  }

  // ����ȫΪ1�����
  judge = true;
  for (int i = 1; i < 9; ++i) {
    if (str[i] == '0') {
      judge = false;
      break;
    }
  }
  if (judge) {
    if (0.0 == frac) // ���MȫΪ0
    {
      if (1 == sign) // ��ô�������Ϊ1 �Ļ������ظ�����
      {
        return -INFINITY;
      } else // �������Ϊ0�Ļ�������������
      {
        return INFINITY;
      }
    } else // ��Ȼ�Ļ�������NAN ����һ������
    {
      return NAN;
    }
  }

  // ���һ�������ָ����ȫΪ0����1
  unsigned char p = 0;
  for (int i = 8, Index = 0; i >= 1; --i, ++Index) {
    p += pow(2, Index) * (str[i] - '0'); // �����㣬��������
  }

  float E = pow(2, p - 127);
  return sign * E * (1 + frac); // ע��fracӦ��1
}
