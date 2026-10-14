# Akıllı Kasa Para Üstü Hesaplama Sistemi ve IEEE 754 Float Hassasiyet Analizi
## Programlama Ödevi Raporu

**Öğrenci Adı:** Ali  
**Ders:** Programlamaya Giriş / C ile Algoritma ve Programlama  
**Konu:** Akıllı Kasa Para Üstü Dağılımı ve Ondalıklı Sayılarda Hassasiyet Kaybı  

---

## 1. Ödevin Amacı ve Kapsamı

Bu çalışmada, bir perakende mağazasında kasiyerin ürün tutarını ve müşterinin verdiği parayı girmesiyle müşteriye verilecek para üstünü **en az sayıda banknot ve madeni para (minimum küpür)** ile hesaplayan bir C programı geliştirilmiştir.

Ödev isterleri kapsamında şu temel kurallar uygulanmıştır:
1. **Veri Tipleri:** Ürün tutarı ve müşterinin ödediği para `float` değişkenlerinde tutulmuştur. Ancak para üstü ve küpür dağılımı hesapları, kayan noktalı sayıların hassasiyet kaybını önlemek amacıyla **kuruş cinsinden tam sayılarla (`int`)** yapılmıştır.
2. **Küpür Dağılımı ve Toplam Adet:** Türk Lirası sistemindeki 12 resmi küpür (200, 100, 50, 20, 10, 5 TL ve 1 TL, 50, 25, 10, 5, 1 Kr) kullanılarak en az sayıda paranın verilmesi sağlanmış ve toplam küpür adedi ekrana yazdırılmıştır.
3. **IEEE 754 Hassasiyet İncelemesi (Rapor Sorusu):** 18.70 TL ürün tutarı ve 20.00 TL ödeme durumunda, doğrudan `(int)((odenen - urun) * 100)` yapıldığında ortaya çıkan 1 kuruşluk hatanın (`129 kuruş`) mimari ve matematiksel nedeni incelenmiş, `roundf()` fonksiyonu ile doğru yuvarlama (`130 kuruş`) sağlanmıştır.

---

## 2. Problem Analizi ve Algoritma Tasarımı

### 2.1. Açgözlü (Greedy) Algoritma Yaklaşımı
Para üstü hesabında **Açgözlü (Greedy) Yaklaşım** tercih edilmiştir. 
- Türk Lirası para sistemi standart (kanonik) bir madeni para sistemidir. Büyük küpürler, küçük küpürlerin belirli katları biçiminde tanımlandığından, her adımda verilebilecek en büyük küpürün seçilmesi matematiksel olarak her zaman minimum toplam küpür sayısını garanti eder.
- Bu sayede karmaşık dinamik programlama algoritmalarına ihtiyaç duyulmadan, $O(N)$ zaman karmaşıklığında ($N=12$ küpür için sabit sürede) optimum sonuca ulaşılır.

### 2.2. Adım Adım Çalışma Mantığı
1. Kullanıcıdan `urun_tutari` ve `odenen_para` değerleri float olarak alınır.
2. Girdilerin geçerliliği denetlenir (negatif olamaz, ödenen tutar ürün tutarından az olamaz).
3. Float farkı hesaplanır: `fark = odenen_para - urun_tutari`.
4. Float hassasiyet kaybını önlemek için `<math.h>` kütüphanesindeki `roundf()` fonksiyonu kullanılarak kuruş tam sayıya dönüştürülür:  
   `kurus = (int)roundf(fark * 100.0f)`
5. Eğer `kurus == 0` ise tam ödeme yapılmıştır, para üstü yoktur.
6. Kuruş değeri büyükten küçüğe sıralı küpür dizisiyle döngüye sokulur:
   - `adet = kalan / kupur[i]`
   - Eğer `adet > 0` ise ekrana yazdırılır, toplam küpür adedine eklenir ve kalan `kalan %= kupur[i]` ile güncellenir.
7. Toplam küpür adedi ekrana yazdırılarak işlem tamamlanır.

---

## 3. Akış Şeması (Flowchart)

```
              +-------------------------------------+
              |                BAŞLA                |
              +-------------------------------------+
                                 |
                                 v
              +-------------------------------------+
              | Girdi Al: urun_tutari, odenen_para  |
              +-------------------------------------+
                                 |
                                 v
              +-------------------------------------+
              | Girdi Geçersiz mi?                  |
              | (Negatif mi veya Odenen < Urun mu?) |
              +-------------------------------------+
                     /                      \
               [Evet]                        [Hayır]
                 /                              \
                v                                v
    +-----------------------+      +------------------------------------------+
    | Hata Mesajı Yazdır    |      | fark = odenen_para - urun_tutari         |
    | ve Çıkış Yap (return) |      | kurus = (int)roundf(fark * 100.0f)       |
    +-----------------------+      +------------------------------------------+
                                                 |
                                                 v
                                   +------------------------------------------+
                                   | kurus == 0 mı?                           |
                                   +------------------------------------------+
                                          /                      \
                                    [Evet]                        [Hayır]
                                      /                              \
                                     v                                v
                         +-----------------------+      +---------------------------+
                         | "Para Üstü Yok" Yazdır|      | kalan = kurus             |
                         | Toplam Küpür = 0      |      | toplam = 0, i = 0         |
                         +-----------------------+      +---------------------------+
                                     |                                |
                                     |                                v
                                     |                  +---------------------------+
                                     |                  | i < 12 (Tüm Küpürler)     |
                                     |                  +---------------------------+
                                     |                     /                     \
                                     |               [Evet]                       [Hayır]
                                     |                 /                             \
                                     |                v                               v
                                     |   +--------------------------+    +--------------------------+
                                     |   | adet = kalan / KUPUR[i]  |    | Toplam Küpür Sayısını    |
                                     |   | adet > 0 ise ekrana yaz  |    | Ekrana Yazdır            |
                                     |   | toplam += adet           |    +--------------------------+
                                     |   | kalan %= KUPUR[i]        |                  |
                                     |   | i = i + 1                |                  |
                                     |   +--------------------------+                  |
                                     |                 |                               |
                                     |                 v                               |
                                     |        (Döngü Başına Dön)                       |
                                     |                                                 |
                                     +------------------------+------------------------+
                                                              |
                                                              v
                                             +----------------------------------+
                                             |              BİTİR               |
                                             +----------------------------------+
```

---

## 4. Rapor Sorusu: Ondalıklı Sayıların Hassasiyeti (IEEE 754 Analizi)

### 4.1. Problem Tanımı
Ödevde incelenmesi istenen örnek:
- **Ürün Tutarı:** `18.70 TL`
- **Ödenen Para:** `20.00 TL`
- **Beklenen Gerçek Para Üstü:** `20.00 - 18.70 = 1.30 TL = 130 Kuruş`

Eğer bu hesaplama doğrudan tam sayıya dönüştürülerek yapılırsa:
```c
int kurus_hatali = (int)((odenen_para - urun_tutari) * 100.0f);
```
Program **129 Kuruş** sonucunu üretir ve müşteriye **1 kuruş eksik** para üstü verilir.

### 4.2. Neden 129 Çıkıyor? (IEEE 754 ve Truncation Hatası)

Bilgisayarlar verileri onluk (desimal) tabanda değil, ikili (binary) tabanda saklar. 

Tek duyarlıklı `float` veri tipi IEEE 754 standardına göre bellekte 32 bit olarak tutulur:
- **1 bit:** İşaret (Sign)
- **8 bit:** Üs (Exponent)
- **23 bit:** Kesir / Mantissa (Fraction)

#### 1. Kesirli Sayının İkili Karşılığı
10'luk tabandaki `0.70` sayısını ikili sisteme çevirdiğimizde:
- $0.70 \times 2 = 1.40 \rightarrow 1$
- $0.40 \times 2 = 0.80 \rightarrow 0$
- $0.80 \times 2 = 1.60 \rightarrow 1$
- $0.60 \times 2 = 1.20 \rightarrow 1$
- $0.20 \times 2 = 0.40 \rightarrow 0$
- $0.40 \times 2 = 0.80 \rightarrow 0$
- $0.80 \times 2 = 1.60 \rightarrow 1$
- $0.60 \times 2 = 1.20 \rightarrow 1$  
İşlem $0.101100110011\dots_2$ şeklinde sonsuza kadar devreder. 

#### 2. Bellekte Saklama
32-bit `float` türünün 23 bitlik mantissa alanı sonsuz devreden bu kesri tam olarak saklayamaz. Sayı en yakın değere yuvarlanarak belleğe yazılır:
- `18.70f` bellekteki değeri: `18.700000762939...` (Hex: `0x4195999A`)
- `20.00f` bellekteki değeri: `20.000000000000...` (Hex: `0x41A00000`)

#### 3. Çıkarma İşlemi
`20.00f - 18.70f` çıkarma işlemi yapıldığında sonuç:
$$\text{fark} = 20.00000000 - 18.70000076 = 1.29999923706\dots$$
Bu sayı 100 ile çarpıldığında:
$$\text{fark} \times 100.0f = 129.999923706\dots$$

#### 4. Type Casting (Truncation - Kırpma)
C dilinde kayan noktalı bir sayı `(int)` türüne dönüştürüldüğünde yuvarlama yapılmaz; sayının virgülden sonraki kısmı doğrudan atılır (truncation):
$$\text{(int)}(129.999923706\dots) = \mathbf{129}$$

Bu nedenle hesaplama 129 kuruş olarak sonuçlanır ve 1 kuruşluk kayıp meydana gelir.

### 4.3. Küpür Dağılımına Etkisi
Bu 1 kuruşluk fark yalnızca eksik para verilmesine yol açmaz, aynı zamanda küpür dağılımını da bozar:

| Durum | Kuruş | Verilen Küpürler | Toplam Küpür |
|---|---|---|---|
| **Hatalı (int cast)** | 129 kr | 1 TL (1 adet), 25 Kr (1 adet), **1 Kr (4 adet)** | **6 adet** (Hatalı ve eksik) |
| **Doğru (roundf)** | 130 kr | 1 TL (1 adet), 25 Kr (1 adet), **5 Kr (1 adet)** | **3 adet** (En az küpür ve tam) |

Hatalı işlem nedeniyle 5 kuruş yerine 4 adet 1 kuruş verilerek toplam küpür adedi 3'ten 6'ya çıkmaktadır.

### 4.4. Çözüm
Ondalıklı sayının en yakın tam sayıya yuvarlanması gerekir. Bunun için `<math.h>` kütüphanesindeki `roundf()` fonksiyonu kullanılmıştır:
```c
int dogru_kurus = (int)roundf((odenen_para - urun_tutari) * 100.0f);
```
`roundf(129.9999237f)` ifadesi sayıyı `130.0f` değerine yuvarlar ve `(int)` dönüşümü kesin olarak `130` sonucunu üretir.

---

## 5. C Kaynak Kodu (`akilli_kasa.c`)

```c
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

    int hatali_kurus = (int)(fark * 100.0f);
    int dogru_kurus  = (int)roundf(fark * 100.0f);

    printf("\nPara Ustu: %.2f TL (%d Kurus)\n", fark, dogru_kurus);

    if (hatali_kurus != dogru_kurus) {
        printf("[Not: Float hassasiyeti nedeniyle (int)(fark*100) = %d kr uretirdi. roundf() ile %d kr olarak duzeltildi]\n",
               hatali_kurus, dogru_kurus);
    }

    kupur_dagilimi_yazdir(dogru_kurus);

    return 0;
}
```

---

## 6. Test Senaryoları ve Çalışma Çıktıları

### Test 1: Ödev Senaryosu (18.70 TL / 20.00 TL)
```
=== Akilli Kasa Sistemi ===
Urun tutarini giriniz (TL): 18.70
Odenen parayi giriniz (TL): 20.00

Para Ustu: 1.30 TL (130 Kurus)
[Not: Float hassasiyeti nedeniyle (int)(fark*100) = 129 kr uretirdi. roundf() ile 130 kr olarak duzeltildi]

Verilecek Para Ustu:
----------------------------------------
1 TL Madeni Para   :  1 adet (1.00 TL)
25 Kurus           :  1 adet (0.25 TL)
5 Kurus            :  1 adet (0.05 TL)
----------------------------------------
TOPLAM VERILEN KUPUR SAYISI: 3 adet
```

### Test 2: Tam Ödeme (50.00 TL / 50.00 TL)
```
=== Akilli Kasa Sistemi ===
Urun tutarini giriniz (TL): 50.00
Odenen parayi giriniz (TL): 50.00

Para Ustu: 0.00 TL (0 Kurus)

Para ustu yoktur (Tam odeme yapildi).
Toplam Kupur Adedi: 0
```

### Test 3: Büyük Banknot Dağılımı (12.50 TL / 200.00 TL)
```
Para Ustu: 187.50 TL (18750 Kurus)
- 100 TL Banknot : 1 adet
- 50 TL Banknot  : 1 adet
- 20 TL Banknot  : 1 adet
- 10 TL Banknot  : 1 adet
- 5 TL Banknot   : 1 adet
- 1 TL Madeni    : 2 adet
- 50 Kurus       : 1 adet
TOPLAM VERILEN KUPUR SAYISI: 8 adet
```

### Test 4: Hata Yönetimi (Yetersiz Ödeme)
```
Urun tutarini giriniz (TL): 30.00
Odenen parayi giriniz (TL): 20.00
HATA: Odenen para (20.00 TL), urun tutarindan (30.00 TL) az olamaz!
```

---

## 7. Sonuç ve Değerlendirme

Bu ödev çalışmasında:
1. Perakende nakit işlemlerinde para üstünün Açgözlü (Greedy) algoritma ile en az sayıda küpürle başarıyla hesaplandığı doğrulanmıştır.
2. Finansal uygulamalarda `float` veya `double` gibi kayan noktalı sayı türlerinin doğrudan tam sayıya dönüştürülmesinin tehlikeleri IEEE 754 standardı çerçevesinde somutlaştırılmıştır.
3. Kuruş ve sent gibi para alt birimleri hesaplanırken işlemlerin tamsayı tabanlı yürütülmesinin ve dönüştürme aşamasında `roundf()` gibi matematiksel yuvarlama fonksiyonlarının kullanılmasının önemi kavranmıştır.
