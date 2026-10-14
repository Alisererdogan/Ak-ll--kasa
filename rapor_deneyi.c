/*
 * rapor_deneyi.c - IEEE 754 Float Hassasiyet Deneyi
 *
 * 18.70 TL urun tutari ve 20.00 TL odenen para orneginde ortaya cikan
 * 1 kurusluk kaybin nedenini bit duzeyinde inceler.
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>

// 32-bit float yapisini (IEEE 754) bit duzeyinde gormek icin union
typedef union {
    float f;
    uint32_t u;
    struct {
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign     : 1;
    } bits;
} FloatBitAnaliz;

void float_yazdir(const char *etiket, float sayi) {
    FloatBitAnaliz fa;
    fa.f = sayi;

    printf("%-20s: %14.8f | Hex: 0x%08X | Isaret:%u  Us:%3u  Mantissa:0x%06X\n",
           etiket,
           (double)sayi,
           fa.u,
           fa.bits.sign,
           fa.bits.exponent,
           fa.bits.mantissa);
}

int main(void) {
    float urun = 18.70f;
    float odenen = 20.00f;
    float fark = odenen - urun;
    float fark_x100 = fark * 100.0f;

    int hatali_kurus = (int)(fark * 100.0f);
    int dogru_kurus = (int)roundf(fark * 100.0f);

    printf("=== IEEE 754 Float Hassasiyet Deneyi ===\n\n");

    printf("1. Bellekteki Temsil (IEEE 754):\n");
    printf("--------------------------------------------------------------------------------\n");
    float_yazdir("Urun (18.70f)", urun);
    float_yazdir("Odenen (20.00f)", odenen);
    float_yazdir("Fark (20 - 18.70)", fark);
    float_yazdir("Fark x 100.0f", fark_x100);
    printf("--------------------------------------------------------------------------------\n\n");

    printf("2. Donusum Karsilastirmasi:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("  Matematiksel Gercek Deger : 20.00 - 18.70 = 1.30 TL (130 Kurus)\n");
    printf("  Bellekte Saklanan Fark    : %.12f TL\n", (double)fark);
    printf("  Fark x 100.0f             : %.12f\n", (double)fark_x100);
    printf("  (int)(fark * 100) [Hatali]: %d Kurus  -> Ondalik kisim kesildi (1 kurus kayip!)\n",
           hatali_kurus);
    printf("  roundf(fark*100)  [Dogru] : %d Kurus  -> En yakin tam sayiya yuvarlandi\n",
           dogru_kurus);
    printf("--------------------------------------------------------------------------------\n\n");

    printf("3. Kupur Dagilimi Uzerindeki Etkisi:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("  [Hatali] 129 Kurus:\n");
    printf("    -> 1 TL (1 adet), 25 Kr (1 adet), 1 Kr (4 adet) = Toplam 6 kupur\n");
    printf("    (Musteri 1 kurus eksik alir ve gereksiz fazla bozukluk verilir)\n\n");
    printf("  [Dogru]  130 Kurus:\n");
    printf("    -> 1 TL (1 adet), 25 Kr (1 adet), 5 Kr (1 adet) = Toplam 3 kupur\n");
    printf("    (Para ustu eksiksizdir ve en az sayida kupurle odenir)\n");
    printf("--------------------------------------------------------------------------------\n");

    return 0;
}
