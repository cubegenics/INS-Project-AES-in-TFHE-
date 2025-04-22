#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <bits/stdc++.h>
using namespace std;
#include <tfhe/polynomials.h>     // TorusPolynomial
#include <tfhe/tlwe.h>            // TLweParams, TLweKey
#include <tfhe/tgsw.h>            // TGswParams, TGswKey, TGswSample
#include <tfhe/lwesamples.h>  
#include <tfhe/tfhe_core.h>                                    // core typedefs (Torus32, EXPORT, struct declarations) :contentReference[oaicite:0]{index=0}
#include <tfhe/tfhe_gate_bootstrapping_structures.h>           // TFheGateBootstrappingParameterSet, TFheGateBootstrappingSecretKeySet :contentReference[oaicite:1]{index=1}
#include <tfhe/lwebootstrappingkey.h> 
#include <aes-master/structures.h>
#include "universal_header.h"

/*
NOTE: the given code assumes the coefficients of the polynomials in TRLWE are 
from Torus32 format. When applying SampleExtract to extract the coefficient, the obtained
result however is a TLWE sample, i.e. the encryption happens behind the scenes.
*/


TorusPolynomial *lut_xor_lsb;
TorusPolynomial *lut_sbox_lsb;
TorusPolynomial *lut_mul2_lsb;
TorusPolynomial *lut_mul3_lsb;
TorusPolynomial *lut_mul9_lsb;
TorusPolynomial *lut_mulb_lsb;
TorusPolynomial *lut_muld_lsb;
TorusPolynomial *lut_mule_lsb;
TorusPolynomial *lut_xor_msb;
TorusPolynomial *lut_sbox_msb;
TorusPolynomial *lut_mul2_msb;
TorusPolynomial *lut_mul3_msb;
TorusPolynomial *lut_mul9_msb;
TorusPolynomial *lut_mulb_msb;
TorusPolynomial *lut_muld_msb;
TorusPolynomial *lut_mule_msb;

void computeTables() {
    //sbox
    //mul_a
    //xor
    lut_xor_lsb = new_TorusPolynomial(256);
    lut_sbox_lsb = new_TorusPolynomial(256);
    lut_mul2_lsb = new_TorusPolynomial(256);
    lut_mul3_lsb = new_TorusPolynomial(256);
    lut_mul9_lsb = new_TorusPolynomial(256);
    lut_mulb_lsb = new_TorusPolynomial(256);
    lut_muld_lsb = new_TorusPolynomial(256);
    lut_mule_lsb = new_TorusPolynomial(256);

    lut_xor_msb = new_TorusPolynomial(256);
    lut_sbox_msb = new_TorusPolynomial(256);
    lut_mul2_msb = new_TorusPolynomial(256);
    lut_mul3_msb = new_TorusPolynomial(256);
    lut_mul9_msb = new_TorusPolynomial(256);
    lut_mulb_msb = new_TorusPolynomial(256);
    lut_muld_msb = new_TorusPolynomial(256);
    lut_mule_msb = new_TorusPolynomial(256);

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = i^j;
            auto torusPair = charToTorus(val);
            lut_xor_lsb->coefsT[j+i*16] = torusPair.first;
            lut_xor_msb->coefsT[j+i*16] = torusPair.second;
        }
    }

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = s[j+i*16];
            auto torusPair = charToTorus(val);
            lut_sbox_lsb->coefsT[j+i*16] = torusPair.first;
            lut_sbox_msb->coefsT[j+i*16] = torusPair.second;
        }
    }

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul2[j+i*16];
            auto torusPair = charToTorus(val);
            lut_mul2_lsb->coefsT[j+i*16] = torusPair.first;
            lut_mul2_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul3[j+i*16];
            auto torusPair = charToTorus(val);
            lut_mul3_lsb->coefsT[j+i*16] = torusPair.first;
            lut_mul3_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul9[j+i*16];
            auto torusPair = charToTorus(val);
            lut_mul9_lsb->coefsT[j+i*16] = torusPair.first;
            lut_mul9_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul11[j+i*16];
            auto torusPair = charToTorus(val);
            lut_mulb_lsb->coefsT[j+i*16] = torusPair.first;
            lut_mulb_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul13[j+i*16];
            auto torusPair = charToTorus(val);
            lut_muld_lsb->coefsT[j+i*16] = torusPair.first;
            lut_muld_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            unsigned char val = mul14[j+i*16];
            auto torusPair = charToTorus(val);
            lut_mule_lsb->coefsT[j+i*16] = torusPair.first;
            lut_mule_msb->coefsT[j+i*16] = torusPair.second;
        }
    }
}

void LUT(
    LweSample *r0,
    LweSample *r1,
    const LweSample *c0,
    const LweSample *c1,
    const TorusPolynomial *lut0,
    const TorusPolynomial *lut1,
    LweBootstrappingKey *bk
)
{
    std::cout << "LUT begin\n";
    /*
    According to note, coefficients of polynomial are from Torus32, but
    after completing step1 of tree based method, extracted coefficients
    are from TLWE. So can't really give this polynomial as input to the
    BlindRotate since it requires a Torus32 polynomial as parameter
    */

    const TGswParams *bk_params = bk->bk_params;
    const TLweParams *accum_params = bk->accum_params;
    const LweParams *in_params = bk->in_out_params;
    const int32_t N = accum_params->N;
    const int32_t Nx2 = 2 * N;
    const int32_t n = in_params->n;

    std::cout << "1\n";

    //level1 of tree based method
    int32_t *bara_c0 = new int32_t[N];

    int32_t barb_c0 = modSwitchFromTorus32(c0->b, Nx2);
    for (int32_t i = 0; i < n; i++) {
        bara_c0[i] = modSwitchFromTorus32(c0->a[i], Nx2);
    }


    std::cout << "2\n";
    
    TorusPolynomial *polynomial_after_step1 = new_TorusPolynomial(256);
    for (int idx = 0; idx < 16; idx++) {
        LweSample *result = new_LweSample(lwe_params);
        tfhe_blindRotateAndExtract(result, lut0, bk->bk, barb_c0, bara_c0, n, bk_params);
        polynomial_after_step1->coefsT[idx] = result->b;
    }

    std::cout << "3\n";

    

    int32_t *bara_c1 = new int32_t[N];

    int32_t barb_c1 = modSwitchFromTorus32(c1->b, Nx2);
    for (int32_t i = 0; i < n; i++) {
        bara_c1[i] = modSwitchFromTorus32(c1->a[i], Nx2);
    }


    std::cout << "4\n";

    // int tempN = bk_params->tlwe_params->N;

    // std::cout << "2*tempN: " << 2 * tempN << '\n';
    // std::cout << "barb: " << barb_c1 << "\n";
    // int tempA = 2 * tempN - barb_c1;
    // int polyN = polynomial_after_step1->N;

    // cout << "polyN: " << polyN << '\n';
    // cout << "tempA: " << tempA << '\n';
    //level2 of tree based method

    //CHANGE
    tfhe_blindRotateAndExtract(r0, polynomial_after_step1, bk->bk, barb_c0, bara_c0, n, bk_params);



    std::cout << "5\n";

    //similarly for MSB
    //level1 of tree based method
    for (int32_t i = 0; i < n; i++) {
        bara_c0[i] = modSwitchFromTorus32(c0->a[i], Nx2);
    }
    
    for (int idx = 0; idx < 16; idx++) {
        LweSample *result = new_LweSample(lwe_params);
        tfhe_blindRotateAndExtract(result, lut1, bk->bk, barb_c0, bara_c0, n, bk_params);
        polynomial_after_step1->coefsT[idx] = result->b;
    }


    for (int32_t i = 0; i < n; i++) {
        bara_c1[i] = modSwitchFromTorus32(c1->a[i], Nx2);
    }

    //level2 of tree based method
    //CHANGE
    tfhe_blindRotateAndExtract(r0, lut1, bk->bk, barb_c0, bara_c0, n, bk_params);

    delete polynomial_after_step1;

    std::cout << "LUT end\n";

    init();
}

// int main() {

//     computeTables();


//     int32_t n = 32;
//     double alpha_min = 0;
//     double alpha_max = 1;
//     LweParams* lweparams = new LweParams(n, alpha_min, alpha_max);
//     LweSample *c0 = new LweSample(lweparams);
//     LweSample *c1 = new LweSample(lweparams);

//     LweSample *r0 = new LweSample(lweparams);
//     LweSample *r1 = new LweSample(lweparams);


//     //begin

//     //end


//     cout << "about to call LUT\n";
//     LUT(
//         r0,
//         r1,
//         c0,
//         c1,
//         lut_xor,
//         lut_xor,
//         bk
//     );
//     return 0;
// }