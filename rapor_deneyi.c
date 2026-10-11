/**
 * @file rapor_deneyi.c
 * @brief Rapor Sorusu Icin IEEE 754 Float Hassasiyet Deneyi ve Bit Analizi
 * @author Ali (Ogrenci Projesi)
 * @date 2026-10-11
 *
 * Bu program:
 * 18.70 TL urun tutari ve 20.00 TL odenen para ornegini
 * IEEE 754 tek duyarlikli (single-precision 32-bit float) standardinda
 * bit ve hex duzeyinde analiz eder, 1 kurusluk kaybin matematiksel
 * nedenini kanitlar.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Float degerin bellek yapisini bit duzeyinde incelemek icin union */
typedef union {
    float f;
    uint32_t u;
    struct {
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign     : 1;
    } bits;
} FloatBitAnaliz;

void float_yazdir(const char *etiket, float val) {
    FloatBitAnaliz fa;
    fa.f = val;

    printf("%-20s: %16.10f | Hex: 0x%08X | Isaret: %u | Us: %3u | Mantissa: 0x%06X\n",
           etiket,
           (double)val,
           fa.u,
           fa.bits.sign,
           fa.bits.exponent,
           fa.bits.mantissa);
}

int main(void) {
    float urun_tutari = 18.70f;
    float odenen_para = 20.00f;
    float fark = odenen_para - urun_tutari;
    float fark_x100 = fark * 100.0f;

    int hatali_kurus = (int)(fark * 100.0f);
    int dogru_kurus = (int)roundf(fark * 100.0f);

    printf("========================================================================================\n");
    printf("                  RAPOR SORUSU: IEEE 754 FLOAT HASSASIYET ANALIZI                       \n");
    printf("========================================================================================\n\n");

    printf("1. DEGISKENLERIN BELLEKTEKI (IEEE 754) BIT DUZEYINDEKI TEMSILI:\n");
    printf("----------------------------------------------------------------------------------------\n");
    float_yazdir("Urun Tutari (18.70f)", urun_tutari);
    float_yazdir("Odenen Para (20.00f)", odenen_para);
    float_yazdir("Fark (20 - 18.70)", fark);
    float_yazdir("Fark x 100.0f", fark_x100);
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("2. MATEMATIKSEL DONUSUM KARSILASTIRMASI:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  Teorik (Gercek) Para Ustu : 20.00 - 18.70 = 1.30 TL = 130 Kurus\n");
    printf("  Bellekte Saklanan Fark    : %.15f TL\n", (double)fark);
    printf("  Bellekte Fark x 100       : %.15f\n", (double)fark_x100);
    printf("  (int)(fark * 100) [HATALI]: %d Kurus  (Truncation: ondalik kisim atildi, 1 kr KAYIP!)\n",
           hatali_kurus);
    printf("  roundf(fark * 100)[DOGRU] : %d Kurus  (Matematiksel en yakin tamsayiya yuvarlandi)\n",
           dogru_kurus);
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("3. KUPUR ETKISI KARSILASTIRMASI (129 Kurus vs 130 Kurus):\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("  [HATALI] 129 Kurus icin kupurler:\n");
    printf("    -> 1 TL (100 kr) : 1 adet\n");
    printf("    -> 25 Kurus      : 1 adet\n");
    printf("    -> 1 Kurus       : 4 adet\n");
    printf("    TOPLAM: 6 adet kupur verilir ve musteri 1 kurus zarara ugratilir!\n\n");

    printf("  [DOGRU]  130 Kurus icin kupurler:\n");
    printf("    -> 1 TL (100 kr) : 1 adet\n");
    printf("    -> 25 Kurus      : 1 adet\n");
    printf("    -> 5 Kurus       : 1 adet\n");
    printf("    TOPLAM: 3 adet kupur verilir, para ustu tam ve en az sayida kupurle odenir!\n");
    printf("========================================================================================\n");

    return 0;
}
