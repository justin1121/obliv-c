#ifndef NNOB_H
#define NNOB_H

#define BUCKET_SIZE 4
#define OTS_PER_aOT 4
#define OTS_PER_aAND 3 
#define OTS_FOR_R 1 
#define OTS_FOR_INPUTS 1 
#define NNOB_HASH_ALGO GCRY_CIPHER_AES192
#define NNOB_HASH_ALGO_KEYBYTES 24
//#define ALL_OTS (2*(OTS_PER_aOT+OTS_PER_aAND+OTS_FOR_R+OTS_FOR_INPUTS)) // 2* because both sides need 1
#define A_BIT_PARAMETER_BYTES (A_BIT_MULTIPLIER*NNOB_KEY_BYTES) // NNOB_KEY_BYTES = 2*k
#define A_BIT_MULTIPLIER 8 

typedef enum { OTExtValidation_hhash, OTExtValidation_byPair } OTExtValidation;
typedef enum {ShareAndMac=true, Key=false} NnobHalfBitType;

typedef struct {
	struct timespec wallclockTime;
	clock_t cpuclockTime;
} time_struct;

typedef struct {
	NnobHalfBitType type;
	union {
		struct {
			bool value;
			nnob_key_t mac;
		} ShareAndMac;
		nnob_key_t key;
	};
} NnobHalfBit;

typedef struct AOTShareAndMacOfZ {
	NnobKey* x0;
	NnobKey* x1;
	NnobShareAndMac* c;
	NnobShareAndMac* z;
	int counter;
	int n;
} AOTShareAndMacOfZ;

typedef struct AOTKeyOfZ {
	NnobShareAndMac* x0;
	NnobShareAndMac* x1;
	NnobKey* c;
	NnobKey* z;
	int counter;
	int n;
} AOTKeyOfZ;

typedef struct AANDShareAndMac{
	NnobShareAndMac* x;
	NnobShareAndMac* y;
	NnobShareAndMac* z;
	int counter;
	int n;
} AANDShareAndMac;

typedef struct AANDKey{
	NnobKey* x;
	NnobKey* y;
	NnobKey* z;
	int counter;
	int n;
} AANDKey;

typedef struct NnobProtocolDesc
{
	bool error;
        int bucketSize;
	nnob_key_t globalDelta;
	int numANDs;
	struct { // Also used to generate OTs
		bool* share;
		char (*mac)[NNOB_KEY_BYTES]; // equivalent to c bits
		int counter;
		int n;
	} aBitsShareAndMac;
	struct { // Also used to generate OTs
		char (*key)[NNOB_KEY_BYTES]; // equivalent to opt0
		int counter;
		int n;
	} aBitsKey;
	struct {
		AOTShareAndMacOfZ aOTShareAndMacOfZ;			
		AOTKeyOfZ aOTKeyOfZ;
		AANDShareAndMac aANDShareAndMac;
		AANDKey aANDKey;
	} FDeal;
} NnobProtocolDesc;

NnobProtocolDesc* initNnobProtocolDesc(ProtocolDesc* pd, int numOTs, 
		OTExtValidation validation);
void cleanupNnobProtocol(NnobProtocolDesc* npd);
void nnobSendOblivInput(ProtocolDesc* pd, bool* input, OblivBit* oblivInput, int numOblivInput);
void nnobRecvOblivInput(ProtocolDesc* pd, OblivBit* oblivInput, int numOblivInput);

void debugTimer(time_struct* t);
void debugPrintTime(time_struct* begin, time_struct* end, char* name, int party);
bool debugMatchingOblivBit(const OblivBit* x, const nnob_key_t globalDeltaToXKey,
		const OblivBit* y, const nnob_key_t globalDeltaToYKey);
static int* allRows(int n);
void debugPrintOblivBit(const OblivBit* bit);


//void getRandomAOTQuadruple(NnobProtocolDesc* npd, 
		//NnobHalfBit* x0, NnobHalfBit* x1, NnobHalfBit* c, NnobHalfBit* z, NnobHalfBitType zType);
//void getRandomAANDTriple(NnobProtocolDesc* npd, 
		//NnobHalfBit* x, NnobHalfBit* y, NnobHalfBit* z, NnobHalfBitType type);
//void debugGetNnobHalfBit(NnobHalfBit* output, bool bit,const nnob_key_t key, 
		//const nnob_key_t globalDelta, NnobHalfBitType type);
bool debugCheckOT(const NnobHalfBit* x0_1, const NnobHalfBit* x1_1,
		const NnobHalfBit* c_1, const NnobHalfBit* z_1, const nnob_key_t globalDelta_1,
		const NnobHalfBit* x0_2, const NnobHalfBit* x1_2,
		const NnobHalfBit* c_2, const NnobHalfBit* z_2, const nnob_key_t globalDelta_2);
bool debugCheckAND(const NnobHalfBit* x_1, const NnobHalfBit* y_1, 
		const NnobHalfBit* z_1, const nnob_key_t globalDelta_1,
		const NnobHalfBit* x_2, const NnobHalfBit* y_2,
		const NnobHalfBit* z_2, const nnob_key_t globalDelta_2);
void nl();
void print(const char* a);
void printWithlen(const char* a, int len);
void debugPrintHex(const char* a, int len);
void debugPrintXor(const char* a,const  char* b, int len);
void debugPrintXorShowParam(const char* a,const  char* b, int len);
void debugPrintNnobHalfBit(const NnobHalfBit* a);
bool debugMatchingNnobHalfBit(const NnobHalfBit* a, const NnobHalfBit* b, const nnob_key_t globalDelta);
void debugPrintKeyfromShareAndMac(const NnobHalfBit* a, const nnob_key_t globalDelta);
void debugPrintNnobHalfBitExtra(const NnobHalfBit* a, const nnob_key_t globalDelta);

#endif
