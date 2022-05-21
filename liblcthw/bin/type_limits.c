#include<stdio.h>
#include<limits.h>

int main() {
   printf("The number of bits in a byte = %d\n", CHAR_BIT);

   printf("\nThe minimum value of Signed CHAR is = %d\n", SCHAR_MIN);
   printf("The maximum value of Signed CHAR is = %d\n", SCHAR_MAX);
   printf("The minimum value of CHAR is = %d\n", CHAR_MIN);
   printf("The maximum value of CHAR is = %d\n", CHAR_MAX);
   printf("The maximum value of Unsigned CHAR is = %u\n", UCHAR_MAX);

   printf("\nThe minimum value of Signed Short is = %d\n", SHRT_MIN);
   printf("The maximum value of Signed Short is = %d\n", SHRT_MAX);
   printf("The maximum value of Unsigned Short is = %u\n",
                                                 USHRT_MAX);

   printf("\nThe minimum value of Signed INT is = %d\n", INT_MIN);
   printf("The maximum value of Signed INT is = %d\n", INT_MAX);
   printf("The maximum value of Unsigned INT is = %u\n", UINT_MAX);

   printf("\nThe minimum value of Signed LONG is = %ld\n", LONG_MIN);
   printf("The maximum value of Signed LONG is = %ld\n", LONG_MAX);
   printf("The maximum value of Unsigned LONG is = %lu\n",
                                                 ULONG_MAX);

   return 0;
}