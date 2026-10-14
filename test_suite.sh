#!/bin/bash
# Akilli Kasa - Test Senaryolari

set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo "=== Akilli Kasa Testleri ==="

TOPLAM=0
BASARILI=0

calistir_test() {
    local test_adi="$1"
    local girdi="$2"
    local beklenen_kelime="$3"
    local beklenen_kod="$4"
    
    TOPLAM=$((TOPLAM + 1))
    echo -n "Test $TOPLAM: $test_adi ... "

    set +e
    cikti=$(printf "%b" "$girdi" | ./akilli_kasa 2>&1)
    kod=$?
    set -e

    if [ "$kod" -eq "$beklenen_kod" ] && echo "$cikti" | grep -q "$beklenen_kelime"; then
        echo -e "${GREEN}Gecti${NC}"
        BASARILI=$((BASARILI + 1))
    else
        echo -e "${RED}Basarisiz${NC}"
        echo "Beklenen kod: $beklenen_kod, Donen: $kod"
        echo "Beklenen icerik: $beklenen_kelime"
        echo "Program ciktisi:"
        echo "$cikti"
    fi
}

# 1. Odev ornegi (18.70 TL urun, 20.00 TL odenen)
calistir_test "18.70 TL / 20.00 TL (Float yuvarlama)" "18.70\n20.00\n" "TOPLAM VERILEN KUPUR SAYISI: 3 adet" 0

# 2. Tam odeme (Para ustu yok)
calistir_test "Tam Odeme (50.00 TL / 50.00 TL)" "50.00\n50.00\n" "Toplam Kupur Adedi: 0" 0

# 3. Buyuk banknot dagilimi
calistir_test "Buyuk Banknotlar (12.50 TL / 200.00 TL)" "12.50\n200.00\n" "TOPLAM VERILEN KUPUR SAYISI: 8 adet" 0

# 4. Kurus kademeleri (1.01 TL / 2.00 TL -> 0.99 TL)
calistir_test "Kurus Kademeleri (1.01 TL / 2.00 TL)" "1.01\n2.00\n" "TOPLAM VERILEN KUPUR SAYISI: 8 adet" 0

# 5. Yetersiz odeme hatasi
calistir_test "Yetersiz Odeme Kontrolu (30.00 TL / 20.00 TL)" "30.00\n20.00\n" "HATA: Odenen para" 1

# 6. Negatif tutar hatasi
calistir_test "Negatif Tutar Kontrolu (-5.00 TL / 10.00 TL)" "-5.00\n10.00\n" "HATA: Tutarlar negatif olamaz" 1

echo "----------------------------------------"
echo -e "Sonuc: $BASARILI / $TOPLAM test basarili."

if [ "$BASARILI" -ne "$TOPLAM" ]; then
    exit 1
fi
