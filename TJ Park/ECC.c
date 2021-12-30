#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P224 7
// 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 00000000 00000001
#define P256 8
// 0xFFFFFFFF 00000001 00000000 00000000 00000000 FFFFFFFF FFFFFFFF FFFFFFFF
#define P384 12
// 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFFFF 00000000 00000000 FFFFFFFF
#define P521 16
// 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 

typedef unsigned int word;

typedef struct{
  word* arr;
}BN;

typedef struct{
  int p;
  BN polynomial;
}ECC_info;

typedef struct{
  BN x;
  BN y;
}Affine;

typedef struct{
  BN x;
  BN y;
  BN z;
}Jacobian;

// void hexword2int(const char* hexword, )

void BN_print_hex(const BN* data, ECC_info ECC, const char* name)
{
  if(name != NULL)
  {
    printf("%s = ", name);
  }
  printf("0x");
  for(int i = ECC.p - 1; i >= 0; i--)
  {
    printf("%08x ", data->arr[i]);
  }
  printf("\n");
}

void ECC_init(ECC_info* ECC, int p_type)
{
  ECC->p = p_type;
  ECC->polynomial.arr = (word*)malloc(ECC->p * sizeof(word));
  
  for(int i = 0; i < p_type; i++)
  {
    ECC->polynomial.arr[i] = 0xFFFFFFFF;
  }

  if (p_type == P224)
  {
    ECC->polynomial.arr[4] = 0;
    ECC->polynomial.arr[5] = 0;
    ECC->polynomial.arr[6] = 1;
  }

  else if (p_type == P256)
  {
    ECC->polynomial.arr[1] = 1;
    ECC->polynomial.arr[2] = 0;
    ECC->polynomial.arr[3] = 0;
    ECC->polynomial.arr[4] = 0;
  }

  else if (p_type == P384)
  {
    ECC->polynomial.arr[7] -= 1;
    ECC->polynomial.arr[9] = 0;
    ECC->polynomial.arr[10] = 0;
  }

  else if (p_type == P521)
  {

  }

  else
  {
    printf("P_TYPE_ERROR : TYPE OF P IS NOT EXIST.\n");
  }
}

void hex2digit(const char hex, int* data)
{
  if (hex >= '0' && hex <= '9')
  {
    *data = (int)(hex - '0');
  }

  else if (hex >= 'A' && hex <= 'F')
  {
    *data = (int)(hex - 'A') + 10;
  }

  else if (hex >= 'a' && hex <= 'f')
  {
    *data = (int)(hex - 'a') + 10;
  }

  else
  {
    return;
  }
}

void hex2BN(const char* hexstring, BN* data, const ECC_info ECC)
{
  int i = 2;
  int j = ECC.p - 1;
  int temp = 0;

  while(hexstring[i] != '\0')
  {
    if(hexstring[i] == ' ')
    {
      j--;
    }
    
    hex2digit(hexstring[i], &temp);
    data->arr[j] = (data->arr[j] << 4) + temp;

    i++;
  }
}

void BN_init(BN* data, const ECC_info ECC)
{
  data->arr = (word*)malloc(ECC.p * sizeof(word));
  for(int i = 0; i < ECC.p; i++)
  {
    data->arr[i] = 0;
  }
}

void WordArr_zerolize(word* data, int size)
{
  for(int i = 0; i < size; i++)
  {
    data[i] = 0;
  }
}

void BN_zerolize(BN* data, const ECC_info ECC)
{
  WordArr_zerolize(data->arr, ECC.p);
}

void BN_set(const char* hexstring, BN* data, const ECC_info ECC)
{
  hex2BN(hexstring, data, ECC);
}

void BNF_reduction_fast(word* data, ECC_info ECC)
{

}

void BNF_add(const BN src1, const BN src2, BN* dst, const ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  int carry_bit = 0;
  int cur_carry_bit = 0;
  word* temp = (word*)malloc((ECC.p + 1) * sizeof(word));
  WordArr_zerolize(temp, ECC.p + 1);

  for(int i = 0; i < ECC.p; i++)
  {
    cur_carry_bit = carry_bit;
    temp[i] = src1.arr[i] + src2.arr[i] + cur_carry_bit;

    if(src1.arr[i] + src2.arr[i] < src1.arr[i])
    {
      carry_bit = 1;
    }
    else
    {
      carry_bit = 0;
    }

    if(temp[i] < cur_carry_bit)
    {
      carry_bit = 1;
    }
  }

  temp[ECC.p] = carry_bit;

  // modulo 필요

  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

// void BNF_complement()

void BNF_sub(const BN src1, const BN src2, BN* dst, ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  int borrow = 0;
  int cur_borrow = 0;
  word* temp = (word*)malloc((ECC.p + 1) * sizeof(word));
  WordArr_zerolize(temp, ECC.p + 1);

  for(int i = 0; i < ECC.p; i++)
  {
    cur_borrow = borrow;
    temp[i] = src1.arr[i] - src2.arr[i] - cur_borrow;

    if(src1.arr[i] < src2.arr[i])
    {
      borrow = 1;
    }
    else
    {
      borrow = 0;
    }

    if((src1.arr[i] == src2.arr[i]) && (cur_borrow == 1))
    {
      borrow = 1;
    }
  }
  
  // modulo 필요
  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

void BNF_mul_OS(const BN src1, const BN src2, BN* dst, const ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  long UV = 0;
  word* temp = (word*)malloc(2 * ECC.p * sizeof(word));
  WordArr_zerolize(temp, 2 * ECC.p);

  for(int i = 0; i < ECC.p; i++)
  {
    UV = 0;

    for(int j = 0; j < ECC.p; j++)
    {
      UV = (long)temp[i + j] + (long)src1.arr[i] * (long)src2.arr[j] + (UV >> 32);
      // 이유는 모르겠으나 UV >> 32로 하였을 때 ffffffff가 앞에 생겨버림.. sign extension 때문인가?
      temp[i + j] = (word) UV;
    }

    temp[i + ECC.p] = (word)(UV >> 32); // 여기서 모듈러 필요할 듯 보임. (reduction)
  }

  // modulo 필요

  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

void BNF_mul_PS(const BN src1, const BN src2, BN* dst, const ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  long UV = 0;
  int carry_bit = 0;
  long R[2] = {0, };
  word* temp = (word*)malloc(2 * ECC.p * sizeof(word));
  WordArr_zerolize(temp, 2 * ECC.p);

  for(int k = 0; k < 2 * ECC.p - 1; k++)
  {
    for(int i = 0; i <= k; i++)
    {
      if((i >= ECC.p) || (k - i >= ECC.p))
      {
        continue;
      }

      UV = (long)src1.arr[i] * (long)src2.arr[k - i];
      R[0] += UV;
      
      if(R[0] < UV)
      {
        carry_bit = 1;
      }
      else
      {
        carry_bit = 0;
      }

      R[1] += carry_bit;
    }
    temp[k] = (word)R[0];
    R[0] = (R[1] << 32) ^ (R[0] >> 32);
    R[1] = 0;
    carry_bit = 0;
  }

  temp[2*ECC.p - 1] = (word)R[0];

  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

void BNF_mul_KO(const BN src1, const BN src2, BN* dst, const ECC_info ECC)
{

}

void BNF_square(const BN src, BN* dst, const ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  long UV = 0;
  int carry_bit = 0;
  long R[2] = {0, };
  word* temp = (word*)malloc(2 * ECC.p * sizeof(word));
  WordArr_zerolize(temp, 2 * ECC.p);

  for(int k = 0; k < 2 * ECC.p - 1; k++)
  {
    for(int i = 0; i <= k - i; i++)
    {
      if((i >= ECC.p) || (k - i >= ECC.p))
      {
        continue;
      }

      UV = (long)src.arr[i] * (long)src.arr[k - i];

      if(i != k - i)
      {
        if((UV >> 63) == 1)
        {
          R[1]++;
        }
        UV <<= 1;
      }

      R[0] += UV;
      
      if(R[0] < UV)
      {
        carry_bit = 1;
      }
      else
      {
        carry_bit = 0;
      }

      R[1] += carry_bit;
    }
    temp[k] = (word)R[0];
    R[0] = (R[1] << 32) ^ ((R[0] >> 32) & 0xffffffff);
    R[1] = 0;
    carry_bit = 0;
  }

  temp[2*ECC.p - 1] = (word)R[0];

  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

void BNF_square2(const BN src, BN* dst, ECC_info ECC)
{
  BN_zerolize(dst, ECC);
  long UV = 0;
  word Wtemp[2] = {0,};
  int carry_bit = 0;
  word* temp = (word*)malloc(2*ECC.p * sizeof(word));

  for(int i = 0; i < ECC.p; i++)
  {
    UV = (long)temp[2 * i] + (long)src.arr[i] * (long)src.arr[i];
    temp[2 * i] = (word)UV;
    Wtemp[0] = (word)(UV >> 32);
    Wtemp[1] = 0;

    for(int j = i + 1; j < ECC.p; j++)
    {
      UV = temp[i + j] + (long)src.arr[i] * (long)src.arr[j] + Wtemp[0];
      Wtemp[0] = (word)(UV >> 32);
      UV = (UV & 0xffffffff) + (long)src.arr[i] * (long)src.arr[j] + Wtemp[1];
      Wtemp[1] = (word)(UV >> 32);
    }

    UV = (long)Wtemp[0] + (long)Wtemp[1];
    Wtemp[1] = (word)(UV >> 32);
    UV = (long)temp[i + ECC.p] + (UV & 0xffffffff);
    temp[i + ECC.p] = (word)UV;
    temp[i + ECC.p + 1] = Wtemp[1] + (word)(UV >> 32);
  }

  memcpy(dst->arr, temp, 4 * ECC.p);
  free(temp);
}

void test()
{
  // BN* a = NULL;
  // BN* b = NULL;
  // a = (BN*)malloc(sizeof(BN));
  // b = (BN*)malloc(sizeof(BN));

  // ECC_info* ECC = NULL;
  // ECC = (ECC_info*)malloc(sizeof(ECC_info));

  BN a;
  BN b;
  ECC_info ECC;

  ECC_init(&ECC, P256);

  printf("INITIALIZE OPERAND...\n");
  BN_init(&a, ECC);
  BN_init(&b, ECC);

  BN_print_hex(&a, ECC, "a");
  BN_print_hex(&b, ECC, "b");

  printf("SET OPERAND...\n");
  BN_set("0x00000000 00000000 00000000 00000000 00000000 00000000 00000000 ffffffff", &a, ECC);
  BN_set("0x00000000 00000000 00000000 00000000 00000000 00000000 80000000 00000000", &b, ECC);
  BN_print_hex(&a, ECC, "a");
  BN_print_hex(&b, ECC, "b");

  BN c;
  BN_init(&c, ECC);

  printf("ADD TEST...\n");
  BNF_add(a, b, &c, ECC);
  BN_print_hex(&c, ECC, "a + b = c");
  printf("\n");

  printf("SUB TEST...\n");
  BNF_sub(a, b, &c, ECC);
  BN_print_hex(&c, ECC, "a - b = c");
  printf("\n");

  printf("MUL TEST...\n");
  printf("OS VERSION...\n");
  BNF_mul_OS(a, b, &c, ECC);
  BN_print_hex(&c, ECC, "a * b = c");

  printf("PS VERSION...\n");
  BNF_mul_PS(a, b, &c, ECC);
  BN_print_hex(&c, ECC, "a * b = c");

  printf("KARATSUBA - OFMAN VERSION...\n");
  // BNF_mul_KO(a, b, &c, ECC);
  // BN_print_hex(&c, ECC, "a * b = c");
  printf("\n");

  printf("SQUARE TEST...\n");
  printf("Classical Squaring...\n");
  BNF_square(a, &c, ECC);
  BN_print_hex(&c, ECC, "a^2 = c");
  printf("\n");

  printf("Another Squaring...\n");
  BNF_square2(a, &c, ECC);
  BN_print_hex(&c, ECC, "a^2 = c");
  printf("\n");
}

void memcpy_test()
{
  int* a = (int*)malloc(2*sizeof(int));
  int* b = (int*)malloc(2*sizeof(int));
  
  printf("%p, %p\n", a, b);

  a[0] = 1;
  a[1] = 2;

  memcpy(b, a, 2);

  free(a);

  printf("%p, %p\n", a, b);

  for(int i = 0; i < 2; i++)
  {
    printf("%d", a[i]);
  }
  printf("\n");

  for(int i = 0; i < 2; i++)
  {
    printf("%d", b[i]);
  }
  printf("\n\n");
}

int main()
{
  memcpy_test();
  test();

  long a = 0xffffffff;

  return 0;
}