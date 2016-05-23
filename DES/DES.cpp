#include < bitset >
#include < vector >
#include < string >
#include < iostream >

using namespace std;
//using namespace gy;

typedef bitset< 4 > _b4;
typedef bitset< 6 > _b6;
typedef bitset< 8 > _b8;
typedef bitset< 28 > _b28;
typedef bitset< 32 > _b32;
typedef bitset< 48 > _b48;
typedef bitset< 56 > _b56;
typedef bitset< 64 > _b64;

typedef vector< _b8 > _vb8;
typedef vector< _b48 > _vb48;
typedef vector< _b64 > _vb64;

namespace {
	
	const unsigned char odd_parity[256]={
		1,  1,  2,  2,  4,  4,  7,  7,  8,  8, 11, 11, 13, 13, 14, 14,
		16, 16, 19, 19, 21, 21, 22, 22, 25, 25, 26, 26, 28, 28, 31, 31,
		32, 32, 35, 35, 37, 37, 38, 38, 41, 41, 42, 42, 44, 44, 47, 47,
		49, 49, 50, 50, 52, 52, 55, 55, 56, 56, 59, 59, 61, 61, 62, 62,
		64, 64, 67, 67, 69, 69, 70, 70, 73, 73, 74, 74, 76, 76, 79, 79,
		81, 81, 82, 82, 84, 84, 87, 87, 88, 88, 91, 91, 93, 93, 94, 94,
		97, 97, 98, 98,100,100,103,103,104,104,107,107,109,109,110,110,
		112,112,115,115,117,117,118,118,121,121,122,122,124,124,127,127,
		128,128,131,131,133,133,134,134,137,137,138,138,140,140,143,143,
		145,145,146,146,148,148,151,151,152,152,155,155,157,157,158,158,
		161,161,162,162,164,164,167,167,168,168,171,171,173,173,174,174,
		176,176,179,179,181,181,182,182,185,185,186,186,188,188,191,191,
		193,193,194,194,196,196,199,199,200,200,203,203,205,205,206,206,
		208,208,211,211,213,213,214,214,217,217,218,218,220,220,223,223,
		224,224,227,227,229,229,230,230,233,233,234,234,236,236,239,239,
		241,241,242,242,244,244,247,247,248,248,251,251,253,253,254,254
	};

	const int keyperm_table56[56] = {
		57, 49, 41, 33, 25, 17,  9,  1, 
		58, 50, 42, 34, 26, 18, 10,  2, 
		59, 51, 43, 35, 27, 19, 11,  3, 
		60, 52, 44, 36, 63, 55, 47, 39, 
		31, 23, 15,  7, 62, 54, 46, 38, 
		30, 22, 14,  6, 61, 53, 45, 37, 
		29, 21, 13,  5, 28, 20, 12,  4
	};

	const int keycompperm_table[48] = {
		14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
	};

	const int keyshift_table[16] = {
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
	};

	const int datainitperm_table[64] = {
		58, 50, 42, 34, 26, 18, 10,  2,
		60, 52, 44, 36, 28, 20, 12,  4,
		62, 54, 46, 38, 30, 22, 14,  6,
		64, 56, 48, 40, 32, 24, 16,  8,
		57, 49, 41, 33, 25, 17,  9,  1,
		59, 51, 43, 35, 27, 19, 11,  3,
		61, 53, 45, 37, 29, 21, 13,  5,
		63, 55, 47, 39, 31, 23, 15,  7
	};

	const int dataexpperm_table[48] = {
		32,  1,  2,  3,  4,  5,
		 4,  5,  6,  7,  8,  9,
		 8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1
	};

	const int datasbox_table[8][4][16] = {
		{
			{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
			{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
			{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
			{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
		},
		{
			{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
			{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
			{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
			{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
		},
		{
			{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
			{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
			{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
			{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
		},
		{
			{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
			{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
			{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
			{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
		},
		{
			{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
			{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
			{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
			{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
		},
		{
			{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
			{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
			{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
			{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
		},
		{
			{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
			{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
			{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
			{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
		},
		{
			{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
			{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
			{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
			{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
		}
	};

	const int datapperm_table[32] = {
		16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
	};

	const int datafp_table[64] = {
		40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
	};

	template < size_t _NB >
	void rotateleft_bitset(bitset< _NB >& input, size_t step)
	{
		bitset< _NB > tmp = input;
		input <<= step;
		tmp >>= _NB - step;
		input |= tmp;
	}

	template < size_t _NB >
		void rotateright_bitset(bitset< _NB >& input, size_t step)
	{
		bitset< _NB > tmp = input;
		input >>= step;
		tmp <<= _NB - step;
		input |= tmp;
	}

	template < class Input, class Output, class Table>
	void Des_Permutation( const Input& input, Output& output, const Table& table, size_t TableSize)
	{
		for (size_t i = 0; i < TableSize; ++i) {
			output[i] = input[ table[i]-1 ];
		}
	}

	void Des_Key_Permutation1(const _b64& input, _b56& output)
	{
		Des_Permutation(input, output, keyperm_table56, sizeof(keyperm_table56)/sizeof(int));
	}

	void Des_Key_CompPerm(const _b56& input, _b48& output)
	{
		Des_Permutation(input, output, keycompperm_table, sizeof(keycompperm_table)/sizeof(int));
	}

	void Des_Key_Shift(_b56& key56, size_t step)
	{
		_b28 l28, r28;
		int i;
		for (i = 0; i < 28; ++i) {
			r28[i] = key56[i];
			l28[i] = key56[28+i];
		}
		rotateleft_bitset(l28, step);
		rotateleft_bitset(r28, step);
		for (i = 0; i < 28; ++i) {
			key56[i] = r28[i];
			key56[28+i] = l28[i];
		}
	}
	
	void Des_Key_Generate(const _b64& key, _vb48& subkey)
	{
		_b56 tmp56;
		Des_Key_Permutation1(key, tmp56);

		for (int i = 0; i < 16; ++i) {
			_b48 tmp48;
			Des_Key_Shift(tmp56, keyshift_table[i]);
			Des_Key_CompPerm(tmp56, tmp48);
			cout << "Key: " << tmp48 << endl;
			subkey.push_back(tmp48);
		}
	}

	void Des_Char_To_Binary(char c, string& binary)
	{
		unsigned char tmp = odd_parity[c];
		binary.append( (tmp & 0x80) ? "1" : "0"  );
		binary.append( (tmp & 0x40) ? "1" : "0"  );
		binary.append( (tmp & 0x20) ? "1" : "0"  );
		binary.append( (tmp & 0x10) ? "1" : "0"  );
		binary.append( (tmp & 0x08) ? "1" : "0"  );
		binary.append( (tmp & 0x04) ? "1" : "0"  );
		binary.append( (tmp & 0x02) ? "1" : "0"  );
		binary.append( (tmp & 0x01) ? "1" : "0"  );
	}

	void Des_String_To_Binary(const string& input, _b64& output)
	{
		string binary;
		for (int i = 0; i < input.length(); ++i ) {
			Des_Char_To_Binary(input[i], binary);
		}
		_b64 tmp64(binary);
		output = tmp64;
	}

	void Des_String_To_Key(const string& key, _vb48& subkey)
	{
		_b64 tmp64;
		Des_String_To_Binary(key, tmp64);
		Des_Key_Generate(tmp64, subkey);
	}

	void Des_Data_ExpPerm(const _b32& input, _b48& output)
	{
		Des_Permutation(input, output, dataexpperm_table, sizeof(dataexpperm_table)/sizeof(int));
	}

	void Des_Data_IP(const _b64& input, _b64& output)
	{
		Des_Permutation(input, output, datainitperm_table, sizeof(datainitperm_table)/sizeof(int));
	}

	void Des_Data_S(const _b6& input, _b4& output, int step)
	{
		bitset< 2 > tmp2;
		_b4 tmp4;
		tmp2[0] = input[5];
		tmp2[1] = input[0];
		tmp4[0] = input[1];
		tmp4[1] = input[2];
		tmp4[2] = input[3];
		tmp4[3] = input[4];
		_b8 t8(datasbox_table[step][tmp2.to_ulong()][tmp4.to_ulong()]);
		for (int i = 0; i < 4; ++i) {
			output[i] = t8[i];
		}
	}

	void Des_Data_S(const _b48& input, _b32& output)
	{
		_b6 tmp6;
		_b4 tmp4;
		int i, j;
		for (i = 0; i < 8; ++i) {
			for (j = 6; j < 0; --j) {
				tmp6[j] = input[ 47 - i*6 - j];
			}
			Des_Data_S(tmp6, tmp4, i);
			for (j = 0; j < 4; ++j) {
				output[ 31 - (i << 2) - j] = tmp4[j];
			}
		}
	}

	void Des_Data_P(const _b32& input, _b32& output)
	{
		Des_Permutation(input, output, datapperm_table, sizeof(datapperm_table)/sizeof(int));
	}

	void Des_Data_F(_b64& input, const _b48& key)
	{
		_b32 l32, r32, tmp32, t32;
		_b48 exp48;
		int i;
		for (i = 0; i < 32; ++i ) {
			r32[i] = input[i];
			l32[i] = input[32+i];
		}
		//cout << "r32** " << r32 << endl;
		Des_Data_ExpPerm(r32, exp48);
		//cout << "exp** " << exp48 << endl;
		//cout << "key** " << key << endl;
		exp48 ^= key;
		//cout << "exp** " << exp48 << endl;
		Des_Data_S(exp48, tmp32);
		//cout << "tmp** " << tmp32 << endl;
		Des_Data_P(tmp32, t32);
		//cout << "t32** " << t32 << endl;
		t32 ^= l32;
		l32 = r32;
		r32 = t32;
		for (i = 0; i < 32; ++i ) {
			input[i] = r32[i];
			input[32+i] = l32[i];
		}
	}

	void Des_data_FP(const _b64& input, _b64& output)
	{
		Des_Permutation(input, output, datafp_table, sizeof(datafp_table)/sizeof(int));
	}
	
	int Des_encode_64bit(const _vb48& key, const _b64 &data, _b64 &output)
	{
		_b64 tmp64;

		Des_Data_IP(data, tmp64);
		for (int i = 0; i < 16; ++i) {
			//cout << "	" << tmp64 << endl;
			Des_Data_F(tmp64, key[i]);
			//cout << "	" << tmp64 << endl;
		}
		Des_data_FP(tmp64, output);
		return 0;
	}
	
	int Des_encode(const string& key, const string& data, _b64 &output)
	{
		_vb48 subkey, decodekey;
		_b64 data64, tmp64;

		Des_String_To_Key(key, subkey);

		for (int i = 0; i < 16; ++i) {
			decodekey.push_back(subkey[ 15-i ]);
		}

		Des_String_To_Binary(data, data64);

		//cout << "data:	" << data64 << endl;

		Des_encode_64bit(subkey, data64, output);
		//cout << "code:	" << output << endl;
		Des_encode_64bit(decodekey, output, tmp64);
		//cout << "reco:	" << tmp64 << endl;
		if (tmp64 == data64) {
			cout << "Congratulation.$$$$$$$$$$$___FCKpd___1quot" << endl;
		}else {
			cout << "Error. **************************" << endl;
		}

		return 0;
	}
	
	int Des_decode(const string& key, const string& data, _b64 &output)
	{
		return 0;
	}
}

namespace gy {
		
	/* Mode: true for encode, false for decode */
	int Des(const string& key, const string& data, _b64 &output, bool Mode)
	{
		if (Mode) {
			return Des_encode(key, data, output);
		}else {
			return Des_decode(key, data, output);
		}
	}
	
void Run_Des(void) 
{
		int select = 0;
		string data, key;
		cout << "1. encode;" << endl << "2. decode;" << endl << "Your choise: ";
		cin >> select;
		cout << endl << "Please input the data: ";
		cin >> data;
		cout << endl << "Please input the key: ";
		cin >> key;

		_b64 output;

		if (select == 1) {
			Des(key, data, output, true);
		}else {
			Des(key, data, output, false);
		}
	}
	
}

int main()
{
	gy::Run_Des();
	__int64 a;
	return 0;
}