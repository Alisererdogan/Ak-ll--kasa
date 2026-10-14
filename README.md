# Akıllı Kasa - Para Üstü Hesaplama Sistemi

Bu proje, bir satış noktasında girilen ürün tutarı ve müşterinin ödediği paraya göre verilecek para üstünü **en az sayıda banknot ve madeni para (küpür)** ile hesaplayan bir C programıdır.

Ayrıca bilgisayar mimarisinde `float` türündeki sayıların ikili (binary) sistemde saklanmasından kaynaklanan **IEEE 754 hassasiyet kaybını** (18.70 TL ürün tutarı ve 20.00 TL ödeme senaryosundaki 1 kuruşluk farkı) inceleyen bir deney programı ve ayrıntılı ödev raporunu içerir.

## Dosyalar

- `akilli_kasa.c`: Ana kasa uygulaması. Girdileri alır, float hassasiyet kaybını önleyerek kuruş bazında en uygun küpür dağılımını hesaplar.
- `rapor_deneyi.c`: 18.70 TL örneğinde oluşan float hassasiyet kaybını bit ve mantissa düzeyinde inceleyen analiz programı.
- `ODEV_RAPORU.md`: Ödev için hazırlanan detaylı rapor (amaç, algoritma, akış şeması, IEEE 754 incelemesi ve test sonuçları).
- `Makefile`: Derleme, çalıştırma ve test otomasyonu.
- `test_suite.sh`: Programın farklı senaryolarda doğru çalıştığını doğrulayan test betiği.

## Derleme ve Çalıştırma

Projeyi derlemek ve çalıştırmak için GCC ve Make gereklidir:

```bash
# Tüm programları derle
make

# Kasa programını çalıştır
make run

# Float hassasiyet deneyini çalıştır
make rapor

# Test senaryolarını çalıştır
make test

# Derleme dosyalarını temizle
make clean
```

## Küpür Dağılımı Mantığı

Para üstü hesaplanırken **Açgözlü (Greedy) Algoritma** kullanılmıştır. Türk Lirası para birimindeki küpürler (200, 100, 50, 20, 10, 5 TL ve 1 TL, 50, 25, 10, 5, 1 Kuruş) kanonik bir yapıya sahip olduğu için, her adımda verilebilecek en büyük küpür seçilerek her zaman minimum sayıda para teslim edilir.

## Float Hassasiyet Sorunu Özeti (18.70 TL Örneği)

- Bilgisayarlar ondalıklı sayıları ikili (binary) tabanda saklar. 10'luk tabandaki `0.70` kesri ikili tabanda devirli (`0.10110011..._2`) olduğundan 32-bit `float` mantissasına tam sığmaz.
- `20.00f - 18.70f` çıkarma işlemi bellekte `1.29999924` olarak tutulur.
- Bu değer 100 ile çarpıldığında `129.999924` elde edilir.
- Doğrudan `(int)` tür dönüşümü yapıldığında ondalık kısım atılır (truncation) ve sonuç **129 Kuruş** çıkar (1 kuruş eksik kalır).
- Çözüm olarak `<math.h>` kütüphanesindeki `roundf()` fonksiyonu kullanılarak değer matematiksel olarak en yakın tamsayıya (**130 Kuruş**) yuvarlanmıştır.

## Örnek Çıktı

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
