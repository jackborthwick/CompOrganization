#include <stdio.h>
void print_bits_generic( unsigned char *ptr, size_t num_bytes )
{
    int byte_idx;
    for( byte_idx = num_bytes - 1; byte_idx >= 0; --byte_idx )
    {
        int bit_idx;
        unsigned char byte = ptr[ byte_idx ];
        for( bit_idx = 7; bit_idx >= 0; --bit_idx )
        {
            int bit = ( byte >> bit_idx ) & 1;
            printf( "%d", bit );
        }
    }
}

void print_bits_int( int i )
{
    unsigned char *p = (unsigned char *)&i;
    print_bits_generic( p, sizeof( int ) );
}

void print_bits_float( float i )
{
    unsigned char *p = (unsigned char *)&i;
    print_bits_generic( p, sizeof( int ) );
}


int main(int argc, char **argv)
{
	int x01 = 42;
    int x02 = -42;
    int x03 = 1 << 17;
    int x04 = x03 - 1;
    int x05 = 1 << 22;
    int x06 = x05 - 1;
    int x07 = x04 ^ x06;
    int x08 = 1 << 31;
    int x09 = -x08;
    int x10 = x08;
    unsigned x11 = 1.25;
    float x12 = -1;
    unsigned x13 = 0x80000000;
    float x14 = x13^x11;

 	print_bits_int( x01 );
    printf( " ( x01 )\n" );
    print_bits_int( x02 );
    printf( " ( x02 )\n" );
    print_bits_int( x03 );
    printf( " ( x03 )\n" );
    print_bits_int( x04 );
    printf( " ( x04 )\n" );
    print_bits_int( x05 );
    printf( " ( x05 )\n" );
    print_bits_int( x06 );
    printf( " ( x06 )\n" );
    print_bits_int( x07 );
    printf( " ( x07 )\n" );
    print_bits_int( x08 );
    printf( " ( x08 )\n" );
    print_bits_int( x09 );
    printf( " ( x09 )\n" );
    print_bits_int( x10 );
    printf( " ( x10 )\n" );
    print_bits_float( x11 );
    printf( " ( x11 )\n" );
    print_bits_float( x12 );
    printf( " ( x12 )\n" );
    print_bits_float( x13 );
    printf( " ( x13 )\n" );
    print_bits_float( x14 );
    printf( " ( x14 )\n" );
}
