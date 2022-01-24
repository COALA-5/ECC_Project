#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P224 7
// 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 00000000 00000000 00000001
#define P256 8
// 0xFFFFFFFF 00000001 00000000 00000000 00000000 FFFFFFFF FFFFFFFF FFFFFFFF
#define P384 12
// 0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFFFF 00000000 00000000 FFFFFFFF
#define P521 17
// 0x00001FF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF 

typedef unsigned int word;

static const word P224_table[][P224] = {
  {0x00000001, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff},
  {0x00000002, 0x00000000, 0x00000000, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff}
};

static const word P256_table[][P256] = {
  {0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xffffffff},
  {0xfffffffe, 0xffffffff, 0xffffffff, 0x00000001, 0x00000000, 0x00000000, 0x00000002, 0xfffffffe},
  {0xfffffffd, 0xffffffff, 0xffffffff, 0x00000002, 0x00000000, 0x00000000, 0x00000003, 0xfffffffd},
  {0xfffffffc, 0xffffffff, 0xffffffff, 0x00000003, 0x00000000, 0x00000000, 0x00000004, 0xfffffffc},
  {0xfffffffb, 0xffffffff, 0xffffffff, 0x00000004, 0x00000000, 0x00000000, 0x00000005, 0xfffffffb}
};

static const word P384_table [][P384] = {
  {0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff},
  {0xfffffffe, 0x00000001, 0x00000000, 0xfffffffe, 0xfffffffd, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff},
  {0xfffffffd, 0x00000002, 0x00000000, 0xfffffffd, 0xfffffffc, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff},
  {0xfffffffc, 0x00000003, 0x00000000, 0xfffffffc, 0xfffffffb, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff}
};

// static const word P256_table[][P521] = {
//   {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x000001ff},
//   {0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x000003ff}
// };

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

void BNF_print_hex(const BN* data, ECC_info* ECC, const char* name)
{
  if(name != NULL)
  {
    printf("%s = ", name);
  }
  printf("0x");
  for(int i = ECC->p - 1; i >= 0; i--)
  {
    printf("%08x ", data->arr[i]);
  }
  printf("\n");
}

void WordArr_zerolize(word* data, int size)
{
  for(int i = 0; i < size; i++)
  {
    data[i] = 0;
  }
}

void BNF_zerolize(BN* data, const ECC_info* ECC)
{
  WordArr_zerolize(data->arr, ECC->p);
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
    ECC->polynomial.arr[2] = 0;
    ECC->polynomial.arr[1] = 0;
    ECC->polynomial.arr[0] = 1;
  }

  else if (p_type == P256)
  {
    ECC->polynomial.arr[6] = 1;
    ECC->polynomial.arr[5] = 0;
    ECC->polynomial.arr[4] = 0;
    ECC->polynomial.arr[3] = 0;
  }

  else if (p_type == P384)
  {
    ECC->polynomial.arr[4] -= 1;
    ECC->polynomial.arr[2] = 0;
    ECC->polynomial.arr[1] = 0;
  }

  else if (p_type == P521)
  {
    ECC->polynomial.arr[16] = 0x1FF;
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

void hex2BNF(const char* hexstring, BN* data, const ECC_info* ECC)
{
  int i = (int)strlen(hexstring) - 1;
  int j = 0;
  int temp = 0;
  int k = 0;

  BNF_zerolize(data, ECC);

  while(i >= 2)
  {
    if(hexstring[i] == ' ')
    {
      j++;
      k = 0;
      i--;
      continue;
    }
    
    hex2digit(hexstring[i], &temp);
    data->arr[j] = data->arr[j] + (temp << (4 * k)) ;
    k++;
    i--;
  }
}

void BNF_init(BN* data, const ECC_info* ECC)
{
  data->arr = (word*)malloc(ECC->p * sizeof(word));
  for(int i = 0; i < ECC->p; i++)
  {
    data->arr[i] = 0;
  }
}

void BNF_free(BN* data)
{
  free(data->arr);
  data->arr = NULL;
  free(data);
  data = NULL;
}

void BNF_set(const char* hexstring, BN* data, const ECC_info* ECC)
{
  hex2BNF(hexstring, data, ECC);
}

int BNF_cmplt(const BN src1, const BN src2, const ECC_info* ECC)
{
  for(int i = ECC->p - 1; i >= 0; i++)
  {
    if(src1.arr[i] < src2.arr[i])
    {
      return 1;
    }
  }
  return 0;
}

void BN_add(BN src1, BN src2, BN* dst, const ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  int carry_bit = 0;
  int cur_carry_bit = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int i = 0; i < ECC->p; i++)
  {
    cur_carry_bit = carry_bit;
    temp->arr[i] = src1.arr[i] + src2.arr[i] + cur_carry_bit;

    if(src1.arr[i] + src2.arr[i] < src1.arr[i])
    {
      carry_bit = 1;
    }
    else
    {
      carry_bit = 0;
    }

    if(temp->arr[i] < cur_carry_bit)
    {
      carry_bit = 1;
    }
  }

  temp->arr[ECC->p] = carry_bit;
}

void BN_sub(BN src1, BN src2, BN* dst, ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  int borrow = 0;
  int cur_borrow = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);
  
  for(int i = 0; i < ECC->p; i++)
  {
    cur_borrow = borrow;
    temp->arr[i] = src1.arr[i] - src2.arr[i] - cur_borrow;

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
}

void BNF_P224Reduction(BN* data, const ECC_info* ECC)
{
  long UV = 0;
  UV = data->arr[]
}

void BNF_P256Reduction(BN* data, const ECC_info* ECC)
{
  long UV = 0;
}

void BNF_P384Reduction(BN* data, const ECC_info* ECC)
{
  long UV = 0;
}

void BNF_P521Reduction(BN* data, const ECC_info* ECC)
{
  long UV = 0;
}

void BNF_FastReduction(BN* data, const ECC_info* ECC)
{
  if (ECC->p == P224)
  {
    BNF_P224Reduction(data, ECC);
  }

  else if (ECC->p == P256)
  {
    BNF_P256Reduction(data, ECC);
  }

  else if (ECC->p == P384)
  {
    BNF_P384Reduction(data, ECC);
  }

  else if(ECC->p == P521)
  {
    BNF_P521Reduction(data, ECC);
  }

  else
  {
    printf("ERROR : prime p is not defined.\n");
  }
}

void BNF_add(const BN src1, const BN src2, BN* dst, const ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  int carry_bit = 0;
  int cur_carry_bit = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int i = 0; i < ECC->p; i++)
  {
    cur_carry_bit = carry_bit;
    temp->arr[i] = src1.arr[i] + src2.arr[i] + cur_carry_bit;

    if(src1.arr[i] + src2.arr[i] < src1.arr[i])
    {
      carry_bit = 1;
    }
    else
    {
      carry_bit = 0;
    }

    if(temp->arr[i] < cur_carry_bit)
    {
      carry_bit = 1;
    }
  }

  temp->arr[ECC->p] = carry_bit;

  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_sub(const BN src1, const BN src2, BN* dst, ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  int borrow = 0;
  int cur_borrow = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);
  
  for(int i = 0; i < ECC->p; i++)
  {
    cur_borrow = borrow;
    temp->arr[i] = src1.arr[i] - src2.arr[i] - cur_borrow;

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
  
  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_mul_OS(const BN src1, const BN src2, BN* dst, const ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  long UV = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int i = 0; i < ECC->p; i++)
  {
    UV = 0;

    for(int j = 0; j < ECC->p; j++)
    {
      UV = (long)temp->arr[i + j] + (long)src1.arr[i] * (long)src2.arr[j] + ((UV >> 32) & 0xffffffff);
      // 이유는 모르겠으나 UV >> 32로 하였을 때 ffffffff가 앞에 생겨버림.. sign extension 때문인가?
      temp->arr[i + j] = (word) UV;
    }

    temp->arr[i + ECC->p] = (word)(UV >> 32); // 여기서 모듈러 필요할 듯 보임. (reduction)
  }

  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_mul_PS(const BN src1, const BN src2, BN* dst, const ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  long UV = 0;
  int carry_bit = 0;
  long R[2] = {0, };
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int k = 0; k < 2 * ECC->p - 1; k++)
  {
    for(int i = 0; i <= k; i++)
    {
      if((i >= ECC->p) || (k - i >= ECC->p))
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
    temp->arr[k] = (word)R[0];
    R[0] = (R[1] << 32) ^ ((R[0] >> 32) & 0xffffffff);
    R[1] = 0;
    carry_bit = 0;
  }

  temp->arr[2*ECC->p - 1] = (word)R[0];

  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_mul_KO(const BN src1, const BN src2, BN* dst, const ECC_info ECC)
{
  return;
}

void BNF_square(const BN src, BN* dst, const ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  long UV = 0;
  int carry_bit = 0;
  long R[2] = {0, };
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int k = 0; k < 2 * ECC->p - 1; k++)
  {
    for(int i = 0; i <= k - i; i++)
    {
      if((i >= ECC->p) || (k - i >= ECC->p))
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
    temp->arr[k] = (word)R[0];
    R[0] = (R[1] << 32) ^ ((R[0] >> 32) & 0xffffffff);
    R[1] = 0;
    carry_bit = 0;
  }

  temp->arr[2*ECC->p - 1] = (word)R[0];

  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_square2(const BN src, BN* dst, ECC_info* ECC)
{
  BNF_zerolize(dst, ECC);
  long UV = 0;
  word Wtemp[2] = {0,};
  int carry_bit = 0;
  BN* temp = (BN*)malloc(sizeof(BN));
  temp->arr = (word*)malloc(2 * ECC->p * sizeof(word));
  WordArr_zerolize(temp->arr, 2 * ECC->p);

  for(int i = 0; i < ECC->p; i++)
  {
    UV = ((long)temp->arr[2 * i] & 0xffffffff) + ((long)src.arr[i] & 0xffffffff) * ((long)src.arr[i] & 0xffffffff);
    temp->arr[2 * i] = (word)UV;
    Wtemp[0] = (word)(UV >> 32);
    Wtemp[1] = 0;

    for(int j = i + 1; j < ECC->p; j++)
    {
      UV = ((long)temp->arr[i + j] & 0xffffffff) + ((long)src.arr[i] & 0xffffffff) * ((long)src.arr[j] & 0xffffffff) + Wtemp[0];
      Wtemp[0] = (word)(UV >> 32);
      UV = (UV & 0xffffffff) + ((long)src.arr[i] & 0xffffffff) * ((long)src.arr[j] & 0xffffffff) + ((long)Wtemp[1] & 0xffffffff);
      temp->arr[i + j] = (word)UV;
      Wtemp[1] = (word)(UV >> 32);
    }

    UV = ((long)Wtemp[0] & 0xffffffff) + ((long)Wtemp[1] & 0xffffffff);
    Wtemp[1] = (word)(UV >> 32);
    UV = ((long)temp->arr[i + ECC->p] & 0xffffffff) + (UV & 0xffffffff);
    temp->arr[i + ECC->p] = (word)UV;
    temp->arr[i + ECC->p + 1] = Wtemp[1] + (word)(UV >> 32);
  }
  BNF_FastReduction(temp, ECC);

  memcpy(dst->arr, temp->arr, ECC->p * sizeof(word));
  memcpy(dst, temp, sizeof(BN));
}

void BNF_EEA(const BN src1, const BN src2, BN* dst, const ECC_info* ECC)
{
  return;
}

void test()
{
  BN* a = NULL;
  BN* b = NULL;
  a = (BN*)malloc(sizeof(BN));
  b = (BN*)malloc(sizeof(BN));

  ECC_info* ECC = NULL;
  ECC = (ECC_info*)malloc(sizeof(ECC_info));

  ECC_init(ECC, P256);

  printf("INITIALIZE OPERAND...\n");
  BNF_init(a, ECC);
  BNF_init(b, ECC);

  BNF_print_hex(a, ECC, "a");
  BNF_print_hex(b, ECC, "b");
  printf("\n");

  printf("SET OPERAND...\n");
  BNF_set("0x0", a, ECC);
  BNF_set("0x1", b, ECC);
  BNF_print_hex(a, ECC, "a");
  BNF_print_hex(b, ECC, "b");
  printf("\n");

  BN* c = NULL;
  c = (BN*)malloc(sizeof(BN));
  BNF_init(c, ECC);

  printf("ADD TEST...\n");
  BNF_add(*a, *b, c, ECC);
  BNF_print_hex(c, ECC, "a + b = c");
  printf("\n");

  printf("SUB TEST...\n");
  BNF_sub(*a, *b, c, ECC);
  BNF_print_hex(c, ECC, "a - b = c");
  printf("\n");

  printf("MUL TEST...\n");
  printf("OS VERSION...\n");
  BNF_mul_OS(*a, *b, c, ECC);
  BNF_print_hex(c, ECC, "a * b = c");

  printf("PS VERSION...\n");
  BNF_mul_PS(*a, *b, c, ECC);
  BNF_print_hex(c, ECC, "a * b = c");

  printf("KARATSUBA - OFMAN VERSION...\n");
  // BNF_mul_KO(a, b, &c, ECC);
  // BNF_print_hex(&c, ECC, "a * b = c");
  printf("\n");

  printf("SQUARE TEST...\n");
  printf("Classical Squaring...\n");
  BNF_square(*a, c, ECC);
  BNF_print_hex(c, ECC, "a^2 = c");
  printf("\n");

  printf("Another Squaring...\n");
  BNF_square2(*a, c, ECC);
  BNF_print_hex(c, ECC, "a^2 = c");
  printf("\n");
}

void BNF_Gen_FastReduction_table(int P)
{
  int max_overflow = 0;

  if (P == P224)
  {
    max_overflow = 2;
  }
  if (P == P256)
  {
    max_overflow = 5;
  }
  if (P == P384)
  {
    max_overflow = 4;
  }
  if (P == P521)
  {
    max_overflow = 2;
  }
  BN* a = NULL;
  BN* b = NULL;
  a = (BN*)malloc(sizeof(BN));
  b = (BN*)malloc(sizeof(BN));

  ECC_info* ECC = NULL;
  ECC = (ECC_info*)malloc(sizeof(ECC_info));

  ECC_init(ECC, P);

  BNF_init(a, ECC);
  BNF_init(b, ECC);

  a->arr[0] = 1;

  printf("static const word P256_table [][%d] = {\n", max_overflow);
  for(int i = 0; i < max_overflow; i++)
  {
    BNF_mul_OS(ECC->polynomial, *a, b, ECC);
    a->arr[0]++;
    printf("  {");
    for(int i = 0; i < ECC->p; i++)
    {
      printf("0x%08x", b->arr[i]);
      if(i != ECC->p - 1)
      {
        printf(", ");
      }
    }
    printf("}");
    if(i != max_overflow - 1)
    {
      printf(",");
    }
    printf("\n");
  }
  printf("};\n");
}

int main()
{
  test();
  // BNF_Gen_FastReduction_table(P224);
  // BNF_Gen_FastReduction_table(P256);
  // BNF_Gen_FastReduction_table(P384);
  // BNF_Gen_FastReduction_table(P521);

  return 0;
}
