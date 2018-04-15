#include "head.h"
_u8 *P;
_u8 *Q;
_u8 *fai;
_u8 **PublicKey;
_u8 **PrivateKey;
int rangeA;
void CreatePublic();
void CreatePrivate();
_u8 *Fai();
int TestPrime[303] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373,
379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443,
449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521,
523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757,
761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839,
853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929,
937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013,
1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451,
1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733,
1741, 1747, 1753, 1759, 1777, 1783, 1787 ,1789, 1801, 1811,
1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
1993, 1997, 1999};
int BitTest[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };
int RealRand(int range) {
	srand((unsigned)time(0) + rand());
	srand((unsigned)time(0)%10 + rand());
	range = rand() % (range + 1);
	return range;
}
extern _u8 **PublicKey;
_u8 *Ini() {
	_u8 *X = (_u8*)malloc(sizeof(_u8));
	X->len = LEN;
	X->sign = 0;
	X->data = (unsigned char*)malloc(LEN * sizeof(unsigned char));
	for (int j = 0; j < LEN; j++)
		X->data[j] = 0;
	return X;
}
int Print(_u8 *X) {
	for (int i = 0; i < X->len; i++)
		printf("%02X", X->data[i]);
	putchar('\n');
	return 0;
}
void destroy(_u8 *de){
	free(de->data);
	free(de);
}
int EqualOne(_u8 *X) {
	if (X->sign)
		return 0;
    if(X->data[0] != 1)
        return 0;
	if (X->data[0] == 1)
		for (int i = 1; i < X->len; i++)
			if (X->data[i])
				return 0;
	return 1;
}
int EqualN_1(_u8 *X, _u8 *N) {
	_u8 *Temp = Ini();
	Temp->data[0] = 1;
	_u8 *temp = Sub(N, Temp);
	destroy(Temp);
	if (Compare(temp, X) == 0) {
		destroy(temp);
		return 1;
	}
	return 0;
}
int EqualZero(_u8 *X) {
	for (int i = 0; i < X->len; i++)
		if (X->data[i])
				return 0;
	return 1;
}

_u8 *Odd(int range) {
	_u8 *X = Ini();
	X->data[0] = RealRand(127) * 2 + 1;
	for (int i = 1; i < range - 1; i++) {
		X->data[i] = RealRand(255);
	}
	X->data[range - 1] = RealRand(1 + 254);
	return X;
}
int F_P(_u8* X) {
	int rmd, n = LEN - 1;
	while (X->data[n] == 0)
		n--;
	for (int j = 1; j < 303; j++) {
		rmd = 0;
		for (int i = n; i >= 0; i--)
			rmd = (rmd * 256 + X->data[i]) % TestPrime[j];
		if (rmd == 0) {
			destroy(X);
			return 1;
		}
	}
	return 0;
}
_u8 *FakePrime(int range) {
	_u8 *X = Odd(range);
	while (F_P(X))
		X = Odd(range);
	return X;
}
_u8 *MR_rand(_u8 *N) {
	_u8 *A = Ini();
	int n = LEN - 1;
	A->data[0] = 1 + RealRand(254);
	while(N->data[n] == 0)
		n--;
	for (int i = 1; i < n; i++)
		A->data[i] = RealRand(255);
	A->data[n] = RealRand(N->data[n] - 1);
	return A;
}
int Witness(_u8 *a,_u8 *n) {
    _u8 *temp = Ini();
	_u8 *Temp = temp;
    temp->data[0] = 1;
    temp = Sub(n, temp);
	factor *R = RightMove(temp);
	destroy(temp);
	destroy(Temp);

	_u8 *X = ModExp(a, R->u, n);
	_u8 *Y = NULL;

	for (int i = 0; i < R->t; i++) {
		Temp = Multiplicate(X, X);
		Y = Mod(Temp, n);
		destroy(Temp);
		if ((EqualOne(Y)) && EqualOne(X)==0 && EqualN_1(X, n)==0) {
			destroy(X);
			destroy(Y);
			destroy(R->u);
			free(R);
			return 1;
		}
		destroy(X);
		X = Y;
	}
	destroy(R->u);
	free(R);
	if (EqualOne(Y)==0)
		return 1;
	return 0;
}
int Miler_Rabin(_u8 *n, int s) {
	_u8 *a;
	for (int i = 0; i < s; i++) {
		a = MR_rand(n);
		if (Witness(a, n)) {
			destroy(n);
			return 1;
		}
		destroy(a);
	}
	return 0;
}
_u8 *MRFakePrime(){
	rangeA = 32 + RealRand(64);
	_u8 *X = FakePrime(rangeA);
	while (Miler_Rabin(X, TIME)) {
		X = FakePrime(rangeA);
	}
	return X;
}
_u8 *imirPefaFRM(_u8 *X) {
	_u8 *Y = FakePrime(128 - rangeA);
	while (Miler_Rabin(Y, TIME)) {
		Y = FakePrime(128 - rangeA);
	}
	return Y;
}

_u8 *Add(_u8 *A, _u8 *B) {
	_u8 *C = Ini();
	int i , carry = 0, sign;
	unsigned int temp;
	//Judge the sign
	if ((A->sign ^ B->sign) == 0)
		sign = A->sign;
	else
		if (A->sign == 1)
		{
			A->sign = 0;
			C = Sub(B, A);
			A->sign = 1;
			return C;
		}
		else
		{
			B->sign = 0;
			C = Sub(A, B);
			B->sign = 1;
			return C;
		}

	C = Ini();
	C->sign = sign;
	//Calculate
	for (i = 0; i < B->len; i++) {
		temp = A->data[i] + B->data[i] + carry;
		C->data[i] = temp % 256;
		carry = temp / 256;
	}
	for (; i < A->len; i++) {

		temp = A->data[i] + carry;
		C->data[i] = temp % 256;
		carry = temp / 256;
	}

	if (carry != 0)
		C->data[i] = carry;
	return C;
}
_u8 *Sub(_u8 *A, _u8 *B) {
	//A > B; A - B = R;
	int i;
	char flag = 0;
	_u8 *R;


	//Judge the sign
	if ((A->sign ^ B->sign) == 1)
	{
		if (A->sign == 1)
		{
			A->sign = 0;
			R = Add(A, B);
			A->sign = 1;
			R->sign = 1;
			return R;
		}
		else
		{
			B->sign = 0;
			R = Add(A, B);
			B->sign = 1;
			R->sign = 0;
			return R;
		}
	}
	else
	{
		if (Compare(A, B) == -1)
		{
			R = Sub(B, A);
			R->sign = A->sign ^ 1;
			return R;
		}
	}

	R = Ini();
	for (i = 0; i < B->len; i++) {
		if (A->data[i] < B->data[i] + flag) {
			R->data[i] = (256 - B->data[i] - flag) + A->data[i];
			flag = 1;
		}
		else {
			R->data[i] = A->data[i] - B->data[i] - flag;
			flag = 0;
		}
	}
	for (; i < A->len; i++)
		if (A->data[i] < flag) {
			R->data[i] = (256 - flag) + A->data[i];
			flag = 1;
		}
		else {
			R->data[i] = A->data[i] - flag;
			flag = 0;
		}
		return R;
}
int Compare(_u8* A, _u8* B)
{
	//if A > B return 1
	//if A == B return 0
	//if A < B return -1
	int i, j;
	//Get the index of biggest bit
	for (i = A->len - 1; i >= 0; i--)
		if (A->data[i] != 0)
			break;
	//Get the index of biggest bit
	for (j = B->len - 1; j >= 0; j--)
		if (B->data[j] != 0)
			break;
	if (i > j)
		return 1;
	else if (i < j)
		return -1;

	for (; j >= 0; j--)
		if (A->data[j] > B->data[j])
			return 1;
		else if (A->data[j] < B->data[j])
			return -1;

	return 0;
}
_u8 *Mod(_u8* A, _u8* B){
	//A Mod B
	_u8 *C, *mB, *temp;
	int i, j;
	int Cfirst = A->len - 1, Bfirst = B->len - 1;

	C = Ini();
	for (i = 0; i < C->len; i++)
		C->data[i] = A->data[i];

	while (C->data[Cfirst--] == 0);
	while (B->data[Bfirst--] == 0);
	Cfirst++, Bfirst++;

	while (Cfirst > Bfirst + 1) {
		mB = Ini();
		for (i = 0, j = 0; i < Cfirst; i++) {

			if (i < Cfirst - 1 - Bfirst)
				mB->data[i] = 0;
			else {
				mB->data[i] = B->data[j];
				j++;
			}
		}
		temp = mB;
		mB = Mul(mB, C->data[i]);
		destroy(temp);
		while (Compare(C, mB) != -1) {
			temp = Sub(C, mB);
			if (Compare(temp, B) == -1) {
				destroy(temp);
				break;
			}
			else {
				destroy(C);
				C = temp;
			}
		}
		while (C->data[Cfirst--] == 0);
		Cfirst++;
		destroy(mB);
	}
	while (Compare(C, B) == 1) {
		temp = C;
		C = Sub(C, B);
		destroy(temp);
	}
	if (Compare(C, B) == 0) {
		destroy(C);
		C = Ini();
		C->data[0] = 0;
	}
	return C;
}
_u8 *Div(_u8 *A, _u8*B){
	//A/B
	_u8 *C, *mB, *temp, *result, *one, *tresult;
	int i, j;
	int Cfirst = A->len - 1, Bfirst = B->len - 1;
	C = Ini();
	for (i = 0; i < C->len; i++)
		C->data[i] = A->data[i];
	result = Ini();
	for (i = 0; i < result->len; i++)
		result->data[i] = 0;
	one = Ini();
	one->data[0] = 1;
	while ((Cfirst >= 0) && C->data[Cfirst--] == 0);
	while ((Bfirst >= 0) && B->data[Bfirst--] == 0);
	Cfirst++, Bfirst++;
	while (Cfirst > Bfirst + 1) {
		mB = Ini();
		for (i = 0, j = 0; i < Cfirst; i++) {

			if (i < Cfirst - 1 - Bfirst)
				mB->data[i] = 0;
			else {
				mB->data[i] = B->data[j];
				j++;
			}
		}
		temp = mB;
		mB = Mul(mB, C->data[i]);
		destroy(temp);

		tresult = Ini();
		for (j = 0; j < tresult->len; j++)
			tresult->data[j] = 0;
		tresult->data[Cfirst - Bfirst - 1] = 1;

		temp = tresult;
		tresult = Mul(tresult, C->data[i]);
		free(temp);

		while (Compare(C, mB) != -1) {
			temp = Sub(C, mB);
			if (Compare(temp, B) == -1) {
				destroy(temp);
				break;
			}
			else {
				destroy(C);
				C = temp;
			}

			temp = result;
			result = Add(result, tresult);
			destroy(temp);


		}
		destroy(tresult);
		while ((Cfirst >= 0) && C->data[Cfirst--] == 0);
		Cfirst++;
		destroy(mB);
	}
	while (Compare(C, B) == 1) {
		temp = C;
		C = Sub(C, B);
		destroy(temp);
		temp = result;
		result = Add(result, one);
		destroy(temp);
	}
	if (Compare(C, B) == 0) {
		destroy(C);
		C = Ini();
		C->data[0] = 0;
		temp = result;
		result = Add(result, one);
		destroy(temp);
	}
	return result;
}
_u8 *Multiplicate(_u8 *A, _u8 *B) {
	if (EqualZero(A) || EqualZero(B)) {
		_u8 *Temp = Ini();
		Temp->data[0]=0;
		return Temp;
	}
	int r = 0, carry = 0;
	_u8 *R;
	R = Ini();
	R->sign = A->sign^B->sign;
	for (int i = 0; i < A->len; i++) {
		for (int j = 0; j < B->len && i + j < LEN; j++) {
			r = R->data[i + j] + A->data[i] * B->data[j];
			carry = r / 256;
			R->data[i + j] = r - carry * 256;
			for (int k = 1; k < R->len - i - j; k++) {
				r = R->data[i + j + k] + carry;
				if (r < 256) {
					R->data[i + j + k] = r;
					break;
				}
				else {
					carry = r / 256;
					R->data[i + j + k] = r - carry * 256;
				}
			}
		}
	}
	return R;
}
_u8 *Mul(_u8* A, unsigned char B){
	unsigned char carry = 0;
	unsigned int temp;
	int i;
	_u8 *C = Ini();
	for (i = 0; i < A->len; i++){
		temp = A->data[i] * B + carry;
		carry = temp / 256;
		C->data[i] = temp % 256;
	}
	if (carry != 0)
		C->data[i] = carry;
	return C;
}
factor *RightMove(_u8 *A){
	int i = 0, j = 0, temp;
	int k = 0, r;
	char copy;
	while (A->data[i++] == 0)
		;
	i--;
	temp = A->data[i];
	while (!(temp & BitTest[j++]))
		;
	j--;
	_u8 *R = Ini();
	r = 8 * i + j;
	for (; i < A->len - 1; i++){
		copy = (A->data[i] >> j) + (A->data[i + 1] << (8 - j));
		R->data[k++] = copy;
	}
	R->data[k] = A->data[i] >> j;
	factor *Rustle = (factor *)malloc(sizeof(factor));
	Rustle->t = r;
	Rustle->u = R;
	return Rustle;
}
_u8 *ModExp(_u8 *a, _u8 *b, _u8 *n) {
	_u8 *d = Ini();
	_u8 *Temp;
	int i, N = LEN-1;
	d->data[0] = 1;
	while (b->data[N] == 0)
		N--;
	for (i = N; i >=0; i--) {
		for (int j = 7; j >= 0; j--) {
			Temp = Multiplicate(d, d);
			destroy(d);
			d = Mod(Temp, n);
			destroy(Temp);
			if (b->data[i] & BitTest[j]) {
				Temp = Multiplicate(d, a);
				destroy(d);
				d = Mod(Temp, n);
				destroy(Temp);
			}
		}
	}
	return d;
}

_u8 **ExEuclid(_u8 *e, _u8 *fai) {
	_u8 *Temp1, *Temp2;
	_u8 **Rustle;
	if (EqualZero(fai)) {
		Rustle = (_u8 **)malloc(2 * sizeof(_u8 *));
		Rustle[0] = Ini();
		Rustle[0]->data[0] = 1;
		Rustle[1] = Ini();
		Rustle[1]->data[0] = 0;
		return Rustle;
	}
	else {
		Temp1 = Mod(e, fai);
		Rustle = ExEuclid(fai, Temp1);
		destroy(Temp1);
		Temp1 = Div(e, fai);
		Temp2 = Multiplicate(Temp1, Rustle[1]);
		destroy(Temp1);
		Temp1 = Sub(Rustle[0], Temp2);
		destroy(Temp2);
		Rustle[0] = Rustle[1];
		Rustle[1] = Temp1;
		return Rustle;
	}
}
void CreatePublic() {
	PublicKey = (_u8 **)malloc(2 * sizeof(_u8 *));
	PublicKey[0] = Ini();
	PublicKey[0]->data[0] = 1;
	PublicKey[0]->data[1] = 0;
	PublicKey[0]->data[2] = 1;
	PublicKey[1] = Multiplicate(P, Q);
}
void CreatePrivate() {
	PrivateKey = (_u8 **)malloc(2 * sizeof(_u8 *));
	PrivateKey[0] = Ini();
	PrivateKey[0] = ExEuclid(PublicKey[0], Fai())[0];
	if (PrivateKey[0]->sign) {
		_u8 *Temp = Add(PrivateKey[0], fai);
		destroy(PrivateKey[0]);
		PrivateKey[0] = Temp;
	}
	PrivateKey[1] = PublicKey[1];
}
_u8 *Fai() {
	_u8 *Temp = Ini();
	_u8 *Temp2;
	Temp2 = P;
	Temp->data[0] = 1;
	P = Sub(Temp2, Temp);

	Temp2 = Q;
	Q = Sub(Temp2, Temp);
	destroy(Temp);
	fai = Multiplicate(P, Q);
	return fai;
}
void Destory() {
	destroy(P);
	destroy(Q);
	destroy(PublicKey[0]);
	destroy(PublicKey[1]);
	free(PublicKey);
	destroy(PrivateKey[0]);
	free(PrivateKey);
}
_u8 *PM(char *m) {
	_u8 *M = Ini();
	for (int i = 0; i < strlen(m); i++) {
		M->data[i] = m[i];
	}
	_u8 *pm = ModExp(M, PublicKey[0], PublicKey[1]);
	return pm;
}
char *SC(_u8 *c) {
	_u8 *sc = ModExp(c, PrivateKey[0], PrivateKey[1]);
	char *SC = (char *)malloc(LEN * sizeof(char));
	for (int i = 0; i < LEN; i++) {
		SC[i] = sc->data[i];
	}
	return SC;
}
