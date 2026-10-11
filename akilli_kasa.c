/**
 * @file akilli_kasa.c
 * @brief Akilli Kasa - Minimum Banknot ve Madeni Para Ustu Hesaplama Programi
 * @author Ali (Ogrenci Projesi)
 * @date 2026-10-11
 *
 * Bu program:
 * 1. Urun tutari ve odenen parayi float turunde alir.
 * 2. Para ustu hesabini kurus cinsinden tam sayilarla (int) yapar.
 * 3. En az sayida kupur (banknot ve madeni para) dagilimini hesaplar.
 * 4. Toplam verilen kupur sayisini ekrana yazdirir.
 * 5. IEEE 754 float hassasiyet sorununu (18.70 TL - 20.00 TL ornegi)
 *    ve dogru yuvarlama (roundf) yontemini aciklar ve uygular.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define KUPUR_SAYISI 12

/* Kupur degerleri kurus cinsinden tanimlanmistir (Buyukten kucuge) */
static const int KUPURLER[KUPUR_SAYISI] = {
    20000, /* 200 TL Banknot     = 20000 Kurus */
    10000, /* 100 TL Banknot     = 10000 Kurus */
     5000, /*  50 TL Banknot     =  5000 Kurus */
     2000, /*  20 TL Banknot     =  2000 Kurus */
     1000, /*  10 TL Banknot     =  1000 Kurus */
      500, /*   5 TL Banknot     =   500 Kurus */
      100, /*   1 TL Madeni Para =   100 Kurus */
       50, /*  50 Kurus          =    50 Kurus */
       25, /*  25 Kurus          =    25 Kurus */
       10, /*  10 Kurus          =    10 Kurus */
        5, /*   5 Kurus          =     5 Kurus */
        1  /*   1 Kurus          =     1 Kurus */
};

/* Kupurlerin aciklayici isimleri */
static const char *KUPUR_ISIMLERI[KUPUR_SAYISI] = {
    "200 TL Banknot",
    "100 TL Banknot",
     "50 TL Banknot",
     "20 TL Banknot",
     "10 TL Banknot",
      "5 TL Banknot",
      "1 TL Madeni Para",
     "50 Kurus Madeni Para",
     "25 Kurus Madeni Para",
     "10 Kurus Madeni Para",
      "5 Kurus Madeni Para",
      "1 Kurus Madeni Para"
};

/**
 * @brief Kupur dagilimini ekrana yazdirir ve toplam kupur adedini dondurur.
 * @param toplam_kurus Hesaplanacak toplam kurus degeri
 * @return int Toplam verilen kupur sayisi
 */
int kupur_dagilimi_yazdir(int toplam_kurus) {
    int kalan = toplam_kurus;
    int toplam_kupur_sayisi = 0;

    printf("\n==========================================================\n");
    printf("        VERILECEK EN AZ SAYIDA KUPUR LISTESI\n");
    printf("==========================================================\n");

    if (toplam_kurus == 0) {
        printf("  Para ustu yoktur (Tam odeme yapilmistir).\n");
        printf("==========================================================\n");
        printf("  Toplam Kupur Adedi: 0\n");
        printf("==========================================================\n");
        return 0;
    }

    for (int i = 0; i < KUPUR_SAYISI; i++) {
        int adet = kalan / KUPURLER[i];
        if (adet > 0) {
            printf("  %-25s : %2d adet  (%6.2f TL)\n",
                   KUPUR_ISIMLERI[i],
                   adet,
                   (adet * KUPURLER[i]) / 100.0);
            toplam_kupur_sayisi += adet;
            kalan %= KUPURLER[i];
        }
    }

    printf("----------------------------------------------------------\n");
    printf("  TOPLAM VERILEN KUPUR SAYISI: %d adet\n", toplam_kupur_sayisi);
    printf("==========================================================\n");

    return toplam_kupur_sayisi;
}

int main(void) {
    /* Kural 1: Urun tutari ve odenen para float turunde tutulur */
    float urun_tutari = 0.0f;
    float odenen_para = 0.0f;

    printf("==========================================================\n");
    printf("               AKILLI KASA PARA USTU SISTEMI              \n");
    printf("==========================================================\n");

    /* Kullanicidan verilerin alinmasi */
    printf("Urun tutarini giriniz (TL cinsinden, ornek: 18.70): ");
    if (scanf("%f", &urun_tutari) != 1) {
        printf("HATA: Gecersiz urun tutari girildi!\n");
        return 1;
    }

    printf("Musterinin odedigi parayi giriniz (TL, ornek: 20.00): ");
    if (scanf("%f", &odenen_para) != 1) {
        printf("HATA: Gecersiz odenen para girildi!\n");
        return 1;
    }

    /* Girdi dogrulama: Tutarlar negatif olamaz */
    if (urun_tutari < 0.0f || odenen_para < 0.0f) {
        printf("HATA: Tutarlar negatif olamaz!\n");
        return 1;
    }

    /* Kural: Odenen para, urun tutarina esit ya da buyuk olmalidir */
    if (odenen_para < urun_tutari) {
        printf("HATA: Odenen para (%.2f TL), urun tutarindan (%.2f TL) az olamaz!\n",
               odenen_para, urun_tutari);
        return 1;
    }

    /* Para ustu float cinsinden hesaplanir */
    float para_ustu_float = odenen_para - urun_tutari;

    /*
     * Kural 3 (Rapor Sorusu Incelemesi):
     * 1. Yuvarlama yapilmadan dogrudan tam sayiya cevirme:
     *    int hatali_kurus = (int)(para_ustu_float * 100.0f);
     * 2. Dogru matematiksel yuvarlama:
     *    int dogru_kurus = (int)roundf(para_ustu_float * 100.0f);
     */
    int hatali_kurus = (int)(para_ustu_float * 100.0f);
    int dogru_kurus  = (int)roundf(para_ustu_float * 100.0f);

    printf("\n----------------------------------------------------------\n");
    printf("                      HESAPLAMA RAPORU                    \n");
    printf("----------------------------------------------------------\n");
    printf("Urun Tutari        : %.2f TL\n", urun_tutari);
    printf("Odenen Para        : %.2f TL\n", odenen_para);
    printf("Para Ustu (Float)  : %.6f TL\n", para_ustu_float);
    printf("Para Ustu x 100    : %.6f\n", para_ustu_float * 100.0f);
    printf("----------------------------------------------------------\n");
    printf("[1] Yuvarlamasiz Donusum (int)(fark * 100) : %d Kurus (%.2f TL)\n",
           hatali_kurus, hatali_kurus / 100.0);
    printf("[2] Dogru Yuvarlama   (int)roundf(fark*100): %d Kurus (%.2f TL)\n",
           dogru_kurus, dogru_kurus / 100.0);

    /* Float hassasiyet farki varsa kullaniciya aciklama goster */
    if (hatali_kurus != dogru_kurus) {
        printf("\n>>> DIKKAT (IEEE 754 Hassasiyet Sorunu Tespit Edildi!):\n");
        printf("    Ondalikli sayilar ikili (binary) sistemde saklanirken\n");
        printf("    18.70f sayisi tam temsil edilemez (devirli ikili kesir).\n");
        printf("    Fark register'da 1.299999... olarak saklanir.\n");
        printf("    100 ile carpilip (int) cast yapildiginda 129.9999 sayisinin\n");
        printf("    ondalik kismi kirpilir (truncation) ve 129 Kurus elde edilir!\n");
        printf("    Bu durumda musteriye 1 Kurus EKSIK para ustu verilmis olur.\n");
        printf("    Cozum olarak roundf() ile en yakin tam sayiya (%d) yuvarlandi.\n",
               dogru_kurus);
    }

    /* Kural 1 & 2: Kupur hesabi kurus tam sayisi ile yapilir ve toplam yazdirilir */
    kupur_dagilimi_yazdir(dogru_kurus);

    return 0;
}
