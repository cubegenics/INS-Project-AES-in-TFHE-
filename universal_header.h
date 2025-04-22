#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <aes-master/structures.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <bits/stdc++.h>
#include <tfhe/polynomials.h>     // TorusPolynomial
#include <tfhe/tlwe.h>            // TLweParams, TLweKey
#include <tfhe/tgsw.h>            // TGswParams, TGswKey, TGswSample
#include <tfhe/lwesamples.h>  
#include <tfhe/tfhe_core.h>                                    // core typedefs (Torus32, EXPORT, struct declarations) :contentReference[oaicite:0]{index=0}
#include <tfhe/tfhe_gate_bootstrapping_structures.h>           // TFheGateBootstrappingParameterSet, TFheGateBootstrappingSecretKeySet :contentReference[oaicite:1]{index=1}
#include <tfhe/lwebootstrappingkey.h> 

using namespace std;

pair<Torus32, Torus32> charToTorus(unsigned char c);

extern TorusPolynomial *lut_xor_lsb;
extern TorusPolynomial *lut_sbox_lsb;
extern TorusPolynomial *lut_mul2_lsb;
extern TorusPolynomial *lut_mul3_lsb;
extern TorusPolynomial *lut_mul9_lsb;
extern TorusPolynomial *lut_mulb_lsb;
extern TorusPolynomial *lut_muld_lsb;
extern TorusPolynomial *lut_mule_lsb;
extern TorusPolynomial *lut_xor_msb;
extern TorusPolynomial *lut_sbox_msb;
extern TorusPolynomial *lut_mul2_msb;
extern TorusPolynomial *lut_mul3_msb;
extern TorusPolynomial *lut_mul9_msb;
extern TorusPolynomial *lut_mulb_msb;
extern TorusPolynomial *lut_muld_msb;
extern TorusPolynomial *lut_mule_msb;

void LUT(
    LweSample *r0,
    LweSample *r1,
    const LweSample *c0,
    const LweSample *c1,
    const TorusPolynomial *lut0,
    const TorusPolynomial *lut1,
    LweBootstrappingKey *bk
);

void init();
void initCompute();

void computeTables();

extern TFheGateBootstrappingParameterSet* params;    
extern TFheGateBootstrappingSecretKeySet* secret_key;
extern LweBootstrappingKey* bk;
extern const LweParams* lwe_params;