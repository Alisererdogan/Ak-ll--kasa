/*
 * Akilli Kasa - Para Ustu Hesaplama
 *
 * Urun tutari ve odenen parayi float olarak alip, para ustunu kurus cinsinden
 * tamsayiyla (int) hesaplar. En az sayida banknot ve madeni para verir.
 *
 * IEEE 754 float hassasiyet kaybi (ornek: 18.70 TL) nedeniyle dogrudan cast
 * yerine math.h icindeki roundf() fonksiyonu kullanilmistir.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define KUPUR_SAYISI 12

// Buyukten kucuge kupur degerleri (kurus cinsinden)
static const int KUPURLER[KUPUR_SAYISI] = {
    20000, // 200 TL
    10000, // 100 TL
     5000, //  50 TL
     2000, //  20 TL
     1000, //  10 TL
      500, //   5 TL
      100, //   1 TL
       50, //  50 Kurus
       25, //  25 Kurus
       10, //  10 Kurus
        5, //   5 Kurus
        1  //   1 Kurus
};

static const char *KUPUR_ISIMLERI[KUPUR_SAYISI] = {
    "200 TL Banknot",
    "100 TL Banknot",
     "50 TL Banknot",
     "20 TL Banknot",
     "10 TL Banknot",
      "5 TL Banknot",
      "1 TL Madeni Para",
     "50 Kurus",
     "25 Kurus",
     "10 Kurus",
      "5 Kurus",
      "1 Kurus"
};

// En az sayida kupur dagilimini hesaplayip yazdirir
int kupur_dagilimi_yazdir(int toplam_kurus) {
    if (toplam_kurus == 0) {
        printf("\nPara ustu yoktur (Tam odeme yapildi).\n");
        printf("Toplam Kupur Adedi: 0\n");
        return 0;
    }

    int kalan = toplam_kurus;
    int toplam_kupur = 0;

    printf("\nVerilecek Para Ustu:\n");
    printf("----------------------------------------\n");

    for (int i = 0; i < KUPUR_SAYISI; i++) {
        int adet = kalan / KUPURLER[i];
        if (adet > 0) {
            printf("%-18s : %2d adet (%.2f TL)\n",
                   KUPUR_ISIMLERI[i],
                   adet,
                   (adet * KUPURLER[i]) / 100.0);
            toplam_kupur += adet;
            kalan %= KUPURLER[i];
        }
    }

    printf("----------------------------------------\n");
    printf("TOPLAM VERILEN KUPUR SAYISI: %d adet\n", toplam_kupur);

    return toplam_kupur;
}

int main(void) {
    float urun_tutari = 0.0f;
    float odenen_para = 0.0f;

    printf("=== Akilli Kasa Sistemi ===\n");

    printf("Urun tutarini giriniz (TL): ");
    if (scanf("%f", &urun_tutari) != 1) {
        printf("HATA: Gecersiz urun tutari girildi!\n");
        return 1;
    }

    printf("Odenen parayi giriniz (TL): ");
    if (scanf("%f", &odenen_para) != 1) {
        printf("HATA: Gecersiz odenen para girildi!\n");
        return 1;
    }

    if (urun_tutari < 0.0f || odenen_para < 0.0f) {
        printf("HATA: Tutarlar negatif olamaz!\n");
        return 1;
    }

    if (odenen_para < urun_tutari) {
        printf("HATA: Odenen para (%.2f TL), urun tutarindan (%.2f TL) az olamaz!\n",
               odenen_para, urun_tutari);
        return 1;
    }

    float fark = odenen_para - urun_tutari;

    // (int)(fark * 100) yapilirsa, ornegin 18.70 icin float 1.299999... olarak saklandigindan
    // 129 kurus (1 kurus eksik) cikar. roundf() ile dogru sekilde en yakin tamsayiya yuvarliyoruz.
    int hatali_kurus = (int)(fark * 100.0f);
    int dogru_kurus  = (int)roundf(fark * 100.0f);

    printf("\nPara Ustu: %.2f TL (%d Kurus)\n", fark, dogru_kurus);

    // Eger float kesme hatasi olusuyorsa bunu belirtelim
    if (hatali_kurus != dogru_kurus) {
        printf("[Not: Float hassasiyeti nedeniyle (int)(fark*100) = %d kr uretirdi. roundf() ile %d kr olarak duzeltildi]\n",
               hatali_kurus, dogru_kurus);
    }

    kupur_dagilimi_yazdir(dogru_kurus);

    return 0;
}
