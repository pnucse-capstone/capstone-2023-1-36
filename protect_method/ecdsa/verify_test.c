typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef uint8_t uint256_t[32];
typedef uint8_t uint512_t[64];

const uint256_t secp256k1_n = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B, 0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41 };

const uint256_t G = { 0x79, 0xbe, 0x66, 0x7e, 0xf9, 0xdc, 0xbb, 0xac, 0x55, 0xa0, 0x62, 0x95, 0xce, 0x87, 0x0b, 0x07, 0x02, 0x9b, 0xfc, 0xdb, 0x2d, 0xce, 0x28, 0xd9, 0x59, 0xf2, 0x81, 0x5b, 0x16, 0xf8, 0x17, 0x98 };

const uint256_t public_key = { 0x5a, 0x47, 0xaf, 0xc9, 0x00, 0x79, 0x56, 0x33, 0xc3, 0x8c, 0x89, 0x5a, 0xd8, 0x7b, 0xf1, 0x6d, 0x70, 0x2c, 0xcf, 0x7b, 0x89, 0x8c, 0x54, 0x5f, 0xe7, 0xd2, 0x8f, 0x5b, 0xf1, 0xaf, 0x0f, 0x05 };

int add_u256( uint256_t result, const uint256_t a, const uint256_t b )
{
	uint16_t carry = 0;
    for ( int i = 31; i >= 0; -- i )
	{
        uint16_t sum = a[i] + b[i] + carry;
        result[i] = sum & 0xFF;
        carry = sum >> 8;
    }
	return carry;
}

void sub_u256( uint256_t result, const uint256_t a, const uint256_t b )
{
	int borrow = 0;
    for ( int i = 31; i >= 0; -- i )
	{
        int diff = a[i] - b[i] - borrow;
        if ( diff < 0 )
		{
            result[i] = ( uint8_t ) ( diff + 256 );
            borrow = 1;
        }
		else
		{
            result[i] = ( uint8_t ) diff;
            borrow = 0;
        }
    }
}

void mul_u256( uint512_t result, const uint256_t a, const uint256_t b ) {
	uint16_t r01 = 0;
	uint8_t r2 = 0;

	for ( int k = 0; k < 63; ++ k )
	{
		unsigned l_min = ( k < 32 ? 0 : ( k + 1 ) - 32 );
		for ( int i = l_min; i <= k && i < 32; ++ i )
		{
			uint16_t l_product = (uint16_t) a[31 - i] * b[31 - ( k - i )];
			r01 += l_product;
			r2 += (r01 < l_product);
		}
		result[63 - k] = (uint8_t)r01;
		r01 = (r01 >> 8) | (((uint16_t)r2) << 8);
        r2 = 0;
	}
	result[0] = (uint8_t) r01;
}

int cmp_u256( const uint256_t a, const uint256_t b )
{
	for ( int i = 0; i < 32; ++ i )
	{
		if ( a[i] > b[i] )
		{
			return 1;
		}
		if ( a[i] < b[i] )
		{
			return -1;
		}
	}
	return 0;
}

int is_zero_u256( const uint256_t num )
{
	for ( int i = 0; i < 32; ++ i )
	{
		if ( num[i] != 0 )
		{
			return 0;
		}
	}
	return 1;
}

void rshift_u256( uint256_t num )
{
	uint8_t carry = 0;
	for ( int i = 0; i < 32; ++ i )
	{
		uint8_t tmp = num[i];
		num[i] = tmp >> 1 | carry;
		carry = tmp << 7;
	}
}
void rshift_u512( uint512_t num )
{
	uint8_t carry = 0;
	for ( int i = 0; i < 64; ++ i )
	{
		uint8_t tmp = num[i];
		num[i] = tmp >> 1 | carry;
		carry = tmp << 7;
	}
}

void assign_u256( uint256_t dest, const uint256_t src )
{
	for ( int i = 0; i < 32; ++ i )
	{
		dest[i] = src[i];
	}
}

unsigned int num_bits( const uint256_t target )
{
	unsigned int num_digits;
	uint8_t digit;
	
	for ( num_digits = 31; num_digits >= 0 && target[31 - num_digits] == 0; -- num_digits )
		;
	num_digits ++;
	if ( num_digits == 0 )
		return 0;

	digit = target[31 - ( num_digits - 1 )];
	int i;
	for (i = 0; digit; i++)
		digit >>= 1;

	return ( ( num_digits - 1 ) * 8 + i );
}

void mod_u256( uint256_t result, uint512_t target, const uint256_t mod )
{
	uint512_t mod_m = { 0, };
	uint512_t tmp = {0, };
	uint8_t *v[2] = { tmp, target };

	int shift = 32 * 2 * 8 - num_bits( mod );
	int word_shift = shift / 8;
	int bit_shift = shift % 8;
	int carry = 0;
	
	if ( bit_shift > 0 )
	{
		for ( int i = 0; i < 32; ++ i )
		{
			mod_m[63 - ( word_shift + i )] = (mod[31 - i] << bit_shift) | carry;
			carry = mod[31 - i] >> (8 - bit_shift);
		}
	}
	else
	{
		for ( int i = 0; i < 32; ++ i )
		{
			mod_m[63 - ( i + word_shift )] = mod[31 - i];
		}
	}

	int index;
	for ( index = 1; shift >= 0; -- shift )
	{
		unsigned borrow = 0;
		for ( int i = 0; i < 64; ++ i )
		{
			uint8_t diff = v[index][63 - i] - mod_m[63 - i] - borrow;
			if ( diff != v[index][63 - i] )
			{
				borrow = ( diff > v[index][63 - i] );
			}
			v[1 - index][63 - i] = diff;
		}
		index = !(index ^ borrow );
		rshift_u256( &mod_m[32] );
		mod_m[32] |= mod_m[31] << 7;
		rshift_u256( mod_m );
	}
	assign_u256( result, &v[index][32] );
}

void mul_mod_u256( uint256_t result, const uint256_t a, const uint256_t b, const uint256_t mod )
{
	uint512_t mul;
	mul_u256( mul, a, b );
	mod_u256( result, mul, mod );
}

void inverse_mod( uint256_t result, const uint256_t target, const uint256_t mod )
{
	uint256_t a = { 0, };
	uint256_t b = { 0, };
	uint256_t u = { 0, };
	uint256_t v = { 0, };
	uint8_t carry;
	int cmp_result;

	if ( is_zero_u256( target ) )
	{
		for ( int i = 0; i < 32; ++ i )
		{
			result[i] = 0;
		}
		return ;
	}

	assign_u256( a, target );
	assign_u256( b, mod );
	u[31] = 1;

	while ( ( cmp_result = cmp_u256( a, b ) ) != 0 )
	{
		carry = 0;
		if ( a[31] % 2 == 0 )
		{
			rshift_u256( a );
			if ( u[31] % 2 != 0 )
			{
				carry = add_u256( u, u, mod );
			}
			rshift_u256( u );

			if ( carry )
			{
				u[0] |= 0x80;
			}
		}
		else if ( b[31] % 2 == 0 )
		{
			rshift_u256( b );
			if ( v[31] % 2 != 0 )
			{
				carry = add_u256( v, v, mod );
			}
			rshift_u256( v );

			if ( carry )
			{
				v[0] |= 0x80;
			}
		}
		else if ( cmp_result > 0 )
		{
			sub_u256( a, a, b );
			rshift_u256( a );

			if ( cmp_u256( u, v ) < 0 )
			{
				add_u256( u, u, mod );
			}
			sub_u256( u, u, v );

			if ( u[31] % 2 != 0 )
			{
				carry = add_u256( u, u, mod );
			}

			rshift_u256( u );

			if ( carry )
			{
				u[0] |= 0x80;
			}
		}
		else
		{
			sub_u256( b, b, a );
			rshift_u256( b );

			if ( cmp_u256( v, u ) < 0 )
			{
				add_u256( v, v, mod );
			}
			sub_u256( v, v, u );
			if ( v[31] % 2 != 0 )
			{
				carry = add_u256( v, v, mod );
			}
			rshift_u256( v );
			if ( carry )
			{
				v[0] |= 0x80;
			}
		}
	}
	assign_u256( result, u );
}

int verify( uint256_t msg, uint256_t r, uint256_t s )
{
	uint256_t s1 = { 0, };
	uint256_t u1 = { 0, };
	uint256_t u2 = { 0, };
	uint256_t v = { 0, };
	if ( is_zero_u256( r ) || cmp_u256( r, secp256k1_n ) >= 0 ||
	    is_zero_u256( s ) || cmp_u256( s, secp256k1_n ) >= 0 )
	{
		return 0;
	}

	inverse_mod( s1, s, secp256k1_n );
	mul_mod_u256( u1, msg, s1, secp256k1_n );
	mul_mod_u256( u2, r, s1, secp256k1_n );

	uint256_t t1 = { 0, };
	uint256_t t2 = { 0, };
	jacobian_u256( t1, u1, G, secp256k1_n );
	jacobian_u256( t2, u2, public_key, secp256k1_n );

	int carry = add_u256( v, t1, t2 );
	if ( carry || cmp_u256( secp256k1_n, v ) != 1 )
	{
		sub_u256( v, v, secp256k1_n );
	}
	return cmp_u256( v, r ) == 0;
}