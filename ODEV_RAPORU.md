# AKILLI KASA PARA ÜSTÜ HESAPLAMA SİSTEMİ VE IEEE 754 FLOAT HASSASİYET ANALİZİ
## BİLGİSAYAR MÜHENDİSLİĞİ / PROGRAMLAMA ÖDEV RAPORU

> **NOT (ÖĞRENCİ İÇİN):** Bu rapor, sınıfta A4 kağıdına **ELLE YAZILARAK** teslim edilmek üzere hazırlanmıştır. Aşağıdaki başlıkları, açıklamaları, akış şemasını ve C kodunu tükenmez/kurşun kalemle A4 kağıdına aktarabilirsiniz. Teams platformuna ise sadece projenin **GitHub linki** yüklenecektir.

---

### ÖĞRENCİ BİLGİLERİ
- **Öğrenci Adı Soyadı:** Ali [Soyadınız]
- **Öğrenci Numarası:** [Öğrenci Numaranız]
- **Bölüm:** Bilgisayar Mühendisliği / Yazılım Mühendisliği
- **Ders Adı:** Programlamaya Giriş / C ile Algoritma ve Programlama
- **Teslim Tarihi:** 15 Ekim 2026 (Perşembe)
- **GitHub Repo Linki:** https://github.com/[KullanıcıAdınız]/akilli-kasa

---

## 1. PROGRAMIN AMACI VE DETAYLARI

### 1.1. Programın Amacı
Bir perakende mağazasında kasiyerin nakit işlemlerinde para üstünü hatasız, hızlı ve müşteriye **en az sayıda banknot ve madeni para (küpür)** vererek teslim etmesini sağlayan akıllı bir kasa algoritması geliştirmektir. 

Finansal yazılımlarda para birimlerinin kuruş ve sent gibi alt basamaklarının doğru hesaplanması kritik bir gereksinimdir. Bu çalışmada ayrıca, bilgisayar mimarisinde kayan noktalı sayıların (floating-point) ikili (binary) tabanda saklanmasından kaynaklanan hassasiyet sınırları ve bu sınırların finansal hesaplamalardaki tehlikeleri somut bir örnek üzerinden incelenmiştir.

### 1.2. Programın Temel Kuralları ve Kapsamı
1. **Veri Tipleri Kuralı:** Ürün tutarı ve müşterinin ödediği para `float` (ondalıklı tek duyarlıklı sayı) türündeki değişkenlerde tutulur. Ancak küpür dağılım hesabı kuruş cinsinden tam sayılarla (`int`) yürütülür.
2. **Küpür Listesi:** Türk Lirası para sistemindeki resmi 12 küpür kullanılmıştır:
   - **Banknotlar (Küpürler):** 200 TL (20000 kr), 100 TL (10000 kr), 50 TL (5000 kr), 20 TL (2000 kr), 10 TL (1000 kr), 5 TL (500 kr).
   - **Madeni Paralar:** 1 TL (100 kr), 50 Kuruş (50 kr), 25 Kuruş (25 kr), 10 Kuruş (10 kr), 5 Kuruş (5 kr), 1 Kuruş (1 kr).
3. **Toplam Küpür Çıktısı:** Müşteriye verilen toplam küpür (banknot + madeni para) adedi ekrana raporlanır.
4. **Hassasiyet ve Yuvarlama Raporu:** Ürün tutarı `18.70 TL` ve ödenen para `20.00 TL` olduğunda ortaya çıkan IEEE 754 duyarlık kaybı incelenmiş, doğrudan tam sayıya dönüştürmenin neden `129 kuruş` (1 kuruş eksik) ürettiği kanıtlanmış ve `roundf()` matematiksel yuvarlama fonksiyonu ile doğru sonuç (`130 kuruş`) elde edilmiştir.

---

## 2. GELİŞTİRME SÜRECİNDE KULLANILAN ARAÇLAR VE YARARLANILMA BİÇİMLERİ

Geliştirme sürecinde modern yazılım mühendisliği standartlarına uygun aşağıdaki araçlardan yararlanılmıştır:

1. **GCC (GNU Compiler Collection - C99/C11 Standardı):**
   - Kodun derlenmesi, statik analiz ve optimizasyonunda kullanılmıştır.
   - `-Wall -Wextra -pedantic -std=c99` derleme bayrakları kullanılarak kodun hiçbir uyarı (warning) üretmeden en katı standartlarda derlenmesi sağlanmıştır.
   - Matematik kütüphanesi fonksiyonları (`roundf`) için `-lm` bağlayıcı bayrağından yararlanılmıştır.

2. **Linux Terminali & Bash Shell:**
   - Programın otomatik test senaryolarını (`test_suite.sh`) yürütmek, sınır durumlarını (boundary conditions) ve uç durumları (edge cases) doğrulamak için komut satırı araçları kullanılmıştır.

3. **GNU Make (Otomasyon Aracı):**
   - Projenin tek komutla derlenmesi (`make all`), çalıştırılması (`make run`), test edilmesi (`make test`) ve bellek analizi deneyinin yapılması (`make rapor`) amacıyla Makefile otomasyonundan faydalanılmıştır.

4. **Git ve GitHub (Versiyon Kontrol Sistemi):**
   - Kaynak kodun aşamalı olarak kaydedilmesi, commit geçmişinin tutulması ve ödev teslim şartı olan açık GitHub reposunun oluşturulması için Git ve GitHub araçları kullanılmıştır.

5. **C Standart Kütüphaneleri:**
   - `stdio.h`: Giriş/çıkış işlemleri (`printf`, `scanf`).
   - `stdlib.h`: Çıkış kodları ve bellek yönetimi.
   - `math.h`: Doğru matematiksel yuvarlama fonksiyonu (`roundf`).
   - `stdint.h`: Bellek bit düzeyinde IEEE 754 analizi için 32-bit tamsayı türleri (`uint32_t`).

---

## 3. ALGORİTMA DETAYLARI VE ADIMLARI

### 3.1. Algoritma Türü: Açgözlü (Greedy) Algoritma
Küpür dağılımında **Açgözlü (Greedy) Yaklaşım** kullanılmıştır.
- **Neden Greedy?:** Türk Lirası para sistemi kanonik (canonical) bir madeni para sistemidir. Yani her büyük küpür, kendisinden küçük olan küpürlerin belirli bir kombinasyonuna tam bölünebilir niteliktedir. Bu özellik sayesinde, her adımda verilebilecek en büyük küpürü tercih etmek matematiksel olarak her zaman **küresel minimum (global optimum)** küpür sayısını garanti eder. Ekstra dinamik programlama maliyetine gerek kalmaz; $O(N)$ zaman karmaşıklığında ($N = 12$) sabit zamanda çalışır.

### 3.2. Adım Adım Algoritma Metni (Pseudocode)
1. **BAŞLA**
2. Küpür değerlerini büyükten küçüğe kuruş cinsinden bir tamsayı dizisine ata:  
   `KUPURLER = {20000, 10000, 5000, 2000, 1000, 500, 100, 50, 25, 10, 5, 1}`
3. `urun_tutari` (float) ve `odenen_para` (float) değişkenlerini kullanıcıdan al.
4. Girdileri doğrula:
   - Eğer `urun_tutari < 0` veya `odenen_para < 0` ise HATA yazdır ve BİTİR.
   - Eğer `odenen_para < urun_tutari` ise "Ödenen para yetersiz" yazdır ve BİTİR.
5. Para üstünü float olarak hesapla: `fark = odenen_para - urun_tutari`.
6. Hassasiyet Analizi:
   - Hatalı (yuvarlamasız) kuruş: `hatali_kurus = (int)(fark * 100.0f)`
   - Doğru (yuvarlanmış) kuruş: `dogru_kurus = (int)roundf(fark * 100.0f)`
7. Eğer `dogru_kurus == 0` ise: "Para üstü yoktur", toplam küpür = 0 yazdır ve BİTİR.
8. `kalan = dogru_kurus`, `toplam_kupur = 0` değerlerini ata.
9. $i = 0$'dan 11'e kadar (tüm küpürler için) döngü kur:
   - `adet = kalan / KUPURLER[i]`
   - Eğer `adet > 0` ise:
     * Küpür adını, adetini ve toplam TL değerini ekrana yazdır.
     * `toplam_kupur = toplam_kupur + adet`
     * `kalan = kalan % KUPURLER[i]`
10. `toplam_kupur` değerini ekrana yazdır.
11. **BİTİR**

---

## 4. AKIŞ ŞEMASI (FLOWCHART)

> **A4 Kağıdına Çizim Notu:** Aşağıdaki adımları standart akış şeması geometrik şekilleriyle sırasıyla alt alta çiziniz:
> - **Elips / Oval:** Başla ve Bitir
> - **Paralelkenar:** Veri Girişi / Çıkışı (`scanf`, `printf`)
> - **Dikdörtgen:** Matematiksel İşlemler ve Atamalar
> - **Eşkenar Dörtgen (Baklava):** Karar ve Kontrol Mekanizmaları (`if`)
> - **Altıgen / Döngü:** Küpür Döngüsü (`for`)

### 4.1. Akış Şeması Diyagramı (Mermaid ve Metin Formatı)

```
           +----------------------------------+
           |              BAŞLA               |  (Elips)
           +----------------------------------+
                            |
                            v
           +----------------------------------+
           | Girdi: urun_tutari, odenen_para  |  (Paralelkenar)
           +----------------------------------+
                            |
                            v
           +----------------------------------+
           | odenen_para < urun_tutari VEYA   |
           | tutarlar negatif mi?             |  (Eşkenar Dörtgen / Karar)
           +----------------------------------+
                 /                      \
           [Evet]                        [Hayır]
             /                              \
            v                                v
+-----------------------+      +------------------------------------------+
| Hata Mesajı Yazdır    |      | fark = odenen_para - urun_tutari         | (Dikdörtgen)
| ve Programı Sonlandır |      | dogru_kurus = (int)roundf(fark * 100.0f) |
+-----------------------+      +------------------------------------------+
            |                                       |
            |                                       v
            |                  +------------------------------------------+
            |                  | dogru_kurus == 0 mı? (Tam ödeme mi?)     | (Eşkenar Dörtgen)
            |                  +------------------------------------------+
            |                        /                      \
            |                  [Evet]                        [Hayır]
            |                    /                              \
            |                   v                                v
            |       +-----------------------+      +------------------------------+
            |       | "Para Üstü Yok" Yaz,  |      | kalan = dogru_kurus          | (Dikdörtgen)
            |       | Toplam Küpür = 0      |      | toplam_kupur = 0, i = 0      |
            |       +-----------------------+      +------------------------------+
            |                   |                               |
            |                   |                               v
            |                   |                  +------------------------------+
            |                   |                  | Döngü: i < 12 (Tüm Küpürler) | (Döngü)
            |                   |                  +------------------------------+
            |                   |                     /                      \
            |                   |               [Evet]                        [Hayır]
            |                   |                 /                              \
            |                   |                v                                v
            |                   |   +--------------------------+    +-----------------------------+
            |                   |   | adet = kalan / KUPUR[i]  |    | Toplam Küpür Sayısını Yazdır|
            |                   |   | adet > 0 ise:            |    +-----------------------------+
            |                   |   |   Ekrana Yaz             |                  |
            |                   |   |   toplam += adet         |                  |
            |                   |   |   kalan %= KUPUR[i]      |                  |
            |                   |   | i = i + 1                |                  |
            |                   |   +--------------------------+                  |
            |                   |                |                                |
            |                   |                +---> (Döngü Başına Dön)         |
            |                   |                                                 |
            +-------------------+-------------------------------------------------+
                                |
                                v
           +----------------------------------+
           |              BİTİR               |  (Elips)
           +----------------------------------+
```

---

## 5. RAPOR SORUSU: ONDALIKLI SAYILARIN HASSASİYETİ (IEEE 754 ANALİZİ)

### 5.1. Deneyin Tanımı
Ödevde verilen özel senaryo:
- **Ürün Tutarı:** `18.70 TL`
- **Ödenen Para:** `20.00 TL`
- **Beklenen Doğru Para Üstü:** `20.00 - 18.70 = 1.30 TL = 130 Kuruş`

Kural 3 gereğince, hiç yuvarlama yapılmadan doğrudan tam sayıya dönüştürme yapıldığında:
```c
int kurus_hatali = (int)((odenen_para - urun_tutari) * 100.0f);
```
Bu işlem çalıştırıldığında programın ürettiği sonuç: **`129 Kuruş`** çıkmaktadır!

### 5.2. Neden 129 Çıkıyor? (IEEE 754 Standardı ve Matematiksel Açıklama)
Bilgisayarlar sayıları 10'luk (desimal) tabanda değil, 2'lik (ikili / binary) tabanda saklar. 

Tek duyarlıklı `float` veri tipi, **IEEE 754 standardına** göre bellekte 32 bit olarak tutulur:
- **1 bit:** İşaret (Sign)
- **8 bit:** Üs (Exponent - Bias: 127)
- **23 bit:** Kesir / Mantissa (Fraction)

#### 1. Kesirli Sayının İkili Karşılığı:
10'luk tabandaki `0.70` kesrinin 2'lik tabandaki karşılığı hesaplandığında:
- $0.70 \times 2 = 1.40 \rightarrow 1$
- $0.40 \times 2 = 0.80 \rightarrow 0$
- $0.80 \times 2 = 1.60 \rightarrow 1$
- $0.60 \times 2 = 1.20 \rightarrow 1$
- $0.20 \times 2 = 0.40 \rightarrow 0$
- $0.40 \times 2 = 0.80 \rightarrow 0$
- $0.80 \times 2 = 1.60 \rightarrow 1$
- $0.60 \times 2 = 1.20 \rightarrow 1$  
Görüldüğü üzere $0.70_{10} = 0.1011001100110011\dots_2$ şeklinde **sonsuza kadar devreden** bir ikili kesirdir.

#### 2. Bellekte Saklama (Kırpma / Yuvarlama):
23 bitlik mantissa kapasitesi nedeniyle bu sayı tam olarak saklanamaz ve en yakın temsiline yuvarlanır:
- `18.70f` bellekteki gerçek değeri: $\approx \mathbf{18.700000762939453125}$ (Hex: `0x4195999A`)
- `20.00f` bellekteki tam değeri: $\mathbf{20.000000000000000000}$ (Hex: `0x41A00000`)

#### 3. Çıkarma İşlemi:
İki değer birbirinden çıkarıldığında:
$$\text{Fark} = 20.000000000000000000 - 18.700000762939453125 = \mathbf{1.299999237060546875}$$
Bu sayı 100 ile çarpıldığında:
$$\text{Fark} \times 100 = \mathbf{129.9999237060546875}$$

#### 4. `(int)` Tür Dönüşümü (Type Casting - Truncation Hatası):
C programlama dilinde bir kayan noktalı sayı `(int)` türüne dönüştürüldüğünde yuvarlama yapılmaz; sayının **ondalık kısmı doğrudan atılır (truncation / sıfıra doğru kırpma)**.
- `(int)(129.999923706...) = 129`

Bu durum sonucunda:
- Gerçekte verilmesi gereken para üstü **130 kuruş** iken,
- Kasada hesaplanan değer **129 kuruş** olmakta,
- Müşteriye **1 kuruş eksik para üstü** verilmektedir.

### 5.3. Küpür Dağılımına Olumsuz Etkisi
| Durum | Kuruş | Verilen Küpürler | Toplam Küpür Adedi |
|---|---|---|---|
| **Hatalı (Yuvarlamasız)** | 129 kr | 1 TL (1), 25 kr (1), **1 kr (4 adet)** | **6 adet** (Müşteri 1 kr zararda) |
| **Doğru (Yuvarlanmış)** | 130 kr | 1 TL (1), 25 kr (1), **5 kr (1 adet)** | **3 adet** (En az küpür + Tam ödeme) |

Hatalı çevrim sadece 1 kuruş eksik vermekle kalmaz, 5 kuruş yerine 4 adet 1 kuruş vererek küpür adedini de iki katına (3'ten 6'ya) çıkarır.

### 5.4. Doğru Yuvarlama Yöntemi ve Çözüm
Doğru çözüm, ondalıklı sayıyı en yakın tamsayıya yuvarlamaktır:
1. **Yöntem 1 (Standart Kütüphane - Tavsiye Edilen):** `<math.h>` kütüphanesindeki `roundf()` fonksiyonu kullanılır:
   ```c
   int dogru_kurus = (int)roundf((odenen_para - urun_tutari) * 100.0f);
   ```
   `roundf(129.9999237f)` ifadesi matematiksel olarak `130.0f` değerini üretir ve `(int)` dönüşümüyle kesin olarak `130` elde edilir.
2. **Yöntem 2 (Matematiksel Ofset Ekleme):**
   ```c
   int dogru_kurus = (int)(((odenen_para - urun_tutari) * 100.0f) + 0.5f);
   ```
   `129.9999f + 0.5f = 130.4999f` olur ve kırpıldığında yine `130` sonucuna ulaşılır.

Programımızda endüstri standardı olan **`roundf()`** fonksiyonu kullanılmıştır.

---

## 6. PROGRAMIN C KAYNAK KODU (`akilli_kasa.c`)

```c
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

/* Kupurlerin isimleri */
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

/* Kupur dagilimini hesaplayip yazdiran fonksiyon */
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

    /* Girdi alma */
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

    /* Girdi dogrulama */
    if (urun_tutari < 0.0f || odenen_para < 0.0f) {
        printf("HATA: Tutarlar negatif olamaz!\n");
        return 1;
    }

    if (odenen_para < urun_tutari) {
        printf("HATA: Odenen para (%.2f TL), urun tutarindan (%.2f TL) az olamaz!\n",
               odenen_para, urun_tutari);
        return 1;
    }

    /* Para ustu hesabi */
    float para_ustu_float = odenen_para - urun_tutari;

    /* Kural 3: Hatali dogrudan cevrim vs Dogru yuvarlama */
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

    /* Kural 1 & 2: Kupur hesabi kurus tamsayisi ile yapilir, toplam yazdirilir */
    kupur_dagilimi_yazdir(dogru_kurus);

    return 0;
}
```

---

## 7. PROGRAMIN ÇALIŞMA EKRAN ÇIKTILARI (TEST SONUÇLARI)

### Test Senaryosu 1: Ödev Özel Örneği (18.70 TL / 20.00 TL)
```
==========================================================
               AKILLI KASA PARA USTU SISTEMI              
==========================================================
Urun tutarini giriniz (TL cinsinden, ornek: 18.70): 18.70
Musterinin odedigi parayi giriniz (TL, ornek: 20.00): 20.00

----------------------------------------------------------
                      HESAPLAMA RAPORU                    
----------------------------------------------------------
Urun Tutari        : 18.70 TL
Odenen Para        : 20.00 TL
Para Ustu (Float)  : 1.299999 TL
Para Ustu x 100    : 129.999924
----------------------------------------------------------
[1] Yuvarlamasiz Donusum (int)(fark * 100) : 129 Kurus (1.29 TL)
[2] Dogru Yuvarlama   (int)roundf(fark*100): 130 Kurus (1.30 TL)

>>> DIKKAT (IEEE 754 Hassasiyet Sorunu Tespit Edildi!):
    Ondalikli sayilar ikili (binary) sistemde saklanirken
    18.70f sayisi tam temsil edilemez (devirli ikili kesir).
    Fark register'da 1.299999... olarak saklanir.
    100 ile carpilip (int) cast yapildiginda 129.9999 sayisinin
    ondalik kismi kirpilir (truncation) ve 129 Kurus elde edilir!
    Bu durumda musteriye 1 Kurus EKSIK para ustu verilmis olur.
    Cozum olarak roundf() ile en yakin tam sayiya (130) yuvarlandi.

==========================================================
        VERILECEK EN AZ SAYIDA KUPUR LISTESI
==========================================================
  1 TL Madeni Para          :  1 adet  (  1.00 TL)
  25 Kurus Madeni Para      :  1 adet  (  0.25 TL)
  5 Kurus Madeni Para       :  1 adet  (  0.05 TL)
----------------------------------------------------------
  TOPLAM VERILEN KUPUR SAYISI: 3 adet
==========================================================
```

### Test Senaryosu 2: Tam Ödeme (50.00 TL / 50.00 TL)
```
Urun tutari: 50.00 | Odenen: 50.00
Para ustu yoktur (Tam odeme yapilmistir).
Toplam Kupur Adedi: 0
```

### Test Senaryosu 3: Büyük Banknotlar (12.50 TL / 200.00 TL)
- Para Üstü: 187.50 TL (18750 Kuruş)
- Küpürler: 100 TL (1), 50 TL (1), 20 TL (1), 10 TL (1), 5 TL (1), 1 TL (2), 50 Kr (1)
- Toplam Küpür: **8 adet**

---

## 8. GITHUB'A YÜKLEME VE TEAMS TESLİMİ ADIMLARI

Ödev yönergesinde belirtildiği üzere, Teams üzerinden yalnızca **GitHub proje bağlantısı** teslim edilecektir. Projeyi GitHub'a yüklemek için izlenen adımlar şunlardır:

1. **Terminalde Git Deposunu Başlatma:**
   ```bash
   git init
   git add .
   git commit -m "feat: Akilli Kasa para ustu hesaplama ve IEEE 754 float hassasiyet analizi"
   ```
2. **GitHub Üzerinde Yeni Bir Repo Açma:**
   - [github.com/new](https://github.com/new) adresine gidilir.
   - Depo adı olarak `akilli-kasa` girilir, "Public" seçilir.
3. **Uzak Depoyu Ekleyip Kodu Yükleme (Push):**
   ```bash
   git branch -M main
   git remote add origin https://github.com/KULLANICI_ADINIZ/akilli-kasa.git
   git push -u origin main
   ```
4. **Teams Teslimi:**
   - Teams ödev teslim formundaki metin kutusuna doğrudan GitHub reposu linki (örneğin: `https://github.com/ali/akilli-kasa`) yapıştırılıp teslim edilir.
